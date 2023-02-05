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

int main(int argc, char *argv) {
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
