#include "mqoe.h"
#include <csv.h>

plans = {
{ "6x1", 10, 3, 10, 3 },
{ "10x1", 10, 3, 10, 3 },
{ "25x3", 10, 3, 10, 3 },
{ "25x10", 10, 3, 10, 3 },
{ "25x25", 10, 3, 10, 3 },
{ "100x10", 10, 3, 10, 3 },
{ "100x20", 10, 3, 10, 3 },
{ "100x100", 10, 3, 10, 3 },
{ "200x20", 10, 3, 10, 3 },
{ "200x100", 10, 3, 10, 3 },
{ "500x100", 10, 3, 10, 3 },
{ "500x100", 10, 3, 10, 3 },
{ "1000x100", 10, 3, 10, 3 },
{ "1000x1000", 10, 3, 10, 3 },
{ "", 10, 3, 10, 3 },
};

// cut-through queueing

ips = {

};

devices = {

};

char *IPV4RANGE = hatever;
char *IPV6RANGE = hatever;

/* flent duty cycles crusader
 * curl
 * netflix */

/* --depth // depth of hieachy
 * --delays
 * --machines
 * --teardown
 * --up
 * --down
 */

/* output a network.json
 * generate dns names according to this scheme
 * output a ShapedDevices.csv
 * output a netns scheme
 * establish test servers running on each server
 * output babel or static routes
 * generate test script
 * generate teardown commands
 */

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

static struct counts {
    u32 fields;
    u32 rows;
    char *ptrs[SHAPED_MAX];
};

void validate(void *buf,u32 entries) {
    u32 c = 0;
    for(shaped s = 0; s < SHAPED_MAX; s++)
        printf()
}

// FIXME, don increment fields or rows on comments or corruptions
void cb1 (void *s, size_t len, void *data) {
    struct counts *c = ((struct counts *)data);
    if((char *)s[0] == '#') {
        return;
    }
    c->ptrs[c->fields++] = calloc(len + 1,1);
    strncpy(c->ptrs[c->fields++],s,len);
}

void cb2 (int c, void *data) {
    struct counts *s = ((struct counts *)data);
    s->fields = 0;
//    if (s->ptrs[download_min] is asci)
    if (false) {
        for(shaped fields = 0; fields < SHAPED_MAX; fields++)
            free(s->ptrs[fields]);
        return; // throw away malformed data (title)
    }
    s->ptrs = s->ptrs[++s->rows];
}

int main(int argc, char *argv[]){
    const char *filepath = argv[1];
    int fd;
    char *ptr;
    void *buf;
    struct stat statbuf;
    struct csv_parser p;
    struct counts c = {};
    size_t bytes_read;

    if (argc < 2) errExit1("Invalid path to csv file\n");
    if ((fd = open(filepath, O_RDONLY)) < 0) errExit("%s Could not open\n",filepath);
    if ((fstat(fd, &statbuf)) < 0) errExit("%s could not stat\n",filepath);
    if ((ptr = mmap(NULL,statbuf.st_size,PROT_READ,MAP_SHARED,fd,0)) == MAP_FAILED) errExit1("CSV file Mapping Failed\n");
    close(fd);

    if (csv_init(&p, 0) != 0) errExit1("CSV initialization failed");

    bytes_read = statbuf.st_size;
    buf = malloc(bytes_read * sizeof(struct ShapedDevices)); // It is impossible to overrun this but excessive
    count->ptr = buf;
    if (csv_parse(&p, ptr, bytes_read, cb1, cb2, &c) != bytes_read) errExit("Error while parsing file: %s\n", csv_strerror(csv_error(&p)) );
    csv_fini(&p, cb1, cb2, &c);
    buf = realloc(buf, c.rows * sizeof(struct ShapedDevices)); // But give all that memory back
    if((munmap(ptr, statbuf.st_size) != 0)) errExit1("Unmapping failed");
    printf("%lu fields, %lu rows\n", c.fields, c.rows);
    csv_free(&p);

    return 0;
}
