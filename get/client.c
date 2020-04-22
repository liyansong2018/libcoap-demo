#include "coap.h"
#include <stdio.h>
#include <netinet/in.h>

/*
 * The response handler
 */ 
static void
message_handler(struct coap_context_t *ctx,
                coap_session_t *session,
                coap_pdu_t *sent,
                coap_pdu_t *received,
                const coap_tid_t id) 
{
	unsigned char* data;
	size_t         data_len;

	coap_log(LOG_DEBUG, "** process incoming %d.%02d response:\n",
           (received->code >> 5), received->code & 0x1F);

	if (COAP_RESPONSE_CLASS(received->code) == 2) 
	{
		if (coap_get_data(received, &data_len, &data))
		{
			printf("Received: %s\n", data);
		}
	}
}

static coap_session_t *
setup_client_session(struct in_addr ip_address)
{
	coap_session_t *session;
	coap_address_t server;
	coap_context_t *context = coap_new_context(NULL);

	if(!context)
		return NULL;
	coap_address_init(&server);
	server.addr.sa.sa_family = AF_INET;
	server.addr.sin.sin_addr = ip_address;
	server.addr.sin.sin_port = htons(5683);

	session = coap_new_client_session(context, NULL, &server, COAP_PROTO_UDP);
	if (!session)
	{
		coap_free_context(context);
		return NULL;
	}
	return session;
}

int main(int argc, char* argv[])
{
	/* Create session */
	coap_session_t session;
	struct in_addr ip_address;
	ulong l1 = inet_addr("0.0.0.0");
	memcpy(&ip_address, &l1, 4);
	session = setup_client_session(ip_address);

	coap_context_t*   ctx;
	coap_address_t    dst_addr, src_addr;
	int   			  coap_fd;

	static coap_uri_t uri;
	fd_set            readfds; 
	coap_pdu_t*       request;
	const char*       server_uri = "coap://127.0.0.1/hello";
	unsigned char     get_method = 1;
	/* Prepare coap socket*/
	coap_address_init(&src_addr);
	src_addr.addr.sin.sin_family      = AF_INET;
	src_addr.addr.sin.sin_port        = htons(0);
	src_addr.addr.sin.sin_addr.s_addr = inet_addr("0.0.0.0");
	ctx = coap_new_context(&src_addr);
	/* The destination endpoint */
	// coap_address_init(&dst_addr);
	// dst_addr.addr.sin.sin_family      = AF_INET;
	// dst_addr.addr.sin.sin_port        = htons(5683);
	// dst_addr.addr.sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	/* Prepare the request */

	coap_split_uri(server_uri, strlen(server_uri), &uri);
	request            = coap_new_pdu(session);	
	request->type = COAP_MESSAGE_CON;
	request->tid   = coap_new_message_id(ctx);
	request->code = get_method;
	coap_add_option(request, COAP_OPTION_URI_PATH, uri.path.length, uri.path.s);
	/* Set the handler and send the request */
	coap_register_response_handler(ctx, message_handler);
	coap_send_confirmed(ctx, ctx->endpoint, &dst_addr, request);

	coap_fd = coap_context_get_coap_fd(ctx);
	FD_ZERO(&readfds);
	FD_SET( coap_fd, &readfds );
	int result = select( FD_SETSIZE, &readfds, 0, 0, NULL );
	if ( result < 0 ) /* socket error */
	{
		exit(EXIT_FAILURE);
	} 
	else if ( result > 0 && FD_ISSET( coap_fd, &readfds )) /* socket read*/
	{	 
		coap_run_once(ctx, COAP_RUN_NONBLOCK);  
	} 
  return 0;
}