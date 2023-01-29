enum { nop, ack, } bus;
enum { nop, ping, } busRequest;

// 0 = true

struct {
		none,
		drops:1 = BIT(1),
		marks = 2,
		ack_drops = 4,
		asc = 8,
        ll = 16,
        v4 = 32,
        v6 = 64,
        local = 128,
		bandwidth,
		MAX_FILT,
		} filter;

// sort by bandwidth ascending, ignore link-local, 

#define FILT_DEFAULT = (v4 | v6)

enum { none, capture = 1, step = 2, backward = 4, } step_cmds;
