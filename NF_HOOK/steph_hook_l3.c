#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/netfilter.h>
#include <linux/ip.h>

#define NEEDLE "STEPH"
#define NEEDLE_LEN 5

/* Source IP address in big endian */
//static const __be32 addr = 0x8301a8c0;
static const __be32 addr = 0x100010a;

/* Implementation of strstr() ignoring '\0'
 * len1: length of haystack
 * len2: length of needle
 */
static inline unsigned char *str_str(unsigned char *haystack, const unsigned char *needle,
			int len1, int len2)
{
	int i;

	if (len2 > len1)
		return NULL;

	for(i = 0; i <= len1 - len2; i++) {
		if (*(haystack + i) == needle[0]) {
			if (memcmp(haystack + i, needle, len2) == 0)
				return haystack + i;
		}
	}

	return NULL;
}

/*
 * Only process the packet going to IP address 'addr'
 */
static unsigned int steph_hook(void *priv,
				struct sk_buff *skb,
				const struct nf_hook_state *state)
{
	struct iphdr *iphdr;

	if (IS_ERR_OR_NULL(skb))
		return NF_DROP;

	iphdr = ip_hdr(skb);
	if ((iphdr->saddr != addr) || (iphdr->protocol != IPPROTO_TCP))
		return NF_ACCEPT;

	if (str_str(skb->data, NEEDLE, skb->len, NEEDLE_LEN)) {
		pr_info("STEPH: Dropping packet: found string \"%s\"", NEEDLE);
		return NF_DROP;
	}

	return NF_ACCEPT;
}

static struct net *net_ns;

static struct nf_hook_ops steph_hook_ops = {
	.hook = steph_hook,
	.pf = NFPROTO_IPV4,
	.hooknum = NF_INET_LOCAL_IN,
	.priority = 0,
};

static int __init steph_init(void)
{
	pr_info("Registering custom module STEPH\n");
	pr_debug("Using nsproxy from PID %d\n", current->pid);

	/* Checkout nsproxy.h -> can be dereferenced without precautions */
	net_ns = current->nsproxy->net_ns;
	if (!net_ns) {
		pr_err("No net_namespace could be found for current\n");
		return -1;
	}

	return nf_register_net_hook(net_ns, &steph_hook_ops);
}

static void __exit steph_exit(void)
{
	pr_info("Unregistering custom module STEPH...\n");
	nf_unregister_net_hook(net_ns, &steph_hook_ops);
}

module_init(steph_init)
module_exit(steph_exit)
MODULE_LICENSE("GPL");
