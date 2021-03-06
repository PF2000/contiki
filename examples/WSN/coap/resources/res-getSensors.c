
#include <stdlib.h>
#include <string.h>
#include "rest-engine.h"
#include "AESMessage.h"

static void res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);

RESOURCE(res_getSensors,
         "Get all disponible sensors",
         res_get_handler,
         NULL,
         NULL,
         NULL);

static void
res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{
  //REST.set_header_content_type(response, REST.type.TEXT_PLAIN);
  //snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, "Temperature;Humidity;Luminosity;Loudness;");  
  char str1[200];
  uint8_t hexSend1[256];

  REST.set_header_content_type(response, REST.type.APPLICATION_JSON);
  
  snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, 
	    "{\"Sensors\":{\"1\":\"Temperature\",\"2\":\"Humidity\",\"3\":\"Luminosity\",\"4\":\"Light\"}}");
  
  strcpy(str1, (char *)buffer);

  int length = encriptMessage(str1, hexSend1);

  REST.set_response_payload(response, hexSend1, length);
  //REST.set_response_payload(response, (uint8_t *)buffer, strlen((char *)buffer));
}
