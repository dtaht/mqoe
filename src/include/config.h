#ifndef MPOE_CONFIG
#define MPOE_CONFIG
#endif

typedef u64 ns;

struct perms {
		u32 max_users;
		u32 umask;
		char group[255]; // FIXME max group size
};

struct stats {
		ns queue_check_period; // ns, not us
}

enum offloads { stop_irq_balance, disable_rxvlan, disable_txvlan = BIT(1), gso, tso, gro, sg, lro };

struct tuning {
		ns queue_check_period;
		ns rx;
		ns tx;
		ns netdev_budget;
		offloads offloads;
		u32 netdev_budget_packets;
};

struct bridge {
		bool use_xdp_bridge;
		bool scan_vlans;
		char in[IFACE_LEN];
		char out[IFACE_LEN];
		u16 tag_in;
		u16 tag_out;
};

struct ispconfig {
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
		u16 queuesAvailableOverride;
};

struct config {
		ispconfig i;
		bridge b;
		tuning t;
};

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

struct interface_map {
		char *name;
		redirect_to *name;
		bool scan_vlans;
};

/*
// [maps]

char interface_a[20];
char interface_b[20];
u32 vlanA;
u32 vlanB;

generatedPNDownloadMbps = 1000
generatedPNUploadMbps = 1000

# If a device shows a WAN IP within these subnets...
# assume they are behind NAT / un-shapable, and ignore them

ignoreSubnets = ['192.168.0.0/16']
allowedSubnets = ['100.64.0.0/10']

upstreamBandwidthCapacityDownloadMbps = 1000
upstreamBandwidthCapacityUploadMbps = 1000

*/
