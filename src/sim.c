#include "mqoe.h"
#include <csv.h>

/* flent duty cycles crusader
 * curl
 * netflix */

/* output a network.json
 * generate dns names according to this scheme
 * output a ShapedDevices.csv
 * output a netns scheme
 * establish test servers running on each server
 * output babel or static routes
 * generate test script
 * generate teardown commands
 */

static const struct option sim_options[] = {
	{ "validate", required_argument	, NULL , 'i' } ,
	{ "connect"    , required_argument	, NULL , 'c' } ,
	{ "reload" , required_argument	, NULL , 'r' } ,
	{ "shutdown"  , required_argument	, NULL , 's' } ,
	{ "dry-run"  , required_argument	, NULL , ' ' } ,
	{ "help"     , no_argument		, NULL , 'h' } ,
	{ "depth", required_argument	, NULL , 'd' } ,
	{ "delays", required_argument	, NULL , 'T' } ,
	{ "loss", required_argument	, NULL , 'L' } ,
	{ "small"     , no_argument		, NULL , 'S' } ,
	{ "medium"     , no_argument		, NULL , 'M' } ,
	{ "large"     , no_argument		, NULL , 'L' } ,
	{ "huge"     , no_argument		, NULL , 'H' } ,
	{ "machines", required_argument	, NULL , 'm' } ,
	{ "up"     , no_argument		, NULL , 'u' } ,
	{ "down"     , no_argument		, NULL , 'S' } ,
	{ "teardown"     , no_argument		, NULL , 'S' } ,
	{ "restart"     , no_argument		, NULL , 'R' } ,
	{ "reload"     , no_argument		, NULL , 'R' } ,
	{ "buffer"   , no_argument		, NULL , 'b' } ,
};

void sim_usage (char *err) {
	if(err) fprintf(stderr,"%s\n",err);
	printf("sim [options]\n");
	printf(
		"\t-h --help \n"
		"\t-b --buffer \n"
		"\t-C --command   [qdisc]\n");
	exit(-1);
}

#define GETOPT ("")

int main_sim(int argc, char **argv) {
		sim_usage("have to write this");
		return 0;
}

#ifndef BUSYBOX
int main(int argc, char **argv) {
		return main_mqval(argc,argv);
}
#endif

// put the generated names in dns

nsupdate(char *name) {

}

gensim() {
	for(i32 p = 0; p < sizeof(plans); p++) {
        for(i32 d = 0; d < sizeof(delays); d++) {
            for(i32 ip = 0; ip < sizeof(ips); ip++) {
                for(i32 dev = 0; dev < sizeof(devs); dev++) {
				}
            }
        }
    }
}


typedef enum {
       circuit_id, circuit_name, device_id, device_name,
       parent_node, mac, ipv4, ipv6,
       download_min, download_max, download_min2, download_max2,
       comment, SHAPED_MAX,
} shaped;

char *shaped_devices[SHAPED_MAX];

