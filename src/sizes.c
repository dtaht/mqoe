#include "include/mqoe.h"
#include "include/config.h"

#define PS(val) { printf("sizeof " # val " = %d\n", sizeof(val)); }

int main(int argc, char *argv) {
	PS(trie_ipv6);
	PS(trie_ipv4);
	PS(perms);
	PS(mqstats);
	PS(interface_map);
	PS(tuning);
	PS(bridge);
	PS(influxdb);
	PS(ispconfig);
	PS(config);
	PS(stuff);
}
