#include "include/mqoe.h"
#include <linux/pkt_sched.h>
#include <linux/netlink.h>
#include "include/cake.h"

static const struct nla_policy cake_policy[TCA_CAKE_MAX + 1] = {
	[TCA_CAKE_BASE_RATE64]   = { .type = NLA_U64 },
	[TCA_CAKE_DIFFSERV_MODE] = { .type = NLA_U32 },
	[TCA_CAKE_ATM]		 = { .type = NLA_U32 },
	[TCA_CAKE_FLOW_MODE]     = { .type = NLA_U32 },
	[TCA_CAKE_OVERHEAD]      = { .type = NLA_S32 },
	[TCA_CAKE_RTT]		 = { .type = NLA_U32 },
	[TCA_CAKE_TARGET]	 = { .type = NLA_U32 },
	[TCA_CAKE_AUTORATE]      = { .type = NLA_U32 },
	[TCA_CAKE_MEMORY]	 = { .type = NLA_U32 },
	[TCA_CAKE_NAT]		 = { .type = NLA_U32 },
	[TCA_CAKE_RAW]		 = { .type = NLA_U32 },
	[TCA_CAKE_WASH]		 = { .type = NLA_U32 },
	[TCA_CAKE_MPU]		 = { .type = NLA_U32 },
	[TCA_CAKE_INGRESS]	 = { .type = NLA_U32 },
	[TCA_CAKE_ACK_FILTER]	 = { .type = NLA_U32 },
	[TCA_CAKE_SPLIT_GSO]	 = { .type = NLA_U32 },
	[TCA_CAKE_FWMARK]	 = { .type = NLA_U32 },
};

