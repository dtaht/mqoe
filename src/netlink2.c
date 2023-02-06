#include <linux/pkt_sched.h>

struct tc_stats TC_stats;
struct tc_stats2 TC_stats2;
struct tcmsg {
       unsigned char    tcm_family;
       int              tcm_ifindex;   /* interface index */
       __u32            tcm_handle;    /* Qdisc handle */
       __u32            tcm_parent;    /* Parent qdisc */
	   __u32            tcm_info;
};


for(attr =, attr) {
		switch(attr.type) {
		case TCA_KIND:
			feed forward into parsing next message
		case TCA_STATS:
		case TCA_STATS2:
		}
}
