#include "include/mqoe.h"

static const struct option mqval_options[] = {
	{ "validate", required_argument	, NULL , 'i' } ,
	{ "count"    , required_argument	, NULL , 'c' } ,
	{ "reload" , required_argument	, NULL , 'r' } ,
	{ "shutdown"  , required_argument	, NULL , 's' } ,
	{ "dry-run"  , required_argument	, NULL , ' ' } ,
	{ "help"     , no_argument		, NULL , 'h' } ,
	{ "buffer"   , no_argument		, NULL , 'b' } ,
};

void mqval_usage (char *err) {
	if(err) fprintf(stderr,"%s\n",err);
	printf("mqval [options]\n");
	printf(
		"\t-h --help \n"
		"\t-b --buffer \n"
		"\t-i --interface [eth0*,wlan0,etc]\n"
		"\t-c --count     [number of iterations]\n"
		"\t-I --interval  [fractional number of seconds]\n"
		"\t-C --command   [qdisc]\n");
	exit(-1);
}

int main_mqval(int argc, char*argv) {
		mqval_usage("have to write this");
		return 0;
}

#ifndef BUSYBOX
int main(int argc, char *argv) {
		return main_mqval(argc,argv);
}
#endif
