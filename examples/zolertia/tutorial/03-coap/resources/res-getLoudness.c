
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "rest-engine.h"
#include "dev/adc-sensors.h"
#include "AESMessage.h"

static int loudness,dbs;

static void res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);

RESOURCE(res_getLoudness,
         "Get loudness",
         res_get_handler,
         NULL,
         NULL,
         NULL);

static void
res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{
  //som
  dbs = adc_sensors.value(ANALOG_GROVE_LOUDNESS);
  loudness= round(20*(0.43429*log(dbs))*10)/10;

  if(loudness == 96){
  	loudness = -1;
  }

  REST.set_header_content_type(response, REST.type.APPLICATION_JSON);
  
  snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, 
	    "{\"Sensors\":{\"Loudness\":\"%02d\"}}",
             loudness);


  REST.set_response_payload(response, (uint8_t *)buffer, strlen((char *)buffer));
}
