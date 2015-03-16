/*
 * clang tx.c -o tx -O0 -g -lubus -lubox -lblobmsg_json -levent
 */

#include <libubus.h>
#include <libubox/blobmsg_json.h>

static struct blob_buf b;


int main(int argc, char **argv)
{
	const char *ubus_socket = "/var/run/ubus.sock";
	struct ubus_context *ctx = ubus_connect(ubus_socket);
	if (!ctx) {
		fprintf(stderr, "Failed to connect to ubus\n");
		return -1;
	}

	blob_buf_init(&b, 0);
	bool ret = blobmsg_add_json_from_string(&b, "{\"a\":\"b\"}");
	if (!ret) {
		printf("fail\n");
		exit(1);
	}
	ubus_send_event(ctx, "foo", b.head);

	blob_buf_free(&b);
	ubus_free(ctx);
	return 0;
}
