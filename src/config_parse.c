#include "mqoe.h"
// #include "config_parse.h"

#include "tomlc99/toml.h"

typedef ns u32;

struct toml_table_t *mqoe_config;

struct influxdb {
		# Bandwidth & Latency Graphing
influxDBEnabled = True
influxDBurl = "http://localhost:8086"
influxDBBucket = "libreqos"
influxDBOrg = "Your ISP Name Here"
influxDBtoken = ""
}
/*

ignoreSubnets = ['192.168.0.0/16']
allowedSubnets = ['100.64.0.0/10']

# Stuff appearing on the bridge not on these networks is bad
# Spoofed traffic, non BCP38 issues from customers, etc also bad
# I am also not big on caseING variable names

mySubnets = ['x.y.z.x/22']
myTunnels = ['192.168.0.0/16'] # Say we use a subset of 10/8 or ...

*/


// https://www.mattmathis.net/
// Grump, no mmap

static void error(const char* msg, const char* msg1)
{
    fprintf(stderr, "ERROR: %s%s\n", msg, msg1?msg1:"");
    exit(1);
}

int main()
{
    FILE* fp;
    char errbuf[200];

    // 1. Read and parse toml file
    fp = fopen("sample.toml", "r");
    if (!fp) {
        error("cannot open sample.toml - ", strerror(errno));
    }

    toml_table_t* conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);

    if (!conf) {
        error("cannot parse - ", errbuf);
    }

    // 2. Traverse to a table.
    toml_table_t* server = toml_table_in(conf, "server");
    if (!server) {
        error("missing [server]", "");
    }

    // 3. Extract values
    toml_datum_t host = toml_string_in(server, "host");
    if (!host.ok) {
        error("cannot read server.host", "");
    }

    toml_array_t* portarray = toml_array_in(server, "port");
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

parse_config() {
    FILE *fp = fopen("test.toml");
    mqoe_config = toml_parse_file(FILE *fp, char *errbuf, int errbufsz);
}
    parse_config() {
    int fd = open(filepath, O_RDONLY);
}
