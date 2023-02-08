#include "include/mqoe.h"
#include "lib.h"

// is_lqosd_alive, clear_ip_mappings, delete_ip_mapping, validate_shaped_devices,
// is_libre_already_running, create_lock_file, free_lock_file

int shell(char *command) {
		if(config->runshell) {
				FILE *fp = popen(command,"w");
				fprintf(fp,data);
				fclose(fp);
		}
}

int shellTC(char *command) {
		do();
}

/*
tc class add dev ens16f1np1 parent 0x2:0x3 classid 0xa2 htb rate 38Mbit ceil 38Mbit
tc qdisc add dev ens16f1np1 parent 0x2:0xa2 fq_codel
tc class add dev ens16f0np0 parent 0x2:0x3 classid 0xa2 htb rate 11Mbit ceil 11Mbit
tc qdisc add dev ens16f0np0 parent 0x2:0xa2 fq_codel
I gave up. It is just easier to think about this in binary.
*/

int tc_htb_add(char *buf, u32 parent, u32 classid, u32 rate, u32 ceil) {
	return sprintf(buf,"tc class add dev %s classid %x:%x htb rate %ukbit ceil");
}


bool first_run_since_boot() {
}

bool idel(char *iface) {
		char buf[255];
		sprintf(buf, "tc qdisc del dev %s", iface);
		return(shell(buf));
}

bool clear_prior() {
		if(config->onastick) {
				return(idel(config->in));
		} else {
				return(idel(config->out) && idel(config->in));
		}
}

bool teardown() {
		clear_ip_mappings();
		return clear_prior();
}

bool findQueues() {

}

bool validatenetwork() {

}

bool loadcircuits() {

}

bool refreshShapers() {

}

// xps_setup, xdp_iphash_to_cpu, tcclassify
