#pragma once

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
} mq_stats;

enum offloads { stop_irq_balance, disable_rxvlan, disable_txvlan = BIT(1), gso, tso, gro, sg, lro };

typedef struct {
	char *name;
	redirect_to *name;
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
	char in[IFACE_LEN];
	char out[IFACE_LEN];
	u16 tag_in;
	u16 tag_out;
} bridge;

typedef struct {
	bool influxDBEnabled;
	char influxDBurl[1024]
	char influxDBBucketi[255];
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
	ipv4_trie *ignore4Subnets[]; // tempted to make this static
	ipv6_trie *ignore6Subnets[];
	ipv4_trie *allowed4Subnets[];
	ipv6_trie *allowed6Subnets[];
	ipv4_trie *my4Subnets[];
	ipv6_trie *my6Subnets[];
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

struct stuff {
	char *sqm;
	char *lqos_bus;
	char *user;
	char *group;
	char *auth;
	u16 umask;
	char *lqos_directory;
	u16 lqos_port;
};


