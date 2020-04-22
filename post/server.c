#include "coap.h"

/*
 * The resource handler
 */ 
static void
hello_handler(coap_context_t *ctx, 
			  struct coap_resource_t *resource, 
              coap_session_t *session, 
              coap_pdu_t *request, 
              coap_binary_t *token, 
              coap_string_t *query, 
              coap_pdu_t *response) 
{
	unsigned char* data;
    size_t         data_len;
    if (coap_get_data(request, &data_len, &data))
    {
        printf("Received from Client: %s\n", data);
    }
}

int main(int argc, char* argv[])
{
	coap_context_t*  ctx;
	coap_address_t   serv_addr;
	coap_resource_t* hello_resource;
	int 			 coap_fd;
	unsigned 		 wait_ms;

	/* Prepare the CoAP server socket */ 
	coap_address_init(&serv_addr);
	serv_addr.addr.sin.sin_family      = AF_INET;
	serv_addr.addr.sin.sin_addr.s_addr = INADDR_ANY;
	serv_addr.addr.sin.sin_port        = htons(5683); //default port
	ctx                                = coap_new_context(&serv_addr);
	if (!ctx)
	{
		coap_log(LOG_DEBUG, "socket error\n");
		exit(1);
	}

	coap_fd = coap_context_get_coap_fd(ctx);
	if (coap_fd == -1)
	{
		coap_log(LOG_DEBUG, "file discription error\n");
		exit(1);
	}

	/* Initialize the hello resource */
	hello_resource = coap_resource_init(coap_make_str_const("hello"), 0);
	coap_register_handler(hello_resource, COAP_REQUEST_GET, hello_handler);
	coap_add_resource(ctx, hello_resource);

	/*Listen for incoming connections*/
	wait_ms = COAP_RESOURCE_CHECK_TIME * 1000;

	while (1)
	{
		int result = coap_run_once(ctx, wait_ms);
		if (result < 0)
		{
			break;
		}
	}
	coap_free_context(ctx);    
}
