// Generate a simulation clock
#include "include/mqoe.h"

u64 *create_simclock(char *filepath) {
  int fd;
  struct stat statbuf;
  void *ptr;
  if ((fd = open(filepath, O_RDWR | O_CREAT)) < 0)
    errExit("%s Could not open\n", filepath);
  if ((fstat(fd, &statbuf)) < 0)
    errExit("%s could not stat\n", filepath);
  if ((ptr = mmap(NULL, sizeof(u64), PROT_READ, MAP_SHARED, fd, 0)) ==
      MAP_FAILED)
    errExit1("CSV file Mapping Failed\n");
  close(fd);
  return (u64 *)ptr;
}

// ADD FAKECLOCK

u64 *open_simclock(char *filepath) {
  int fd;
  struct stat statbuf;
  void *ptr;
  if ((fd = open(filepath, O_READ)) < 0)
    errExit("%s Could not open\n", filepath);
  if ((fstat(fd, &statbuf)) < 0)
    errExit("%s could not stat\n", filepath);
  if ((ptr = mmap(NULL, sizeof(u64), PROT_READ, MAP_SHARED, fd, 0)) ==
      MAP_FAILED)
    errExit1("CSV file Mapping Failed\n");
  close(fd);
  return (u64 *)ptr;
}

void start_clock() {}

const float finterval = .2;

test_clock() {
  int ctr = 1000;
  volatile u64 *clock;
  struct itimerspec new_value = {0};
  clock = create_simclock("/var/run/clock");
  int timer = timerfd_create(CLOCK_REALTIME, 0);
  interval.tv_sec = floor(o->finterval);
  interval.tv_nsec =
      (long long)((o->finterval - o->interval.tv_sec) * NSEC_PER_SEC);
  new_value.it_interval = a->interval;
  new_value.it_value = a->interval;

  do {
    u64 fired;
    //		if(write(tool,cmd,csize)==-1) perror("writing cmd");
    if (read(timer, &fired, sizeof(fired)) != 8)
      perror("reading timer");
    ctr += fired;
    // FIXME write to clock in ns
  } while (ctr < a->count);
}

read_thread() {
  volatile u64 *clock;
  struct itimerspec new_value = {0};
  u64 *clock = open_simclock("/var/run/clock");
  // usleep
}

int main(int argc, char *argv[]) {}
