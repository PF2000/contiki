#include <string.h>
#include "rest-engine.h"
#include "er-coap.h"

#include <math.h>
#include "dev/dht22.h"
#include "dev/adc-sensors.h"
#include "dev/tsl256x.h"

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINT6ADDR(addr) PRINTF("[%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x]", ((uint8_t *)addr)[0], ((uint8_t *)addr)[1], ((uint8_t *)addr)[2], ((uint8_t *)addr)[3], ((uint8_t *)addr)[4], ((uint8_t *)addr)[5], ((uint8_t *)addr)[6], ((uint8_t *)addr)[7], ((uint8_t *)addr)[8], ((uint8_t *)addr)[9], ((uint8_t *)addr)[10], ((uint8_t *)addr)[11], ((uint8_t *)addr)[12], ((uint8_t *)addr)[13], ((uint8_t *)addr)[14], ((uint8_t *)addr)[15])
#define PRINTLLADDR(lladdr) PRINTF("[%02x:%02x:%02x:%02x:%02x:%02x]", (lladdr)->addr[0], (lladdr)->addr[1], (lladdr)->addr[2], (lladdr)->addr[3], (lladdr)->addr[4], (lladdr)->addr[5])
#else
#define PRINTF(...)
#define PRINT6ADDR(addr)
#define PRINTLLADDR(addr)
#endif

static void res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);
static void res_event_handler(void);

/*
 * Example for an event resource.
 * Additionally takes a period parameter that defines the interval to call [name]_periodic_handler().
 * A default post_handler takes care of subscriptions and manages a list of subscribers to notify.
 */
PERIODIC_RESOURCE(res_event,
               "title=\"Event demo\";obs",
               res_get_handler,
               NULL,
               NULL,
               NULL,
               5 * CLOCK_SECOND,
               res_event_handler);

/*
 * Use local resource state that is accessed by res_get_handler() and altered by res_event_handler() or PUT or POST.
 */
static int32_t event_counter = 0;
static int loudness,dbs,temperature,humidity,light;

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
	temperature = 0;
	humidity = 0;
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

static void
res_event_handler(void)
{
  /* Do the update triggered by the event here, e.g., sampling a sensor. */
  ++event_counter;

  /* Usually a condition is defined under with subscribers are notified, e.g., event was above a threshold. */
  if(1) {
    PRINTF("TICK %u for /%s\n", event_counter, res_event.url);

    /* Notify the registered observers which will trigger the res_get_handler to create the response. */
    REST.notify_subscribers(&res_event);
  }
}
