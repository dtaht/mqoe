#include <stdio.h>

const char *bus[] = {
	"NOP",
	"OK",
	"ACK",
	"NACK",
	"PING",
	"ISO",
	"FAIL",
	"ERROR",
	"INFO",
	"Reload",
	"Validate",
	"Shutdown",
	"Interval",
	"Alert",
	"BOOTTIME",
	"CURTIME",
	"CSINFO",
	"CPUINFO",
	"THREADINFO",
	"MEMINFO",
	"BALANCE",
	"HostCounters",
	"CurrentThroughput",
	"Downloads",
	"Uploads",
	"Rtt",
	"XdpPping",
	"RttHistgram",
	"QueueStats",
	"QueueXStats",
	"ClassStats",
	"TraceRoute",
	"MTR",
	"TCPDUMP",
	"NTEST",
	"MAX_CMDS",
	NULL
};

char *filters[] = {
	"ASC",
	"IPV4",
	"IPV6",
	"LL",
	"BEST",
	"UP",
	"MYNETWORK", // this is the defined range of my entire arin ip space
	"UNKNOWNIPS",
	"INETIPS",
	"MappedIPs",
	"MPLS",
	"ROUTING",
	"PLAN", // over? under?
	"RFC1918",
	"IEXP", // 240/4
	"IZERO", // 0/8
    "TEST", // Test networks per RFC
	"MCAST",
	"ASN",
	"MAC",
	"DSCP",
	"ECN",
	"CUSTOMER", // circuit-id?
}; // Enums are only 32 bits on 64 bit architecure, so this ends being defines in C.


int main(int argc, char **argv) {
		const char **p = bus;
		printf("enum a {\n");
		do {
				printf("%s,\n", *p);
		} while(*p++);
		printf("};\n");
		p = bus;
		int c = 0;
		printf("enum cmds {\n");
		do {
				printf("%s = BIT(%d),\n", *p, c++);
		} while(*p++);
		printf("};\n");

		return 0;
}
