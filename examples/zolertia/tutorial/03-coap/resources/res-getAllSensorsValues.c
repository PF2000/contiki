
#include <stdlib.h>
#include <string.h>
#include "rest-engine.h"
#include <math.h>
#include "dev/dht22.h"
#include "dev/adc-sensors.h"
#include "dev/tsl256x.h"
#include "AESMessage.h"

static int loudness,dbs,temperature,humidity,light;

static void res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);

RESOURCE(res_getAllSensorsValues,
	"",
         res_get_handler,
         NULL,
         NULL,
         NULL);

static void
res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{
  SENSORS_ACTIVATE(dht22);
  SENSORS_ACTIVATE(tsl256x);

  //Sound sensor is on the 5V - pin 2
  adc_sensors.configure(ANALOG_GROVE_LOUDNESS, 2);

  //Configure light sensor
  tsl256x.configure(TSL256X_INT_OVER, 0x15B8);


  REST.set_header_content_type(response, REST.type.APPLICATION_JSON);//REST.type.TEXT_PLAIN);
  
  if(dht22_read_all(&temperature, &humidity) != DHT22_ERROR) {
	//snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, "HUM%02d.%02d", humidity / 10, humidity % 10); 
  	//snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, "TEM%02d.%02d", temperature / 10, temperature % 10); 
  }else{
	temperature = -1;
	humidity = -1;
  }

  dbs = adc_sensors.value(ANALOG_GROVE_LOUDNESS);
  loudness= round(20*(0.43429*log(dbs))*10)/10;
  
  if(loudness != 96){
  	//snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, "LOU%d", loudness);  
  }else{
    loudness = -1;
  }

  light = tsl256x.value(TSL256X_VAL_READ);
  if(light != -1){
    //snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, "\"LIGHT\":\"%d\"", light);  
  }

  if(strlen((char *)buffer) == 0){
	//snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, "Em branco");  
  }  	

  
  snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, 
	    "{\"Sensors\":{\"Temperature\":\"%02d.%02d\",\"Humidity\":\"%02d.%02d\",\"Loudness\":\"%02d\",\"Light\":\"%02d\"}}",
             temperature / 10, temperature % 10,humidity / 10, humidity % 10,loudness,light);

  REST.set_response_payload(response, (uint8_t *)buffer, strlen((char *)buffer));
}
