#ifndef MQOE_BUS_H_
#define MQOE_BUS_H_
#include ¨include/macros.h¨

#if __GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 27)
static inline int memfd_create(const char *name, unsigned int flags) {
    return syscall(__NR_memfd_create, name, flags);
}
#endif

enum cmds {
		NOP = BIT(0),
		ACK = BIT(1),
		PING = BIT(2),
		ISO = BIT(3), // isochronous test
		FAIL = BIT(4),
		CPUINFO = BIT(7),
		MEMINFO = BIT(7),
		CurrentThroughput = BIT(5),
		HostCounters = BIT(6),
		TopDownloads = BIT(7),
		BestRtt = BIT(8),
		MappedIPs = BIT(9),
		XdpPping = BIT(10),
		RttHistgram = BIT(11),
		Reload = BIT(12),
		Validate = BIT(13),
		Shutdown = BIT(14),
		QueueStats = BIT(15),
		TraceRoute = BIT(16),
		Interval = BIT(17),
		NEWBPFFD = BIT(18),
}

enum filters {
		// Filters
		UnknownIPs = BIT(0),
		IPV4 = BIT(1),
		IPV6 = BIT(2),
		LL = BIT(3),
		BEST = BIT(4),
		MYNETWORK = BIT(5),
		ASN = BIT(6), // think about this
		MCAST = BIT(6), // 224
		MPLS = BIT(7),
		ROUTING = BIT(8),
		SPEED = BIT(9), // over? under?
		RFC1918 = BIT(10),
		EXP = BIT(11), // 240
        TEST = BIT(12),
		ASC = BIT(13), // sort order

// FIXME
		TIN0 = BIT(6),
		TIN1 = BIT(7),
		TIN2 = BIT(8),
		TIN3 = BIT(9),
}

struct mpoe_msg {
		u64 cmd,
		u64 filt,
		u64 start,
		u64 size,
}

struct mpoe_resp {
		u64 cmd,
		u64 filt,
		u64 start,
		u64 size,
}

// Filt to ebpf compiler

#define user_security_filters
#define admin_security_filters

#define ALL_TINS (TIN0 | TIN1 | TIN2 | TIN4)
#define ALL_PROTOS (MCAST | LL | IPV6 | IPV6 | EXP | RFC1918 | MPLS )
#define FILT_DEFAULT (ALL_TINS | ALL_PROTOS )
#define ODD besomethingelse

#endif // MQOE_BUS_H_
