#include <getopt.h>
#include <linux/tcp_metrics.h>
#include <linux/types.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *mqoe_options = "c:d:a:b:c";

static const struct option mqoe_long_options[] = {
    {"connect", required_argument, 0, 'c'},
    {"validate", no_argument, 0, 'V'},
    {"dry-run", no_argument, 0, 0},
    {"debug", no_argument, 0, 'd'},
    {"reload", required_argument, 0, 'r'},
    {"restart", no_argument, 0, 'R'},
    {"shutdown", required_argument, 0, 'S'},
    {"status", required_argument, 0, 's'},
    {"file", required_argument, 0, 0},
    {0, 0, 0, 0}};

static const struct option mqtop_long_options[] = {
    {"connect", required_argument, 0, 'c'},
    {"validate", no_argument, 0, 'V'},
    {"reload", required_argument, 0, 'r'},
    {"restart", no_argument, 0, 'R'},
    {"debug", no_argument, 0, 'd'},
    {"reload", required_argument, 0, 'r'},
    {"shutdown", required_argument, 0, 'S'},
    {"status", required_argument, 0, 's'},
    {"file", required_argument, 0, 0},
    {0, 0, 0, 0}};

static const struct option mqpg_long_options[] = {
    {"connect", required_argument, 0, 'c'},
    {"validate", no_argument, 0, 'V'},
    {"reload", required_argument, 0, 'r'},
    {"restart", no_argument, 0, 'R'},
    {"shutdown", required_argument, 0, 'S'},
    {"debug", no_argument, 0, 'd'},
    {"reload", required_argument, 0, 'r'},
    {"status", required_argument, 0, 's'},
    {"file", required_argument, 0, 0},
    {0, 0, 0, 0}};

/* This are binary options */
enum filter_cmds {
  NOP,
  ACK,
  PING,
  V4,
  V6,
  LL,
  ASC,
  BANDWIDTH,
  SHAPED,
  MYNETWORK,
  DROPS,
  ACK_DROPS,
  MARKS,
  TIN0,
  TIN1,
  TIN2,
  TIN3,
  FILTER_MAX
};

// #include "mqoe.h"
// FIXME - WARN_ONCE

// FIXME These are bits

enum ip_options { NOP, ll, mc, ula, lo, test, exp };

// ip_options = none, ll, mc, ula, lo, test, exp
// fixme

typedef __u8 u8;
typedef __u32 u32;
typedef __uint128_t u128;

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
  char sqm[255];
  trie_ipv4 **my4;
  trie_ipv6 **my6;
  char *location;
  char *bus;
} config;

bool usage_mqoe(int c, char **argv) {
  printf("mpqoe options!\n");
  exit(-1);
}

bool usage_mqoed(int c, char **argv) {
  printf("mqoed options\n");
  exit(-1);
}

bool usage_mqtop(int c, char **argv) {
  printf("mqtop options!\n");
  exit(-1);
}

bool usage(int c, char **argv) {
  printf("%s: [mqtop,mqoe,mqoed]\n", argv[0]);
  exit(-1);
}

/* init, config, signals, threads, go */

void main(int c, char **argv) {
  strcmp(argv[0], "mqtop") == 0   ? usage_mqtop(c, argv)
  : strcmp(argv[0], "mqoed") == 0 ? usage_mqoed(c, argv)
  : strcmp(argv[0], "mqoe") == 0  ? usage_mqoe(c, argv)
                                  : usage(c, argv);
}

/* close signal handlers
 * --dry-run
 * --connect
 * reload
 * balance
 * restart
 * shutdown
 * config */

/* */
