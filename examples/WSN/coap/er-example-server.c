
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "contiki-net.h"
#include "rest-engine.h"

#include "sys/timer.h"
#include "sys/process.h"
#include "sys/pt.h"
#include "dev/ecc-algorithm.h"
#include "dev/ecc-curve.h"

#include "AESMessage.h"
#include "EECHelper.h"

#include <string.h>
#include <stdio.h>


#include "sys/etimer.h"
#include "dev/dht22.h"
//luz
#include "dev/tsl256x.h"
//som
#include "dev/adc-sensors.h"


#include "dev/adc-zoul.h"
#include "dev/zoul-sensors.h"
#include "dev/button-sensor.h"



uint32_t brPubKeyX[6] ;	
uint32_t brPubKeyY[6] ;	
uint32_t MYPrivateKey[6];
uint32_t MyPubKeyX[6] ;	
uint32_t MyPubKeyY[6] ;	



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
  res_hello,
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
  //res_ecc,
  //res_pubKeys,
  res_mirror,
  res_push,
  res_sub,
  res_sht25,
  res_zoul,
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
PROCESS(genSharedKey_ecdh, "End-ECDH");

AUTOSTART_PROCESSES(&er_example_server,&genSharedKey_ecdh);
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

  adc_zoul.configure(SENSORS_HW_INIT, ZOUL_SENSORS_ADC_ALL);


  /*
   * Bind the resources to their Uri-Path.
   * WARNING: Activating twice only means alternate path, not two instances!
   * All static variables are the same for each URI path.
   */
  //rest_activate_resource(&res_hello, "test/hello");
  ////rest_activate_resource(&res_battery, "data/battery");
  //rest_activate_resource(&res_address, "data/address");
  rest_activate_resource(&res_resetMote, "data/Reset");
  rest_activate_resource(&res_getSensors, "data/Sensors");
  rest_activate_resource(&res_getHumidity, "data/Humidity");
  rest_activate_resource(&res_getTemperature, "data/Temperature");
  rest_activate_resource(&res_getLight, "data/Light");
  rest_activate_resource(&res_getLoudness, "data/Loudness");
  rest_activate_resource(&res_getAllSensorsValues, "data/AllValues");
  rest_activate_resource(&res_leds, "actuators/Leds");
  rest_activate_resource(&res_ledr, "actuators/LedRed");
  rest_activate_resource(&res_ledg, "actuators/LedGreen");
  rest_activate_resource(&res_ledb, "actuators/LedBlue");
  //rest_activate_resource(&res_toggle, "actuators/toggle");
  //rest_activate_resource(&res_ecc, "actuators/ecc");
  //rest_activate_resource(&res_pubKeys, "actuators/pubKeys");
  rest_activate_resource(&res_event, "observe/Event");
  //rest_activate_resource(&res_separate, "test/separate");
  //rest_activate_resource(&res_push, "test/push");
  //rest_activate_resource(&res_sub, "test/sub");
  //rest_activate_resource(&res_sht25, "sensors/sht25");
  //rest_activate_resource(&res_zoul, "sensors/zoul");

  printf("\nCoAP server started\n");
  print_radio_values();

  while(1) {
    /* Wait forever */
    PROCESS_YIELD();

    if(ev == sensors_event && data == &button_sensor) {
      /* Call the event_handler for this application-specific event. */
      res_event.trigger();

      /* Also call the separate response example handler. */
      res_separate.resume();
    }
  }

  PROCESS_END();
}
//-----------------------------------------------------------------------------------------------
//--------------------------------------ECDH-----------------------------------------------------
//-----------------------------------------------------------------------------------------------

PROCESS_THREAD(genSharedKey_ecdh, ev, data) {

  PROCESS_BEGIN();

	puts("-----------------------------------------\n"
	     "Initializing pka...");
	pka_init();

	static ecc_compare_state_t state = {
	  .process = &genSharedKey_ecdh,
	  .size    = 6,
	};

	memcpy(state.b, nist_p_192.n, sizeof(uint32_t) * 6);
	static uint32_t secret_a[6];
		memcpy(secret_a, MYPrivateKey, sizeof(uint32_t) * 6);
	do {
	  memcpy(state.a, secret_a, sizeof(uint32_t) * 6);
	  PT_SPAWN(&(genSharedKey_ecdh.pt), &(state.pt), ecc_compare(&state));
	} while(state.result != PKA_STATUS_A_LT_B);



	static ecc_multiply_state_t side_a = {
	  .process    = &genSharedKey_ecdh,
	  .curve_info = &nist_p_192,
	};
	memcpy(side_a.point_in.x, nist_p_192.x, sizeof(uint32_t) * 6);
	memcpy(side_a.point_in.y, nist_p_192.y, sizeof(uint32_t) * 6);
	memcpy(side_a.secret, secret_a, sizeof(secret_a));


	printf("secret_a ");
	printKey192(secret_a);  	

	//Gera pontos para chave publica
	PT_SPAWN(&(genSharedKey_ecdh.pt), &(side_a.pt), ecc_multiply(&side_a));


	   printf("Before Key Exchange - Chaves Publicas \n ");
	
	printf("side_a.point_out.x ");
	printKey192(side_a.point_out.x); 
	printf("side_a.point_out.y ");
	printKey192(side_a.point_out.y); 


	//Troca de chaves
	//actualiza com chaves publicas do border router
	memcpy(side_a.point_in.x, brPubKeyX, sizeof(uint32_t) * 6);
	memcpy(side_a.point_in.y, brPubKeyY, sizeof(uint32_t) * 6);



	printf("Key Exchange \n ");


	printf("side_a.point_in.x ");
	printKey192(side_a.point_in.x); 
	printf("side_a.point_in.y ");
	printKey192(side_a.point_in.y); 
	printf("side_a.point_out.x ");
	printKey192(side_a.point_out.x); 
	printf("side_a.point_out.y ");
	printKey192(side_a.point_out.y); 

	//define a pontos da chave publica para enviar
	memcpy(MyPubKeyX, side_a.point_out.x, sizeof(uint32_t) * 6);
	memcpy(MyPubKeyY, side_a.point_out.y, sizeof(uint32_t) * 6);
		
		
		
	//Determina chave partilhada
	PT_SPAWN(&(genSharedKey_ecdh.pt), &(side_a.pt), ecc_multiply(&side_a));


	printf("side_a.point_out.x Chave Partilhada  ");
	printKey192(side_a.point_out.x);  
	
	printf("side_a.point_out.y Chave Partilhada  ");
	printKey192(side_a.point_out.y);  
	
	//GRAVA A CHAVE SIMETRICA
	memcpy(key , &side_a.point_out.x, sizeof(key));

	printKeyuint8(key);


	puts("-----------------------------------------\n"
	     "Disabling pka...");
	pka_disable();

	puts("Done!");

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
