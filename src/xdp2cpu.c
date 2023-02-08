#include "include/mqoe.h"

enum BusRequest { NOP, ACK, FAIL, MAPPEDIPS, MAPIPTOFLOW, DONE };

static const struct option xdp2cpu_options[] = {
	{ "validate", required_argument	, NULL , 'i' } ,
	{ "connect"    , required_argument	, NULL , 'c' } ,
	{ "reload" , required_argument	, NULL , 'r' } ,
	{ "shutdown"  , required_argument	, NULL , 's' } ,
	{ "dry-run"  , required_argument	, NULL , ' ' } ,
	{ "help"     , no_argument		, NULL , 'h' } ,
	{ "buffer"   , no_argument		, NULL , 'b' } ,
};

static const struct option xdp2cpu_add_options[] = {
	{ "ip", required_argument	, NULL , 'i' } ,
	{ "classid"    , required_argument	, NULL , 'C' } ,
	{ "cpu" , required_argument	, NULL , 'c' } ,
	{ "upload"  , required_argument	, NULL , 'u' } ,
	{ "help"     , no_argument		, NULL , 'h' } ,
};

void xdp2cpu_usage (char *err) {
	if(err) fprintf(stderr,"%s\n",err);
	printf("xdp2cpu [options]\n");
	printf(
		"\t-h --help\n"
		"\t-a add\n"
		"\t\t-i --ip\n"
		"\t\t-C --classid\n"
		"\t\t-c --cpu\n"
		"\t\t-u --upload\n"
		"\t-b --batch\n"
		"\t-d del\n"
		"\t-c clear\n"
		"\t-l list\n"
		"\t-c --command   [qdisc]\n");
	exit(-1);
}

#define GETOPT ("")

int main_xdp2cpu(int argc, char **argv) {
		xdp2cpu_usage("have to write this");
		return 0;
}

#ifndef BUSYBOX
int main(int argc, char **argv) {
		return main_xdp2cpu(argc,argv);
}
#endif

bool talk_to_server(BusRequest b) {
	switch(enum BusRequest responses = bus_request(b)) {
		case ACK: printf("Success"); return true;
		case MappedIps: print_ips(ips); return true;
		case Fail: E("Bus Failure");
		default: PE("Command execution failed"); return false;
	}
}

fn print_ips(ips: &[IpMapping]) {
  println!("\nMapped IP Addresses:");
  println!(
    "--------------------------------------------------------------------"
  );
  for ip in ips.iter() {
    let ip_formatted = if ip.ip_address.contains(':') {
      format!("{}/{}", ip.ip_address, ip.prefix_length)
    } else {
      format!("{}/{}", ip.ip_address, ip.prefix_length - 96)
    };
    println!(
      "{:<45} CPU: {:<4} TC: {}",
      ip_formatted,
      ip.cpu,
      ip.tc_handle.to_string()
    );
  }
  println!();
}

fn parse_add_ip(
  ip: &str,
  classid: &str,
  cpu: &str,
  upload: &Option<String>,
) -> Result<BusRequest> {
  //if ip.parse::<IpAddr>().is_err() {
  //    return Err(Error::msg(format!("Unable to parse IP address: {ip}")));
  //}
  if !classid.contains(':') {
    return Err(Error::msg(
      "Class id must be in the format (major):(minor), e.g. 1:12",
    ));
  }
  Ok(BusRequest::MapIpToFlow {
    ip_address: ip.to_string(),
    tc_handle: TcHandle::from_string(classid)?,
    cpu: u32::from_str_radix(&cpu.replace("0x", ""), 16)?, // Force HEX representation
    upload: upload.is_some(),
  })
}

#[tokio::main(flavor = "current_thread")]
pub async fn main() -> Result<()> {
  let cli = Args::parse();

  match cli.command {
    Some(Commands::Add { ip, classid, cpu, upload }) => {
      talk_to_server(parse_add_ip(&ip, &classid, &cpu, &upload)?).await?;
    }
    Some(Commands::Del { ip, upload }) => {
      talk_to_server(BusRequest::DelIpFlow {
        ip_address: ip.to_string(),
        upload: upload.is_some(),
      })
      .await?
    }
    Some(Commands::Clear) => talk_to_server(BusRequest::ClearIpFlow).await?,
    Some(Commands::List) => talk_to_server(BusRequest::ListIpFlow).await?,
    None => {
      println!("Run with --help to see instructions");
      exit(0);
    }
  }

  Ok(())
}
