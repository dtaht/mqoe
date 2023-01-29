#include "mqoe.h"
#include "main.h"

// Toml, json, and csv, oh my

// Constants
#define NSEC_PER_SEC (1000000000.0)
#define BUFFERSIZE (1024*1024)

const char *jsontemplate = "{\"name\":\"Mash\",\"star\":4,\"hits\":[2,2,1,3]}";
const char *toml = "";

struct arg {
	i32 count;
	struct timespec interval;
	double finterval;
	char *interface;
	char *command;
	char buffer;
};

typedef struct arg args;

static const struct option long_options[] = {
	{ "interface", required_argument	, NULL , 'i' } ,
	{ "count"    , required_argument	, NULL , 'c' } ,
	{ "interval" , required_argument	, NULL , 'I' } ,
	{ "command"  , required_argument	, NULL , 'C' } ,
	{ "help"     , no_argument		, NULL , 'h' } ,
	{ "buffer"   , no_argument		, NULL , 'b' } ,
};

void usage (char *err) {
	if(err) fprintf(stderr,"%s\n",err);
	printf("mqoe [options]\n");
	printf(
		"\t-h --help \n"
		"\t-b --buffer \n"
		"\t-i --interface [eth0*,wlan0,etc]\n"
		"\t-c --count     [number of iterations]\n"
		"\t-I --interval  [fractional number of seconds]\n"
		"\t-C --command   [qdisc]\n");
	exit(-1);
}

static void defaults(args *a) {
	a->interface = "eth0";
	a->command = "qdisc";
	a->finterval=.2;
	a->count=10;
	a->interval.tv_nsec = 0;
	a->interval.tv_sec = 0;
	a->buffer = 0;
}

#define QSTRING "i:c:I:C:hb"

int process_options(i32 argc, char **argv, args *o)
{
	i32          option_index = 0;
	i32          opt = 0;
	optind       = 1;

	while(1)
	{
		opt = getopt_long(argc, argv,
				  QSTRING,
				  long_options, &option_index);
		if(opt == -1) break;

		switch (opt)
		{
		case 'c': o->count = strtoul(optarg,NULL,10);  break;
		case 'I': o->finterval = strtod(optarg,NULL); break;
		case 'C': o->command = optarg; break;
		case 'i': o->interface = optarg; break;
		case 'b': o->buffer = 1; break;
		case '?':
		case 'h': usage(NULL); break;
		default:  usage(NULL);
		}
	}
	o->interval.tv_sec = floor(o->finterval);
	o->interval.tv_nsec = (u64) ((o->finterval - o->interval.tv_sec) * NSEC_PER_SEC);
	return 0;
}

// Since this is linux only we can use timerfd for an isochronous clock

int forkit(args *a)
{
	i32 filedes[2]; // 0 = read
	i32 filedes2[2]; // 0 = read
	pipe(filedes);
	pipe(filedes2);
	i32 tool = filedes[1]; // write this
	i32 in = filedes2[0]; // connect out to in
	char tmpfile[] = "/tmp/tc_iterateXXXXXX";
	i32 out = a->buffer ? mkstemp(tmpfile) : STDOUT_FILENO;
	pid_t child;

	if(a->buffer && !out) {
		perror("Unable to create tmpfile");
		exit(-1);
	}
	// probably want the pipe line buffered via fcntl
	if((child = fork())==0)
	{
		close(filedes[1]);
		close(filedes2[0]);
		dup2(filedes2[1],STDOUT_FILENO);
		dup2(filedes[0],STDIN_FILENO);

		if(execlp("tc", "tc", "-s", "-b", "-",NULL)==-1) {
			perror("Failed to execute cmd");
			exit(-1);
		}
	}
	close(filedes2[1]);
	close(filedes[0]);
	struct itimerspec new_value = {0};

	i32 timer = timerfd_create(CLOCK_REALTIME, 0);
	new_value.it_interval = a->interval;
	new_value.it_value = a->interval;

	/* better method would be to poll for input (since writes from the
	   tool could block or return no output for some reason), timestamp the input,
	   and if the difference is less than half, skip this round.
	   this would absorb non-completing stuff */

	char buffer[BUFFERSIZE];
	char cmd[1024];
	i32 size = 0;
	sprintf(cmd,"%s show dev %s\n",a->command,a->interface);
	i32 csize = strlen(cmd);
	i32 ctr = 0;
	timerfd_settime(timer,0,&new_value,NULL); // relative timer

	do {
		u64 fired;
		if(write(tool,cmd,csize)==-1) perror("writing cmd");
		if(read(timer,&fired,sizeof(fired))!=8) perror("reading timer");
		ctr+=fired;
		if((size = read(in,buffer,sizeof(buffer))) > 0) {
			result(out,size,BUFFERSIZE,buffer);
		} else {
			result(out,0,BUFFERSIZE,buffer);
			perror("reading cmd output");
		}
	} while (ctr < a->count);
	close(tool);
	close(in);
	if(a->buffer) {
		lseek(out, 0, SEEK_SET);
		while((size = read(out, buffer, sizeof(buffer))) > 0)
			write(STDOUT_FILENO,buffer,size);
		unlink(tmpfile);
	}
	close(out);
	wait(NULL);
	return 0;
}


// Functions

bool parse_namespaces() { return true; }
bool block_signals() { return true; }
bool create_threads() { return true; }

i32 run() {
		return 0;
}

bool parse_config(char *file) {
}

i32 main(int c, char** argv) {
		args a;
		int status = 0;
		defaults(&a);
		process_options(argc, argv, &a);
		parse_config(MQOE_CONFIG);
		create_namespaces();
		block_signals();
		create_threads();
		return exit(run());
}
