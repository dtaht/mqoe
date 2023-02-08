// FIXME somewhere defines for this already exist
// https://en.wikipedia.org/wiki/Reserved_IP_addresses

typedef struct {
		int type;
		char *ip;
} reserved_ip;

enum { current_network, RFC1918, };

const reserved_ip *reserved_ips = {
		{	current_network, "0.0.0.0/8", },
		{	RFC1918, "10.0.0.0/8", },
};

int ip_to_binary() { }
int ip_from_binary() { }
