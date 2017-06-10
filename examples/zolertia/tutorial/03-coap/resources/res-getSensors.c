
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

  REST.set_header_content_type(response, REST.type.APPLICATION_JSON);
  
  snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, 
	    "{\"Sensors\":{\"1\":\"Temperature\",\"2\":\"Humidity\",\"3\":\"Luminosity\",\"4\":\"Light\"}}");


  REST.set_response_payload(response, (uint8_t *)buffer, strlen((char *)buffer));
}
