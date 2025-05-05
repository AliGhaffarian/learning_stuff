#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

SEC("xdp")
int drop_everything(struct xdp_md *ctx){
	return XDP_ABORTED;
}

char __license[] SEC("license") = "GPL";
