#pragma once
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <getopt.h>
#include <linux/if.h>
#include <linux/tcp_metrics.h>
#include <linux/types.h>
#include <arpa/inet.h>
#include "version.h"

const char *LOCK_PATH = "/run/mqoe/lqosd.lock";
const char *LOCK_DIR = "/run/mqoe";
const char *LOCK_DIR_PERMS = "/run/mqoe";

#define CLOCKID CLOCK_REALTIME
#define errExit(msg, data)         do     {      printf(msg,data); \
        exit(EXIT_FAILURE);                             \
    } while (0)

#define errExit1(msg)         do     {      printf(msg); \
        exit(EXIT_FAILURE);                             \
    } while (0)


// FIXME - WARN_ONCE
// FIXME BIT(X) and enum macros
// FIXME linux defines these somewhere

typedef __u8 u8;
typedef __u16 u16;
typedef __u32 u32;
typedef __u64 u64;
typedef __uint128_t u128;
