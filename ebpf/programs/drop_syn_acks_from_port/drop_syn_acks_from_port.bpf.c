

#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>

#define DROP_SRC_IP_N_ORDER  0x0100007f 
#define DROP_SRC_PORT_N_ORDER bpf_htons(8000)

SEC("xdp")
int drop_syn_acks_from_port(struct xdp_md *ctx){
	void *start_of_pkt = (void *)(long)ctx->data;
	void *end_of_pkt = (void *)(long)ctx->data_end;


	if(start_of_pkt + sizeof(struct ethhdr) > end_of_pkt)
		return XDP_PASS;
	struct ethhdr *ethernet_header = start_of_pkt;

	if(ethernet_header->h_proto != bpf_htons(ETH_P_IP))
		return XDP_PASS;


	start_of_pkt += sizeof(struct ethhdr);
	if(start_of_pkt + sizeof(struct iphdr) > end_of_pkt)
		return XDP_PASS;
	struct iphdr *ip_header = start_of_pkt;

	if(ip_header->protocol != IPPROTO_TCP)
		return XDP_PASS;

	if(ip_header->saddr != DROP_SRC_IP_N_ORDER)
		return XDP_PASS;


	start_of_pkt += ip_header->ihl * 4;
	if(start_of_pkt + sizeof(struct tcphdr)> end_of_pkt)
		return XDP_PASS;
	struct tcphdr *tcp_header = start_of_pkt;

	if(tcp_header->source != DROP_SRC_PORT_N_ORDER)
		return XDP_PASS;

	if(tcp_header->syn == 0 || tcp_header->ack == 0)
		return XDP_PASS;


	return XDP_DROP;
}

char __license[] SEC("license") = "GPL";
