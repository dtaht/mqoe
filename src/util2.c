#include "mqoe.h"

// Heh. Thread safety in C, not... I am not sure if I will do it this
// way... NUM_CALLS?

#define THREAD_WASTE 16
#define GBT (1000000000)
#define MBT (1000000)
#define KBT (1000)

// These functions are lightly warmed over from the babeld sources
// Which are mit licensed. And made more threadsafe.
// It hurts me to do this. This is an xor(u128 a, u128 a)

const unsigned char zeroes[16] = {};

const unsigned char v4prefix[16] =
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0, 0, 0 };

static const unsigned char llprefix[16] =
    {0xFE, 0x80};

// FIXME - add ipv4 link local especially

const char *
format_thousands(unsigned int value)
{
    static __thread char buf[THREAD_WASTE][15];
    static __thread int i = 0;
    i = (i + 1) % THREAD_WASTE;
    snprintf(buf[i], 15, "%u.%.3u", value / KBT, value % KBT);
    return buf[i];
}

const char * scale_bits(u64 n) {
		static __thread char buf[THREAD_WASTE][20];
		static __thread int i = 0;
		if (n > GBT) {
				snprintf(buf[i], 20, "%ul.%.2ul gbit/s", n / GBT, n % GBT);
		} else if ( n > MBT ) {
				snprintf(buf[i], 20, "%ul.%.2ul mbit/s", n / MBT, n % MBT);
		} else if ( n > KBT ) {
				snprintf(buf[i], 20, "%ul.%.2ul kbit/s", n / KBT, n % KBT);
		} else {
				snprintf(buf[i], 20, "%ul.%.2ul  bit/s", n, n);
		}
		return(buf[i]);
}

const char * scale_packets (u64 n) {
		static __thread char buf[THREAD_WASTE][20];
		static __thread int i = 0;
		i = (i + 1) % THREAD_WASTE;
		if (n > GBT) {
				snprintf(buf[i], 20, "%ul.%.2ul gpps/s", n / GBT, n % GBT);
		} else if ( n > MBT ) {
				snprintf(buf[i], 20, "%ul.%.2ul mpps/s", n / MBT, n % MBT);
		} else if ( n > KBT ) {
				snprintf(buf[i], 20, "%ul.%.2ul kpps/s", n / KBT, n % KBT);
		} else {
				snprintf(buf[i], 20, "%ul.%.2ul  pps/s", n, n);
		}
		return(buf[i]);
}

