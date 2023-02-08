#include "include/mqoe.h"
#include <asm/types.h>
#include <net/if.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sys/socket.h>

/*
       RTM_NEWQDISC, RTM_DELQDISC, RTM_GETQDISC
              Add, remove, or get a queueing discipline.  The message
              contains a struct tcmsg and may be followed by a series of
              attributes.

              struct tcmsg {
                  unsigned char    tcm_family;
                  int              tcm_ifindex;   // interface index 
                  __u32            tcm_handle;    // Qdisc handle 
                  __u32            tcm_parent;    // Parent qdisc 
                  __u32            tcm_info;
              };

                    
*/

/*            TCA_UNSPEC   -                    unspecified
              TCA_KIND     asciiz string        Name of queueing discipline
              TCA_OPTIONS  byte sequence        Qdisc-specific options follow
              TCA_STATS    struct tc_stats      Qdisc statistics
              TCA_XSTATS   qdisc-specific       Module-specific statistics
*/

// from http://ifeanyi.co/posts/linux-namespaces-part-4/

// ip link add ifname type veth ifname name peername
void create_veth(int sock_fd, char *ifname, char *peername)
{
    __u16 flags =
            NLM_F_REQUEST  // This is a request message
            | NLM_F_CREATE // Create the device if it doesn't exist
            | NLM_F_EXCL   // If it already exists, do nothing
            | NLM_F_ACK;   // Reply with an acknowledgement or error

    // Initialise request message.
    struct nl_req req = {
            .n.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg)),
            .n.nlmsg_flags = flags,
            .n.nlmsg_type = RTM_NEWLINK, // This is a netlink message
            .i.ifi_family = PF_NETLINK,
    };
    struct nlmsghdr *n = &req.n;
    int maxlen = sizeof(req);

    /*
     * Create an attribute r0 with the veth info. e.g if ifname is veth0
     * then the following will be appended to the message
     * {
     *   rta_type: IFLA_IFNAME
     *   rta_len: 5 (len(veth0) + 1)
     *   data: veth0\0
     * }
     */
    addattr_l(n, maxlen, IFLA_IFNAME, ifname, strlen(ifname) + 1);

    // Add a nested attribute r1 within r0 containing iface info
    struct rtattr *linfo =
            addattr_nest(n, maxlen, IFLA_LINKINFO);
    // Specify the device type is veth
    addattr_l(&req.n, sizeof(req), IFLA_INFO_KIND, "veth", 5);

    // Add another nested attribute r2
    struct rtattr *linfodata =
            addattr_nest(n, maxlen, IFLA_INFO_DATA);

    // This next nested attribute r3 one contains the peer name e.g veth1
    struct rtattr *peerinfo =
            addattr_nest(n, maxlen, VETH_INFO_PEER);
    n->nlmsg_len += sizeof(struct ifinfomsg);
    addattr_l(n, maxlen, IFLA_IFNAME, peername, strlen(peername) + 1);
    addattr_nest_end(n, peerinfo); // end r3 nest

    addattr_nest_end(n, linfodata); // end r2 nest
    addattr_nest_end(n, linfo); // end r1 nest

    // Send the message
    send_nlmsg(sock_fd, n);
}

// ip link add ifname type veth ifname name peername
void create_veth(int sock_fd, char *ifname, char *peername)
{
    __u16 flags =
            NLM_F_REQUEST  // This is a request message
            | NLM_F_CREATE // Create the device if it doesn't exist
            | NLM_F_EXCL   // If it already exists, do nothing
            | NLM_F_ACK;   // Reply with an acknowledgement or error

    // Initialise request message.
    struct nl_req req = {
            .n.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg)),
            .n.nlmsg_flags = flags,
            .n.nlmsg_type = RTM_NEWLINK, // This is a netlink message
            .i.ifi_family = PF_NETLINK,
    };
    struct nlmsghdr *n = &req.n;
    int maxlen = sizeof(req);

    /*
     * Create an attribute r0 with the veth info. e.g if ifname is veth0
     * then the following will be appended to the message
     * {
     *   rta_type: IFLA_IFNAME
     *   rta_len: 5 (len(veth0) + 1)
     *   data: veth0\0
     * }
     */
    addattr_l(n, maxlen, IFLA_IFNAME, ifname, strlen(ifname) + 1);

    // Add a nested attribute r1 within r0 containing iface info
    struct rtattr *linfo =
            addattr_nest(n, maxlen, IFLA_LINKINFO);
    // Specify the device type is veth
    addattr_l(&req.n, sizeof(req), IFLA_INFO_KIND, "veth", 5);

    // Add another nested attribute r2
    struct rtattr *linfodata =
            addattr_nest(n, maxlen, IFLA_INFO_DATA);

    // This next nested attribute r3 one contains the peer name e.g veth1
    struct rtattr *peerinfo =
            addattr_nest(n, maxlen, VETH_INFO_PEER);
    n->nlmsg_len += sizeof(struct ifinfomsg);
    addattr_l(n, maxlen, IFLA_IFNAME, peername, strlen(peername) + 1);
    addattr_nest_end(n, peerinfo); // end r3 nest

    addattr_nest_end(n, linfodata); // end r2 nest
    addattr_nest_end(n, linfo); // end r1 nest

    // Send the message
    send_nlmsg(sock_fd, n);
}

