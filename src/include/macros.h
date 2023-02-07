#pragma once

#define BIT(val) (1<<(val))

#define PUT_STAT_U32(attr, data) do {							\
		if (nla_put_u32(d->skb, TCA_CAKE_STATS_ ## attr, data)) \
			goto nla_put_failure;			       \
	} while (0)
#define PUT_STAT_U64(attr, data) do {				       \
		if (nla_put_u64_64bit(d->skb, TCA_CAKE_STATS_ ## attr, \
					data, TCA_CAKE_STATS_PAD)) \
			goto nla_put_failure;			

/*
struct CakeTin {
  threshold_rate64: Nla64,
  sent_bytes64: Nla64,
  backlog_bytes: Nla32,
  target_us: Nla32,
  interval_us: Nla32,
  sent_packets: Nla32,
  dropped_packets: Nla32,
  ecn_marked_packets: Nla32,
  acks_dropped_packets: Nla32,
  peak_delay_us: Nla32,
  avg_delay_us: Nla32,
  base_delay_us: Nla32,
  way_indirect_hits: Nla32,
  way_missed: Nla32,
  way_collisions: Nla32,
  sparse_flows: Nla32,
  bulk_flows: Nla32,
  unresponsive_flows: Nla32,
  max_skblen: Nla32,
  flow_quantum: Nla32,  
}
*/
