#include "include/mqoe.h"
#include <csv.h>

// enum ShapedFields(circuit_id);
#define SHAPED_FIELDS 13

struct ShapedDevices {
       char *circuit_id;
       char *circuit_name;
       char *device_id;
       char *device_name;
       char *parent_node;
       char mac[6];
       char *ipv4;
       char *ipv6;
       u32 download_min;
       u32 download_max;
       u32 download_min2;
       u32 download_max2;
       char *comment;
};

static size_t count;

static struct counts {
  long unsigned fields;
  long unsigned rows;
  char **ptrs[SHAPED_FIELDS];
};

/* void cb1 (void *s, size_t i, void *outfile) {
  csv_fwrite((FILE *)outfile, s, i);
  fputc(',',(FILE *)outfile);
}

void cb2 (int c, void *outfile) {
  fseek((FILE *)outfile, -1, SEEK_CUR);
  fputc('\n', (FILE *)outfile);
  } */

void cb1 (void *s, size_t len, void *data) {
    struct counts *c = ((struct counts *)data);
    u8 buf[len];
    strncpy(buf,s,len);
    if(buf[0] == '#') {
        return;
    }
    if(c->rows == 1) return; // throw away first line

    buf[len] = 0;
    switch(c->fields) {
        case 0:
            c->ptrs[c->fields] = calloc(len + 1,1);
            printf("%s ",buf);
            break;
        case 1:
            printf("%s ",buf);
            break;
        case 8:
        case 9:
        case 10:
        case 11:
            printf("%sMbit ",buf);
            break;
        case 12: printf("%s\n", buf);
    }
    ++c->fields;
}

void cb2 (int c, void *data) {
    struct counts *s = ((struct counts *)data);
    s->rows++;
    s->fields = 0;
//    struct shaper *s = (struct shaper *) data;
/*    strcpy(shaper->circuit_id,s[c++]);
    strcpy(shaped->circuit_name,s[c++]);
    strcpy(shaped->device_id,s[c++]);
    strcpy(shaped->device_name);
    char *shaped->parent_node;
    char shaped->mac[6];
    char shaped->*ipv4;
    char shaped->*ipv6;
    strlul(download_min);
    u32 download_max;
    u32 download_min2;
    u32 download_max2;
    char shaped->*comment; */
//    count++;
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
    if ((fd = open(filepath, O_RDONLY)) < 0) errExit("%s Ccould not open\n",filepath);
    if ((fstat(fd, &statbuf)) < 0) errExit("%s could not stat\n",filepath);
    if ((ptr = mmap(NULL,statbuf.st_size,PROT_READ,MAP_SHARED,fd,0)) == MAP_FAILED) errExit1("CSV file Mapping Failed\n");
    close(fd);

    if (csv_init(&p, 0) != 0) errExit1("CSV initialization failed");

    bytes_read = statbuf.st_size;
    buf = malloc(bytes_read * sizeof(struct ShapedDevices)); // It is impossible to overrun this but excessive
    // ptr!!
    if (csv_parse(&p, ptr, bytes_read, cb1, cb2, &c) != bytes_read) errExit("Error while parsing file: %s\n", csv_strerror(csv_error(&p)) );
    csv_fini(&p, cb1, cb2, &c);
    buf = realloc(buf, c.rows * sizeof(struct ShapedDevices)); // But give all that memory back
    if((munmap(ptr, statbuf.st_size) != 0)) errExit1("Unmapping failed");
    printf("%lu fields, %lu rows\n", c.fields, c.rows);

    csv_free(&p);

    return 0;
}
