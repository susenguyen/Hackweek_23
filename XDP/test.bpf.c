#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>

#define NEEDLE "STEPH"
#define NEEDLE_LEN 5
#define MAX_PKT_SIZE 1550

//static const __be32 addr = 0x100007f;
static const __be32 addr = 0x100010a;

SEC("xdp")
int test(struct xdp_md *ctx) {
	void *data = (void *)(long)ctx->data;
	void *data_end = (void *)(long)ctx->data_end;
	void *payload;
	struct iphdr *iph;
	int i, j;

	if ((data_end - data) > 0xffff) {
		bpf_printk("STEPH: ((data_end - data) > MAX_PKT_SIZE)");
		return XDP_DROP;
	}

	if ((data + sizeof(struct ethhdr) + sizeof(struct iphdr)) > data_end) {
		bpf_printk("STEPH: ((data + sizeof(struct ethhdr) + sizeof(struct iphdr)) > ...");
		return XDP_DROP;
	}

	iph = (struct iphdr *) (data + sizeof(struct ethhdr));

	if ((iph->saddr != addr) || (iph->protocol != 6))	// TCP only
		return XDP_PASS;

	//payload = data + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct tcphdr);
	payload = data;

	if ((payload + NEEDLE_LEN) >= data_end) {
		bpf_printk("STEPH: ((payload + NEEDLE_LEN) >= data_end)");
		return XDP_DROP;
	}


        for(i = 0; i < (int)(data_end - payload) - NEEDLE_LEN; i++) {

		// No XDP support for jumbo frames
		if (i > MAX_PKT_SIZE) {
			bpf_printk("STEPH: (i > MAX_PKT_SIZE): %d", data_end - payload);
			return XDP_DROP;
		}

		if ((payload + i + NEEDLE_LEN) > data_end) {
			bpf_printk("STEPH: ((payload + i + NEEDLE_LEN) > data_end)");
			return XDP_DROP;
		}

                if (*((char *)payload + i) == NEEDLE[0]) {
                        if (__builtin_memcmp(payload + i, NEEDLE, NEEDLE_LEN) == 0) {
				bpf_printk("STEPH: Dropping Packet...");
				return XDP_DROP;
			}
                }
        }

	return XDP_PASS;
}

char LICENSE[] SEC("license") = "Dual BSD/GPL";
