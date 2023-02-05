#ifndef MPOE_CONFIG
#define MPOE_CONFIG
#endif

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

queue_check_period_ms = 1000

struct interface_map {
		char *name;
		redirect_to *name;
		bool scan_vlans;
};

enum offloads (nop,gso,lro,sg,gro,tso,rxvlan);

struct tuning {
		u32 netdev_budget_usecs;
		u32 netdev_budget_packets;
		u32 rx_usecs;
		u32	tx_usecs;
		bool stop_irq_balance;
		bool disable_rxvlan;
		bool disable_txvlan;
		disable_offload = [ "gso", "tso", "lro", "sg", "gro" ]
};

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

struct modes {
		bool monitor_only;
		bool enable_shell;
		bool enable_sudo;
		bool on_a_stick;
		u32 queuesOverride;
		u32 binPackingMode;
};

upstreamBandwidthCapacityDownloadMbps = 1000
upstreamBandwidthCapacityUploadMbps = 1000

typedef struct mpoe_config {
};
