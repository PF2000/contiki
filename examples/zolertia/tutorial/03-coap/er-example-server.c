
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "contiki-net.h"
#include "rest-engine.h"
#include "sys/etimer.h"
#include "dev/dht22.h"
//luz
#include "dev/tsl256x.h"
//som
#include "dev/adc-sensors.h"

#if CONTIKI_TARGET_ZOUL
#include "dev/adc-zoul.h"
#include "dev/zoul-sensors.h"
#else
#include "dev/adxl345.h"
#endif

#include "dev/button-sensor.h"
/*---------------------------------------------------------------------------*/
#define DEBUG 1
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

/*---------------------------------------------------------------------------*/
/*
 * Resources to be activated need to be imported through the extern keyword.
 * The build system automatically compiles the resources in the corresponding
 * sub-directory.
 */

extern resource_t
  ////res_hello,
  res_leds,
  res_ledr,
  res_ledg,
  res_ledb,
  //res_address,
  res_resetMote,
  res_getSensors,
  res_getHumidity,
  res_getTemperature,
  res_getLight,
  res_getLoudness,
  res_getAllSensorsValues,
  //res_battery,
  res_toggle,
#if CONTIKI_TARGET_ZOUL
  res_mirror,
  res_push,
  res_sub,
  res_sht25,
  res_zoul,
#else /* Default is Z1 */
  res_adxl345,
#endif
  res_event,
  res_separate;
/*---------------------------------------------------------------------------*/
static void
print_radio_values(void)
{
  radio_value_t aux;

  printf("\n* Radio parameters:\n");

  NETSTACK_RADIO.get_value(RADIO_PARAM_CHANNEL, &aux);
  printf("   Channel %u", aux);

  NETSTACK_RADIO.get_value(RADIO_CONST_CHANNEL_MIN, &aux);
  printf(" (Min: %u, ", aux);

  NETSTACK_RADIO.get_value(RADIO_CONST_CHANNEL_MAX, &aux);
  printf("Max: %u)\n", aux);

  NETSTACK_RADIO.get_value(RADIO_PARAM_TXPOWER, &aux);
  printf("   Tx Power %3d dBm", aux);

  NETSTACK_RADIO.get_value(RADIO_CONST_TXPOWER_MIN, &aux);
  printf(" (Min: %3d dBm, ", aux);

  NETSTACK_RADIO.get_value(RADIO_CONST_TXPOWER_MAX, &aux);
  printf("Max: %3d dBm)\n", aux);

  /* This value is set in contiki-conf.h and can be changed */
  printf("   PAN ID: 0x%02X\n", IEEE802154_CONF_PANID);
}
/*---------------------------------------------------------------------------*/
PROCESS(er_example_server, "Erbium Example Server");
AUTOSTART_PROCESSES(&er_example_server);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(er_example_server, ev, data)
{
  PROCESS_BEGIN();

  PROCESS_PAUSE();

  /* Initialize the REST engine. */
  rest_init_engine();

  /* Enable the sensors and devices */
  SENSORS_ACTIVATE(button_sensor);

  //sensor de temperature e humidade
  SENSORS_ACTIVATE(dht22);
  SENSORS_ACTIVATE(tsl256x);

  //Sound sensor is on the 5V - pin 2
  adc_sensors.configure(ANALOG_GROVE_LOUDNESS, 2);

  //Configure light sensor
  tsl256x.configure(TSL256X_INT_OVER, 0x15B8);

#if CONTIKI_TARGET_ZOUL
  adc_zoul.configure(SENSORS_HW_INIT, ZOUL_SENSORS_ADC_ALL);
#else /* Default is Z1 */
  SENSORS_ACTIVATE(adxl345);
#endif

  /*
   * Bind the resources to their Uri-Path.
   * WARNING: Activating twice only means alternate path, not two instances!
   * All static variables are the same for each URI path.
   */
  ////rest_activate_resource(&res_hello, "test/hello");
  ////rest_activate_resource(&res_battery, "data/battery");
  //rest_activate_resource(&res_address, "data/address");
  rest_activate_resource(&res_resetMote, "data/resetMote");
  rest_activate_resource(&res_getSensors, "data/getSensors");
  rest_activate_resource(&res_getHumidity, "data/getHumidity");
  rest_activate_resource(&res_getTemperature, "data/getTemperature");
  rest_activate_resource(&res_getLight, "data/getLight");
  rest_activate_resource(&res_getLoudness, "data/getLoudness");
  rest_activate_resource(&res_getAllSensorsValues, "data/getAllSensorsValues");
  rest_activate_resource(&res_leds, "actuators/leds");
  rest_activate_resource(&res_ledr, "actuators/ledr");
  rest_activate_resource(&res_ledg, "actuators/ledg");
  rest_activate_resource(&res_ledb, "actuators/ledb");
  rest_activate_resource(&res_toggle, "actuators/toggle");
  rest_activate_resource(&res_event, "sensors/button");
  rest_activate_resource(&res_separate, "test/separate");

#if CONTIKI_TARGET_ZOUL
  rest_activate_resource(&res_push, "test/push");
  rest_activate_resource(&res_sub, "test/sub");
  rest_activate_resource(&res_sht25, "sensors/sht25");
  rest_activate_resource(&res_zoul, "sensors/zoul");
#else
  rest_activate_resource(&res_adxl345, "sensors/adxl345");
#endif
  

  printf("\nCoAP server started\n");
  print_radio_values();

  while(1) {
    static struct etimer et;
    etimer_set(&et,CLOCK_SECOND*5);
    /* Wait forever */
    PROCESS_YIELD();

    //if(ev == sensors_event && data == &button_sensor) {
    if(etimer_expired(&et)) {
      /* Call the event_handler for this application-specific event. */
      res_event.trigger();

      /* Also call the separate response example handler. */
      res_separate.resume();
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