static int
h2i(char c)
{
    if(c >= '0' && c <= '9')
        return c - '0';
    else if(c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    else if(c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    else
        return -1;
}

int
fromhex(unsigned char *dst, const char *src, int n)
{
    int i;
    if(n % 2 != 0)
        return -1;
    for(i = 0; i < n/2; i++) {
        int a, b;
        a = h2i(src[i*2]);
        if(a < 0)
            return -1;
        b = h2i(src[i*2 + 1]);
        if(b < 0)
            return -1;
        dst[i] = a*16 + b;
    }
    return n/2;
}

int
parse_address(const char *address, unsigned char *addr_r, int *af_r)
{
    struct in_addr ina;
    struct in6_addr ina6;
    int rc;

    rc = inet_pton(AF_INET, address, &ina);
    if(rc > 0) {
        memcpy(addr_r, v4prefix, 12);
        memcpy(addr_r + 12, &ina, 4);
        if(af_r) *af_r = AF_INET;
        return 0;
    }

    rc = inet_pton(AF_INET6, address, &ina6);
    if(rc > 0) {
        memcpy(addr_r, &ina6, 16);
        if(af_r) *af_r = AF_INET6;
        return 0;
    }

    return -1;
}

unsigned char *
normalize_prefix(unsigned char *restrict ret,
                 const unsigned char *restrict prefix, unsigned char plen)
{
    if(plen >= 128) {
        memcpy(ret, prefix, 16);
        return ret;
    }

    memset(ret, 0, 16);
    memcpy(ret, prefix, plen / 8);
    if(plen % 8 != 0)
        ret[plen / 8] =
            (prefix[plen / 8] & ((0xFF << (8 - (plen % 8))) & 0xFF));
    return ret;
}

int
linklocal(const unsigned char *address)
{
    return memcmp(address, llprefix, 8) == 0;
}

int
v4mapped(const unsigned char *address)
{
    return memcmp(address, v4prefix, 12) == 0;
}

void
v4tov6(unsigned char *dst, const unsigned char *src)
{
    memcpy(dst, v4prefix, 12);
    memcpy(dst + 12, src, 4);
}


const char *
format_address(const unsigned char *address)
{
    static __thread char buf[THREAD_WASTE][INET6_ADDRSTRLEN];
    static __thread int i = 0;
    i = (i + 1) % THREAD_WASTE;
    if(v4mapped(address))
        inet_ntop(AF_INET, address + 12, buf[i], INET6_ADDRSTRLEN);
    else
        inet_ntop(AF_INET6, address, buf[i], INET6_ADDRSTRLEN);
    return buf[i];
}

const char *
format_prefix(const unsigned char *prefix, unsigned char plen)
{
    static __thread char buf[THREAD_WASTE][INET6_ADDRSTRLEN + 4];
    static __thread int i = 0;
    int n;
    i = (i + 1) % THREAD_WASTE;
    if(plen >= 96 && v4mapped(prefix)) {
        inet_ntop(AF_INET, prefix + 12, buf[i], INET6_ADDRSTRLEN);
        n = strlen(buf[i]);
        snprintf(buf[i] + n, INET6_ADDRSTRLEN + 4 - n, "/%d", plen - 96);
    } else {
        inet_ntop(AF_INET6, prefix, buf[i], INET6_ADDRSTRLEN);
        n = strlen(buf[i]);
        snprintf(buf[i] + n, INET6_ADDRSTRLEN + 4 - n, "/%d", plen);
    }
    return buf[i];
}

const char *
format_eui64(const unsigned char *eui)
{
    static __thread char buf[THREAD_WASTE][28];
    static __thread int i = 0;
    i = (i + 1) % THREAD_WASTE;
    snprintf(buf[i], 28, "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",
             eui[0], eui[1], eui[2], eui[3],
             eui[4], eui[5], eui[6], eui[7]);
    return buf[i];
}

int
parse_net(const char *net, unsigned char *prefix_r, unsigned char *plen_r,
          int *af_r)
{
    char buf[INET6_ADDRSTRLEN];
    char *slash, *end;
    unsigned char prefix[16];
    long plen;
    int af;
    struct in_addr ina;
    struct in6_addr ina6;
    int rc;

    if(strcmp(net, "default") == 0) {
        memset(prefix, 0, 16);
        plen = 0;
        af = AF_INET6;
    } else {
        slash = strchr(net, '/');
        if(slash == NULL) {
            rc = parse_address(net, prefix, &af);
            if(rc < 0)
                return rc;
            plen = 128;
        } else {
            if(slash - net >= INET6_ADDRSTRLEN)
                return -1;
            memcpy(buf, net, slash - net);
            buf[slash - net] = '\0';
            rc = inet_pton(AF_INET, buf, &ina);
            if(rc > 0) {
                memcpy(prefix, v4prefix, 12);
                memcpy(prefix + 12, &ina, 4);
                plen = strtol(slash + 1, &end, 0);
                if(*end != '\0' || plen < 0 || plen > 32)
                    return -1;
                plen += 96;
                af = AF_INET;
            } else {
                rc = inet_pton(AF_INET6, buf, &ina6);
                if(rc > 0) {
                    memcpy(prefix, &ina6, 16);
                    plen = strtol(slash + 1, &end, 0);
                    if(*end != '\0' || plen < 0 || plen > 128)
                        return -1;
                    af = AF_INET6;
                } else {
                    return -1;
                }
            }
        }
    }
    normalize_prefix(prefix_r, prefix, plen);
    *plen_r = plen;
    if(af_r) *af_r = af;
    return 0;
}

int
martian_prefix(const unsigned char *prefix, int plen)
{
    static const unsigned char ones[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    return
        (plen >= 8 && prefix[0] == 0xFF) ||
        (plen >= 10 && prefix[0] == 0xFE && (prefix[1] & 0xC0) == 0x80) ||
        (plen >= 128 && memcmp(prefix, zeroes, 15) == 0 &&
         (prefix[15] == 0 || prefix[15] == 1)) ||
        (plen >= 96 && v4mapped(prefix) &&
         ((plen >= 104 && (prefix[12] == 127 || prefix[12] == 0)) ||
          (plen >= 100 && (prefix[12] & 0xF0) == 0xE0) ||
          (plen >= 128 && memcmp(prefix + 12, ones, 4) == 0)));
}

int
daemonise()
{
    int rc;

    fflush(stdout);
    fflush(stderr);

    rc = fork();
    if(rc < 0)
        return -1;

    if(rc > 0)
        exit(0);

    rc = setsid();
    if(rc < 0)
        return -1;

    return 1;
}

