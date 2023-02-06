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
