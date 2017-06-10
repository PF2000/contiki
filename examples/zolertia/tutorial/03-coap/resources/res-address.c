

#include <stdlib.h>
#include <string.h>
#include "rest-engine.h"
#include "net/rime/rime.h"
#include "uip.h"
#include "uip-debug.h"
//#include "net/ip/uiplib.h"

static void res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);


RESOURCE(res_address,
         //"title=\"Get node address: ?len=0..\";rt=\"Text\"",
         "title=\"Get node address\";rt=\"Text\"",
         res_get_handler,
         NULL,
         NULL,
         NULL);

static void
res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{
  int length = 12;
  uip_ipaddr_t addr;


  uip_ip6addr_u8(&addr,0xaa, 0xaa, 0x00, 0x00,
	       0x00, 0x00, 0x00, 0x00,
	       0x02, 0x12, 0x74, 0x01,
	       0x00, 0x01, 0x01, 0x01);

  uip_debug_ipaddr_print(&addr);
  putchar('\n');

  //uiplib_ip6addrconv(&str,&addr);  
  char const *const message = (char*) &addr;


  memcpy(buffer, message, length);
  REST.set_header_content_type(response, REST.type.TEXT_PLAIN); /* text/plain is the default, hence this option could be omitted. */
  REST.set_header_etag(response, (uint8_t *)&length, 1);
  REST.set_response_payload(response, buffer, length);
}








