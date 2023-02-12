#include "include/mqoe.h"
#include "include/config.h"
#include <string.h>
#include "tomlc99/toml.h"

struct toml_table_t *mqoe_config;

static void error(const char* msg, const char* msg1)
{
    fprintf(stderr, "ERROR: %s%s\n", msg, msg1?msg1:"");
    exit(1);
}

// Hah. Fell apart on writing this macro last night need to check for overrun

#define SPUL(arg) { if (!(ptr = toml_table_in(ptr, #arg))) error("missing table section [" #arg "]",""); }
#define SCPY(arg) { toml_datum_t t = toml_string_in(ptr, #arg); \
		        if(t.ok && strlen(t.u.s) < sizeof(conf->s ## . ## arg)) \
			 { strncpy(ptr, t.u.s, sizeof(conf->s ## . ## arg)); free(t.u.s); \
		         } else { printf("string length exception"); } } 

#define ICPY(s,arg) { conf->s##.##arg; }
#define ACPY(s,arg) { conf->s##.##arg; }
#define TCPY(s,arg) { toml_datum_t t_in = toml_string_in(ptr, arg); \
    		      if (!t.ok) { error("cannot read server.host", ""); \
		    }}						   \

config * conf;

int main(int argc, char **argv)
{
FILE* fp;
conf = malloc(sizeof(config));
char errbuf[200];

    // 1. Read and parse toml file
    if(!(fp = fopen("/etc/lqos.conf", "r"))) {
    	if(!(fp = fopen("lqos.conf", "r"))) {
        	error("cannot open lqos.conf - ", strerror(errno));
		}
    }
    toml_table_t* conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);

    if (!conf) {
        error("cannot parse - ", errbuf);
    }

    // 2. Traverse to a table.
    	toml_table_t* ptr = NULL;
	SPUL(main);
  	SPUL(perms);
//	ICPYD(s,max_users,32);
//	ICPYD(s,umask,0770);
//	SCPYD(s,group,"lqos");
	SPUL(bridge);
	SPUL(lqos);
	SPUL(tuning);
	SPUL(stats)
	SPUL(influx);
//	BCPYD(s,enable,false);

    // 3. Extract values
    toml_datum_t host = toml_string_in(ptr, "host");
    if (!host.ok) {
        error("cannot read server.host", "");
    }

    toml_array_t* portarray = toml_array_in(ptr, "port");
    if (!portarray) {
        error("cannot read server.port", "");
    }

    printf("host: %s\n", host.u.s);
    printf("port: ");
    for (int i = 0; ; i++) {
        toml_datum_t port = toml_int_at(portarray, i);
        if (!port.ok) break;
        printf("%d ", (int)port.u.i);
    }
    printf("\n");

    // 4. Free memory
    free(host.u.s);
    toml_free(conf);
    return 0;
}

bool parse_config() {
	return false;
}
