/*
 * clang rx.c -o rx -O0 -g -lubus -lubox -lblobmsg_json -levent
 */

#include <libubus.h>
#include <libubox/blobmsg_json.h>
#include <event2/event.h>

static struct ubus_event_handler listener;

static void receive_event(struct ubus_context *ctx,
		struct ubus_event_handler *ev, const char *type, struct blob_attr *msg) {
	char *str;
	str = blobmsg_format_json(msg, true);
	printf("{ \"%s\": %s }\n", type, str);
	free(str);
}

void cb(evutil_socket_t fd, short what, void *arg) {
	struct ubus_context *ctx = arg;
	ubus_handle_event(ctx);
}

static void signal_cb(evutil_socket_t fd, short event, void *arg)
{
	struct event_base *evloop = arg;
	event_base_loopbreak(evloop);
}

int main(int argc, char **argv) {
	const char *ubus_socket = "/var/run/ubus.sock";
	struct ubus_context *ctx = ubus_connect(ubus_socket);
	if (!ctx) {
		fprintf(stderr, "Failed to connect to ubus\n");
		return -1;
	}

	memset(&listener, 0, sizeof(listener));
	listener.cb = receive_event;
	int ret = ubus_register_event_handler(ctx, &listener, "foo");
	if (ret)
		return ret;

	struct event_base *evloop = event_base_new();
	struct event *signal_int = evsignal_new(evloop, SIGINT, signal_cb, evloop);
	event_add(signal_int, NULL);

	struct event *e = event_new(evloop, ctx->sock.fd, EV_READ | EV_PERSIST, cb, ctx);
	event_add(e, NULL);

	event_base_dispatch(evloop);

	event_free(signal_int);
	event_free(e);
	event_base_free(evloop);

	ubus_free(ctx);
	return 0;
}