// $ ip link set veth1 netns coke
void move_if_to_pid_netns(int sock_fd, char *ifname, int netns)
{
    struct nl_req req = {
            .n.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg)),
            .n.nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK,
            .n.nlmsg_type = RTM_NEWLINK,
            .i.ifi_family = PF_NETLINK,
    };

    addattr_l(&req.n, sizeof(req), IFLA_NET_NS_FD, &netns, 4);
    addattr_l(&req.n, sizeof(req), IFLA_IFNAME,
              ifname, strlen(ifname) + 1);
    send_nlmsg(sock_fd, &req.n);
}

static void prepare_netns(int child_pid)
{
    char *veth = "veth0";
    char *vpeer = "veth1";
    char *veth_addr = "10.1.1.1";
    char *vpeer_addr = "10.1.1.2";
    char *netmask = "255.255.255.0";

    // Create our netlink socket
    int sock_fd = create_socket(
            PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);

    // ... and our veth pair veth0 <=> veth1.
    create_veth(sock_fd, veth, vpeer);

    // veth0 is in our current (initial) namespace
    // so we can bring it up immediately.
    if_up(veth, veth_addr, netmask);

    // ... veth1 will be moved to the command namespace.
    // To do that though we need to grab a file descriptor
    // to and enter the commands namespace but first we must
    // remember our current namespace so we can get back to it
    // when we're done.
    int mynetns = get_netns_fd(getpid());
    int child_netns = get_netns_fd(child_pid);

    // Move veth1 to the command network namespace.
    move_if_to_pid_netns(sock_fd, vpeer, child_netns);

    // ... then enter it
    if (setns(child_netns, CLONE_NEWNET)) {
        die("cannot setns for child at pid %d: %m\n", child_pid);
    }

    // ... and bring veth1 up
    if_up(vpeer, vpeer_addr, netmask);

    // ... before moving back to our initial network namespace.
    if (setns(mynetns, CLONE_NEWNET)) {
        die("cannot restore previous netns: %m\n");
    }

    close(sock_fd);
}

    ...
    // Get the writable end of the pipe.
    int pipe = params.fd[1];

    prepare_userns(cmd_pid);
    prepare_netns(cmd_pid);

    // Signal to the command process we're done with setup.
    ...

/* Example set the mtu */
int mtu(int c, char **argv) {
struct {
       struct nlmsghdr  nh;
       struct ifinfomsg iface;
       char   attrbuf[512];
} req;

           struct rtattr *rta;
           int rtnetlink_sk = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
		   unsigned int ifindex = if_nametoindex("dummy0");
 	   if (ifindex == 0) { printf("Cannot get ifindex for dummy device\n"); exit(-1); }
           memset(&req, 0, sizeof(req));
           req.nh.nlmsg_len = NLMSG_LENGTH(sizeof(req.iface));
           req.nh.nlmsg_flags = NLM_F_REQUEST;
           req.nh.nlmsg_type = RTM_NEWLINK;
           req.iface.ifi_family = AF_UNSPEC;
           req.iface.ifi_index = ifindex;
           req.iface.ifi_change = 0xffffffff; /* ??? */
           rta = (struct rtattr *)(((char *) &req) +
                                    NLMSG_ALIGN(req.nh.nlmsg_len));
           rta->rta_type = IFLA_MTU;
           rta->rta_len = RTA_LENGTH(sizeof(mtu));
           req.nh.nlmsg_len = NLMSG_ALIGN(req.nh.nlmsg_len) +
                                         RTA_LENGTH(sizeof(mtu));
           memcpy(RTA_DATA(rta), &mtu, sizeof(mtu));
           send(rtnetlink_sk, &req, req.nh.nlmsg_len, 0);
}
/*
struct tcmsg {
		unsigned char    tcm_family;
		int              tcm_ifindex;   // interface index
		__u32            tcm_handle;    // Qdisc handle
		__u32            tcm_parent;    // Parent qdisc
		__u32            tcm_info;
};

*/												\

int main(int c, char **argv) {
struct {
       struct nlmsghdr  nh;
       struct tcmsg tcstuff;
       char   attrbuf[512];
} req;
		// struct tcmsg2 req;
		struct rtattr *rta;
		int rtnetlink_sk = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
		unsigned int ifindex = if_nametoindex("dummy0");
		if (ifindex == 0) { printf("Cannot get ifindex for dummy device\n"); exit(-1); }
           memset(&req, 0, sizeof(tcmsg2));
           req.nh.nlmsg_len = NLMSG_LENGTH(sizeof(tcmsg2));
           req.nh.nlmsg_flags = NLM_F_REQUEST;
           req.nh.nlmsg_type = RTM_GETQDISC;
           req.iface.ifi_family = AF_UNSPEC;
           req.iface.ifi_index = ifindex;
           req.iface.ifi_change = 0xffffffff; /* ??? */
           rta = (struct rtattr *)(((char *) &req) +
                                    NLMSG_ALIGN(req.nh.nlmsg_len));
           rta->rta_type = IFLA_MTU;
           rta->rta_len = RTA_LENGTH(sizeof(mtu));
           req.nh.nlmsg_len = NLMSG_ALIGN(req.nh.nlmsg_len) +
                                         RTA_LENGTH(sizeof(req.tcmsg));
           memcpy(RTA_DATA(rta), &req.tcmsg, sizeof(req.tcmsg));
           send(rtnetlink_sk, &req, req.nh.nlmsg_len, 0);
}
