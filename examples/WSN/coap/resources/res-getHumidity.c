
#include <stdlib.h>
#include <string.h>
#include "rest-engine.h"

#include "dev/dht22.h"

static void res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);

RESOURCE(res_getHumidity,
	"",
         res_get_handler,
         NULL,
         NULL,
         NULL);

static void
res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{
  SENSORS_ACTIVATE(dht22);
  static int temperature,humidity;

  char str1[200];
  uint8_t hexSend1[256];

  REST.set_header_content_type(response, REST.type.APPLICATION_JSON);
  
  if(dht22_read_all(&temperature, &humidity) != DHT22_ERROR) {

  }else{
	temperature = -1;
	humidity = -1;
  }

  snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, 
	    "{\"Sensors\":{\"Humidity\":\"%02d.%02d\"}}",
             humidity / 10, humidity % 10);

  strcpy(str1, (char *)buffer);

  int length = encriptMessage(str1, hexSend1);

  REST.set_response_payload(response, hexSend1, length);
  //REST.set_response_payload(response, (uint8_t *)buffer, strlen((char *)buffer));
}
