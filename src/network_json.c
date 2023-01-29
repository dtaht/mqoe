#include "network_json.h"

const char *json = "{\"name\":\"Mash\",\"star\":4,\"hits\":[2,2,1,3]}";


struct bandwidth {
		u64 bps;
		u64 pps;
};

struct bidir {
		bandwidth up;
		bandwidth down;
};
