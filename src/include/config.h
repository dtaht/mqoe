#pragma once

#define MAX_TRIE 16 // Arbitrary number of tries

typedef u64 ns;

typedef struct {
	u128 ip;
	u8 mask;
	u8 options;
} trie_ipv6;

typedef struct {
	u32 ip;
	u8 mask;
	u8 options;
} trie_ipv4;

typedef struct {
	u32 max_users;
	u32 umask;
	char group[255]; // FIXME max group size
} perms;

typedef struct {
	ns queue_check_period; // ns, not us
} mqstats;

// Probably a bad idea but I have to go look up how ethtool does it

typedef struct {
		u32 stop_irq_balance:1;
		u32 disable_rxvlan:1;
		u32 disable_txvlan:1;
		u32 gso:1;
		u32 tso:1;
		u32 gro:1;
		u32 sg:1;
		u32 lro:1;
} offloads;

typedef struct {
	char *name;
	char *redirect_to;
	bool scan_vlans;
} interface_map;

typedef struct {
	ns queue_check_period;
	ns rx;
	ns tx;
	ns netdev_budget;
	offloads offloads;
	u32 netdev_budget_packets;
} tuning;

typedef struct {
	bool use_xdp_bridge;
	bool scan_vlans;
	char in[IFNAMSIZ];
	char out[IFNAMSIZ];
	u16 tag_in;
	u16 tag_out;
} bridge;

typedef struct {
	bool influxDBEnabled;
	char influxDBurl[1024];
	char influxDBBucket[255];
	char influxDBOrg[255]; // "Your ISP Name Here"
	char influxDBtoken[255];
} influxdb;

typedef struct {
	char sqm_in[255];
	char sqm_out[255];
	bool monitor;
	bool noshell;
	bool sudo;
	bool binpack;
	u32 upstreamUpload;
	u32 upstreamDownload;
	u32 generatedPNDownloadMbps;
	u32 generatedPNup;
	u32 upstreamBandwidthCapacityDownloadMbps;
	u32 upstreamBandwidthCapacityUploadMbps;
	trie_ipv4 ignore4Subnets[MAX_TRIE];
	trie_ipv6 ignore6Subnets[MAX_TRIE];
	trie_ipv4 allowed4Subnets[MAX_TRIE];
	trie_ipv6 allowed6Subnets[MAX_TRIE];
	trie_ipv4 my4Subnets[MAX_TRIE];
	trie_ipv6 my6Subnets[MAX_TRIE];
//	myTunnels = ['192.168.0.0/16'] # Say we use a subset of 10/8 or ...
	u16 queuesAvailableOverride;
} ispconfig;

typedef struct {
	ispconfig i;
	bridge b;
	mqstats s;
	tuning t;
} config;

extern config *conf;

typedef struct {
	char *sqm;
	char *lqos_bus;
	char *user;
	char *group;
	char *auth;
	u16 umask;
	char *lqos_directory;
	u16 lqos_port;
} stuff;