/*
static int cake_dump_stats(struct Qdisc *sch, struct gnet_dump *d)
{
	struct nlattr *stats = nla_nest_start(d->skb, TCA_STATS_APP);
	struct cake_sched_data *q = qdisc_priv(sch);
	struct nlattr *tstats, *ts;
	int i;

	if (!stats)
		return -1;

#define PUT_STAT_U32(attr, data) do {				       \
		if (nla_put_u32(d->skb, TCA_CAKE_STATS_ ## attr, data)) \
			goto nla_put_failure;			       \
	} while (0)
#define PUT_STAT_U64(attr, data) do {				       \
		if (nla_put_u64_64bit(d->skb, TCA_CAKE_STATS_ ## attr, \
					data, TCA_CAKE_STATS_PAD)) \
			goto nla_put_failure;			       \
	} while (0)

	PUT_STAT_U64(CAPACITY_ESTIMATE64, q->avg_peak_bandwidth);
	PUT_STAT_U32(MEMORY_LIMIT, q->buffer_limit);
	PUT_STAT_U32(MEMORY_USED, q->buffer_max_used);
	PUT_STAT_U32(AVG_NETOFF, ((q->avg_netoff + 0x8000) >> 16));
	PUT_STAT_U32(MAX_NETLEN, q->max_netlen);
	PUT_STAT_U32(MAX_ADJLEN, q->max_adjlen);
	PUT_STAT_U32(MIN_NETLEN, q->min_netlen);
	PUT_STAT_U32(MIN_ADJLEN, q->min_adjlen);

#undef PUT_STAT_U32
#undef PUT_STAT_U64

	tstats = nla_nest_start(d->skb, TCA_CAKE_STATS_TIN_STATS);
	if (!tstats)
		goto nla_put_failure;

#define PUT_TSTAT_U32(attr, data) do {					\
		if (nla_put_u32(d->skb, TCA_CAKE_TIN_STATS_ ## attr, data)) \
			goto nla_put_failure;				\
	} while (0)
#define PUT_TSTAT_U64(attr, data) do {					\
		if (nla_put_u64_64bit(d->skb, TCA_CAKE_TIN_STATS_ ## attr, \
					data, TCA_CAKE_TIN_STATS_PAD))	\
			goto nla_put_failure;				\
	} while (0)

	for (i = 0; i < q->tin_cnt; i++) {
		struct cake_tin_data *b = &q->tins[q->tin_order[i]];

		ts = nla_nest_start(d->skb, i + 1);
		if (!ts)
			goto nla_put_failure;

		PUT_TSTAT_U64(THRESHOLD_RATE64, b->tin_rate_bps);
		PUT_TSTAT_U64(SENT_BYTES64, b->bytes);
		PUT_TSTAT_U32(BACKLOG_BYTES, b->tin_backlog);

		PUT_TSTAT_U32(TARGET_US,
			      ktime_to_us(ns_to_ktime(b->cparams.target)));
		PUT_TSTAT_U32(INTERVAL_US,
			      ktime_to_us(ns_to_ktime(b->cparams.interval)));

		PUT_TSTAT_U32(SENT_PACKETS, b->packets);
		PUT_TSTAT_U32(DROPPED_PACKETS, b->tin_dropped);
		PUT_TSTAT_U32(ECN_MARKED_PACKETS, b->tin_ecn_mark);
		PUT_TSTAT_U32(ACKS_DROPPED_PACKETS, b->ack_drops);

		PUT_TSTAT_U32(PEAK_DELAY_US,
			      ktime_to_us(ns_to_ktime(b->peak_delay)));
		PUT_TSTAT_U32(AVG_DELAY_US,
			      ktime_to_us(ns_to_ktime(b->avge_delay)));
		PUT_TSTAT_U32(BASE_DELAY_US,
			      ktime_to_us(ns_to_ktime(b->base_delay)));

		PUT_TSTAT_U32(WAY_INDIRECT_HITS, b->way_hits);
		PUT_TSTAT_U32(WAY_MISSES, b->way_misses);
		PUT_TSTAT_U32(WAY_COLLISIONS, b->way_collisions);

		PUT_TSTAT_U32(SPARSE_FLOWS, b->sparse_flow_count +
					    b->decaying_flow_count);
		PUT_TSTAT_U32(BULK_FLOWS, b->bulk_flow_count);
		PUT_TSTAT_U32(UNRESPONSIVE_FLOWS, b->unresponsive_flow_count);
		PUT_TSTAT_U32(MAX_SKBLEN, b->max_skblen);

		PUT_TSTAT_U32(FLOW_QUANTUM, b->flow_quantum);
		nla_nest_end(d->skb, ts);
	}

#undef PUT_TSTAT_U32
#undef PUT_TSTAT_U64

	nla_nest_end(d->skb, tstats);
	return nla_nest_end(d->skb, stats);

nla_put_failure:
	nla_nest_cancel(d->skb, stats);
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 8, 0)
	sch_tree_unlock(sch);
#endif
	return -1;
}

static int cake_dump_class_stats(struct Qdisc *sch, unsigned long cl,
				 struct gnet_dump *d)
{
	struct cake_sched_data *q = qdisc_priv(sch);
	const struct cake_flow *flow = NULL;
	struct gnet_stats_queue qs = { 0 };
	struct nlattr *stats;
	u32 idx = cl - 1;

	if (idx < CAKE_QUEUES * q->tin_cnt) {
		const struct cake_tin_data *b = \
			&q->tins[q->tin_order[idx / CAKE_QUEUES]];
		const struct sk_buff *skb;

		flow = &b->flows[idx % CAKE_QUEUES];

		if (flow->head) {
			cake_maybe_lock(sch);
			skb = flow->head;
			while (skb) {
				qs.qlen++;
				skb = skb->next;
			}
			cake_maybe_unlock(sch);
		}
		qs.backlog = b->backlogs[idx % CAKE_QUEUES];
		qs.drops = flow->dropped;
	}
	if (gnet_stats_copy_queue(d, NULL, &qs, qs.qlen) < 0)
		return -1;
	if (flow) {
		ktime_t now = ktime_get();

		stats = nla_nest_start(d->skb, TCA_STATS_APP);
		if (!stats)
			return -1;

#define PUT_STAT_U32(attr, data) do {				       \
		if (nla_put_u32(d->skb, TCA_CAKE_STATS_ ## attr, data)) \
			goto nla_put_failure;			       \
	} while (0)
#define PUT_STAT_S32(attr, data) do {				       \
		if (nla_put_s32(d->skb, TCA_CAKE_STATS_ ## attr, data)) \
			goto nla_put_failure;			       \
	} while (0)

		PUT_STAT_S32(DEFICIT, flow->deficit);
		PUT_STAT_U32(DROPPING, flow->cvars.dropping);
		PUT_STAT_U32(COBALT_COUNT, flow->cvars.count);
		PUT_STAT_U32(P_DROP, flow->cvars.p_drop);
		if (flow->cvars.p_drop) {
			PUT_STAT_S32(BLUE_TIMER_US,
				     ktime_to_us(
					     ktime_sub(now,
						       flow->cvars.blue_timer)));
		}
		if (flow->cvars.dropping) {
			PUT_STAT_S32(DROP_NEXT_US,
				     ktime_to_us(
					     ktime_sub(now,
						       flow->cvars.drop_next)));
		}

		if (nla_nest_end(d->skb, stats) < 0)
			return -1;
	}

	return 0;

nla_put_failure:
	nla_nest_cancel(d->skb, stats);
	return -1;
}
*/

// This is specific to cake diffserv4 because, why not?
// We use saturating math anyway throughout.
// My intent here is to make sure we use saturating math
// And to fiddle with various analytical math methods along the way

#ifndef BUSYBOX
int main(int argc, char **argv) {
		cake_stats *p = calloc(sizeof(cake_stats),10000);
		for(int i = 0; i < 10000; i++) {
				p[i].sent_bytes[0] = random();
				p[i].sent_bytes[1] = random();
				p[i].sent_bytes[2] = random();
				p[i].sent_bytes[3] = random();
		}
		cake_stats sum = {};
		for(int i=0;i<10000;i++) {
				sum.sent_bytes[0] += p[i].sent_bytes[0];
				sum.sent_bytes[1] += p[i].sent_bytes[1];
				sum.sent_bytes[2] += p[i].sent_bytes[2];
				sum.sent_bytes[3] += p[i].sent_bytes[3];
		}
		printf("sent: %llu\nsent: %llu\nsent: %llu\nsent: %llu\n",
				sum.sent_bytes[0],sum.sent_bytes[1],sum.sent_bytes[2],sum.sent_bytes[3]);
		return(0);
}
#endif
