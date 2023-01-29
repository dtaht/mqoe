#include "util2.h"

// Heh. Thread safety in C, not... I am not sure if I will do it this
// way... NUM_CALLS?

#define THREAD_WASTE 16
#define GBT (1000000000)
#define MBT (1000000)
#define KBT (1000)

const char *
format_thousands(unsigned int value)
{
    static __thread char buf[THREAD_WASTE][15];
    static __thread int i = 0;
    i = (i + 1) % THREAD_WASTE;
    snprintf(buf[i], 15, "%u.%.3u", value / KBT, value % KBT);
    return buf[i];
}

const char * scale_bits(u64 n) {
		static __thread char buf[THREAD_WASTE][20];
		static __thread int i = 0;
		if (n > GBT) {
				snprintf(buf[i], 20, "%ul.%.2ul gbit/s", n / GBT, n % GBT);
		} else if ( n > MBT ) {
				snprintf(buf[i], 20, "%ul.%.2ul mbit/s", n / MBT, n % MBT);
		} else if ( n > KBT ) {
				snprintf(buf[i], 20, "%ul.%.2ul kbit/s", n / KBT, n % KBT);
		} else {
				snprintf(buf[i], 20, "%ul.%.2ul  bit/s", n, n);
		}
		return(buf[i]);
}

const char * scale_packets (u64 n) {
		static __thread char buf[THREAD_WASTE][20];
		static __thread int i = 0;
		i = (i + 1) % THREAD_WASTE;
		if (n > GBT) {
				snprintf(buf[i], 20, "%ul.%.2ul gpps/s", n / GBT, n % GBT);
		} else if ( n > MBT ) {
				snprintf(buf[i], 20, "%ul.%.2ul mpps/s", n / MBT, n % MBT);
		} else if ( n > KBT ) {
				snprintf(buf[i], 20, "%ul.%.2ul kpps/s", n / KBT, n % KBT);
		} else {
				snprintf(buf[i], 20, "%ul.%.2ul  pps/s", n, n);
		}
		return(buf[i]);
}

/* Compound literal
 * structure = ((struct foo) {x + y, 'a', 0});
This is equivalent to writing the following:

{
  struct foo temp = {x + y, 'a', 0};
  structure = temp;
}
 *
https://paulmck.livejournal.com/64209.html
*
* __ rcu
*
*/

// https://github.com/goldsborough/ipc-bench
// Memory mapped files are 20x faster than messages


threaded_reader() {
		while(fread(fdtimer,timeout)) {

				rcu_bla(); // critical section
				rcu_synchronize();
				write(data);

		}
}

static
void *thr_reader(void *_count)
{
	unsigned long long *count = _count;
	struct test_array *local_ptr;

	printf_verbose("thread_begin %s, tid %lu\n",
			"reader", urcu_get_thread_id());

	set_affinity();

	rcu_register_thread();

	while (!test_go)
	{
	}
	cmm_smp_mb();

	for (;;) {
		rcu_read_lock();
		local_ptr = rcu_dereference(test_rcu_pointer);
		rcu_debug_yield_read();
		if (local_ptr)
			urcu_posix_assert(local_ptr->a == 8);
		if (caa_unlikely(rduration))
			loop_sleep(rduration);
		rcu_read_unlock();
		URCU_TLS(nr_reads)++;
		if (caa_unlikely(!test_duration_read()))
			break;
	}

	rcu_unregister_thread();

	*count = URCU_TLS(nr_reads);
	printf_verbose("thread_end %s, tid %lu\n",
			"reader", urcu_get_thread_id());
	return ((void*)1);

}

static
void *thr_writer(void *_count)
{
	unsigned long long *count = _count;
	struct test_array *new, *old;

	printf_verbose("thread_begin %s, tid %lu\n",
			"writer", urcu_get_thread_id());

	set_affinity();

	while (!test_go)
	{
	}
	cmm_smp_mb();

	for (;;) {
		rcu_copy_mutex_lock();
		new = test_array_alloc();
		new->a = 8;
		old = test_rcu_pointer;
		rcu_assign_pointer(test_rcu_pointer, new);
		if (caa_unlikely(wduration))
			loop_sleep(wduration);
		synchronize_rcu();
		if (old)
			old->a = 0;
		test_array_free(old);
		rcu_copy_mutex_unlock();
		URCU_TLS(nr_writes)++;
		if (caa_unlikely(!test_duration_write()))
			break;
		if (caa_unlikely(wdelay))
			loop_sleep(wdelay);
	}

	printf_verbose("thread_end %s, tid %lu\n",
			"writer", urcu_get_thread_id());
	*count = URCU_TLS(nr_writes);
	return ((void*)2);
}
