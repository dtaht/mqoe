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

/* Example set the mtu */
int mtu(int c, char **argv) {
struct {
       struct nlmsghdr  nh;
       struct ifinfomsg iface;
       char   attrbuf[512];
} req;

           struct rtattr *rta;
           unsigned int mtu = 1000;
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
		unsigned int mtu = 1000;
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
                                         RTA_LENGTH(sizeof(mtu));
           memcpy(RTA_DATA(rta), &mtu, sizeof(mtu));
           send(rtnetlink_sk, &req, req.nh.nlmsg_len, 0);
}
