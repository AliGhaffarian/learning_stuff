#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

int counter = 0;
SEC("xdp")
int print_packets(struct xdp_md *ctx){
	counter++;
	bpf_printk("counter: %d", counter);
	return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
