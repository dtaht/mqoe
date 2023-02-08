#pragma once
#include ¨include/macros.h¨

#if __GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 27)
static inline int memfd_create(const char *name, unsigned int flags) {
    return syscall(__NR_memfd_create, name, flags);
}
#endif

// fixme, look over what we did in freeswitch/asterisk
// A property of a message bus or protocol, long term, is that applications
// written in other languages need to be able to connect to it.
// Also, shared memory is usually 20x faster than unix sockets.

enum cmds {
		// These are also responses, defined once, no need to have a second name
		NOP, // A nop is means to probe for other OS errors not expecting a response
		OK, // are you OK?
		ACK, // Sure I am OK
		NACK, // No I am not OK
		PING,
		ISO,
		FAIL, // These test the message bus
		ERROR, // This tells the other side what errors are going on
		INFO, // This tells the other side what INFO notices are happening
		Reload,
		Validate,
		Shutdown, // Actually I would use all uppercase due to my assembly days
		Interval,
		Alert, // send an alert or alert on some parameter
		BOOTTIME,
		CURTIME,
		PSINFO, // I care a heck of a lot more about context switch irq service than load
		CSINFO, // I care a heck of a lot more about context switch irq service than load
		CPUINFO, // I care a heck of a lot more about context switch irq service than load
		THREADINFO,
		MEMINFO, // see procps utilities top for this implementation
		BALANCE,
		HostCounters,
		CurrentThroughput,
		Downloads,
		Uploads,
		Rtt, // formerly best RTT
		XdpPping, // I am not sure what this would do
		RttHistgram, // this neither
		QueueStats,
		QueueXStats,
		ClassStats, // cake has 4096 classes also
		TraceRoute,
		CRASH,
		BENCH,
		MTR,
		TCPDUMP, 
		NTEST, // run some sort of test
		MAX_CMDS
}

enum filters {
		ASC = BIT(0), // sort order
		// Filters
		IPV4 = BIT(1),
		IPV6 = BIT(2),
		LL = BIT(3),
		BEST = BIT(4),
		UP = BIT(5),
		MYNETWORK = BIT(6), // this is the defined range of my entire arin ip space
		UNKNOWNIPS = BIT(7), 
		INETIPS = BIT(8), 
		MappedIPs = BIT(9),
		MPLS = BIT(10),
		ROUTING = BIT(11),
		PLAN = BIT(12), // over? under?
		RFC1918 = BIT(13),
		IEXP = BIT(14), // 240/4 
		IZERO = BIT(15), // 0/8
	        TEST = BIT(16), // Test networks per RFC
		MCAST = BIT(17), // 224/4
		ASN = BIT(18), // think about this as 
		MAC = BIT(19), 
		DSCP = BIT(20), 
		ECN = BIT(21), 
		CUSTOMER = BIT(22), // circuit-id?
		// AP0-AP7 We presently have a max circuit depth of 8
}; // Enums are only 32 bits on 64 bit architecure, so this ends being defines in C.

tins {
	u64 tins; // one bit for each possible tin
};

dscp {
	u64 dscp; // one bit for each possible tin
};


struct mpoe_msg {
		u32 cmd,
		u32 tag, // a tag increments by request and is unique per connection
		u64 filt,
		u64 start,
		u64 size,
}

struct mpoe_resp {
		u32 cmd,
		u32 tag, 
		u64 filt,
		u64 start,
		u64 size,
}

// Filt to ebpf compiler

#define user_security_filters
#define admin_security_filters

#define ALL_TINS MAX_U64
#define ALL_PROTOS (MCAST | LL | IPV6 | IPV6 | EXP | RFC1918 | MPLS | ROUTING | MAC )
#define FILT_DEFAULT (ALL_TINS | ALL_PROTOS)
#define ODD besomethingelse

#endif // MQOE_BUS_H_
