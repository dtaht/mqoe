#include "include/mqoe.h"
#include <linux/tcp_metrics.h>

static const char *mqoe_options = "c:d:a:b:c";

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

bool usage_mqoe(int c, char **argv) {
  printf("mpqoe options:\n");
  exit(-1);
}

bool usage(int c, char **argv) {
  printf("%s: [mqtop,mqoe,mqoed]\n", argv[0]);
  exit(-1);
}

/* init, config, signals, threads, go */

int main(int c, char **argv) {
  strcmp(argv[0], "mqtop") == 0   ? usage_mqtop(c, argv)
  : strcmp(argv[0], "mqoed") == 0 ? usage_mqoed(c, argv)
  : strcmp(argv[0], "mqoe") == 0  ? usage_mqoe(c, argv)
                                  : usage(c, argv);
}
