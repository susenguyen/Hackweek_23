#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <arpa/inet.h>

#include <stdint.h>
#include <linux/netfilter.h>
#include <linux/netfilter/nfnetlink.h>
#include <libnetfilter_queue/libnetfilter_queue.h>

/*
 * Deep Packet Inspector
 * Looks for string "STEPH" in the payload
 * Rejects if founds, lets through if not found
 */

#define handle_error(msg)		\
	do {				\
		perror(msg);		\
		exit(EXIT_FAILURE);	\
	} while(0)

#define BUF_SIZE 65536
#define NEEDLE "STEPH"
#define RCV_BUF_SIZE 819200

static unsigned char cb_buf[BUF_SIZE];
static unsigned char recv_buf[BUF_SIZE];

void dump_hex(const unsigned char *str, int len)
{
	unsigned int i;
	unsigned char hex_str[3 * BUF_SIZE];

	memset(hex_str, 0, 3 * BUF_SIZE);

	if (len > BUF_SIZE) {
		printf("%d > %d => limiting string size to %d", len, BUF_SIZE, BUF_SIZE);
		len = BUF_SIZE;
	}

	for(i = 0; i < len; i++)
		sprintf(&hex_str[3 * i], "%02x ", str[i]);

	printf("%s\n", hex_str);
}

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
 * Callback function to be called on packet enqueue
 * Checks for the presence of string "STEPH" in the payload
 * Payload includes ethhdr, iphdr or tcphdr/udphdr (if applicable)
 */
int cb(struct nfq_q_handle *nfq_q_hdl, struct nfgenmsg *nfmsg,
		struct nfq_data *nfad, void *data)
{
	int id, i, ret;
	unsigned char *payload;
	struct nfqnl_msg_packet_hdr *ph;

	if ((ph = nfq_get_msg_packet_hdr(nfad)) == NULL) {
		handle_error("nfq_get_msg_packet_hdr()");
	}

	id = ntohl(ph->packet_id);

	if ((ret = nfq_get_payload(nfad, &payload)) < 0) {
		return -1;
	}

	memcpy(cb_buf, (unsigned char *) payload, ret);

	//dump_hex(cb_buf, ret);

	// DPI: looking for string NEEDLE
	if (str_str(cb_buf, NEEDLE, ret, strlen(NEEDLE)) == NULL) {
		//printf("ACCEPT\n");
		return nfq_set_verdict(nfq_q_hdl, id, NF_ACCEPT, 0, NULL);
	} else {
		printf("DROP\n");
		return nfq_set_verdict(nfq_q_hdl, id, NF_DROP, 0, NULL);
	}
}

int main(int argc, char *argv[])
{
	int queue_num, fd, ret;
	unsigned int rcv_bufsiz;
	struct nfq_handle *nfq_hdl;
	struct nfq_q_handle *nfq_q_hdl;
	struct nfnl_handle *nfnlh;

	if (argc != 2) {
		printf("Syntax: %s <queue_num>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	queue_num = atoi(argv[1]);
	printf("Monitoring queue number [%d]\n", queue_num);

	if ((nfq_hdl = nfq_open()) == NULL)
		handle_error("nfq_open()");

	/*
	 * Optimization to reduce occurrence of the ENOBUF condition 
	 * Increases performance significantly
	 */
	if (nfnlh = nfq_nfnlh(nfq_hdl)) {
		rcv_bufsiz = nfnl_rcvbufsiz(nfnlh, RCV_BUF_SIZE);
		printf("Setting Receive Buffer size to %u bytes\n", rcv_bufsiz);
	}

	if ((nfq_q_hdl = nfq_create_queue(nfq_hdl, queue_num, cb, NULL)) == NULL)
		handle_error("nfq_create_queue");

	if (nfq_set_mode(nfq_q_hdl, NFQNL_COPY_PACKET, 0xffff) < 0)
		handle_error("nfq_set_mode");

	fd = nfq_fd(nfq_hdl);

	while(1) {
		if ((ret = recv(fd, (unsigned char *) recv_buf, BUF_SIZE, 0)) < 0) {
			perror("recv");
			continue;
		}

		if (nfq_handle_packet(nfq_hdl, (char *) recv_buf, ret) < 0)
			handle_error("nfq_handle_packet");
	}

	nfq_close(nfq_hdl);
	exit(EXIT_SUCCESS);
}
