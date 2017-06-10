#include "contiki.h"
#include "dev/ecc-algorithm.h"
#include "dev/ecc-curve.h"
#include "lib/random.h"
#include "sys/rtimer.h"
#include "sys/pt.h"

#include <string.h>
#include <stdio.h>


static void
printCenas(uint32_t *cenas)
{
	int i;
	for (i=7; i>=0; i--){
		printf("%08X", cenas[i]);
	}
	printf("\n");
}


PROCESS(ecdh_test1, "ecdh test");
AUTOSTART_PROCESSES(&ecdh_test1);

PROCESS_THREAD(ecdh_test1, ev, data) {
  PROCESS_BEGIN();

  /*
   * Variable for Time Measurement
   */
  static rtimer_clock_t time;


  /*
   * Activate Engine
   */
  puts("-----------------------------------------\n"
       "Initializing pka...");
  pka_init();

  /*
   * Generate secrets make sure they are valid (smaller as order)
   */
  static ecc_compare_state_t state = {
    .process = &ecdh_test,
    .size    = 8,
  };

  memcpy(state.b, nist_p_256.n, sizeof(uint32_t) * 8);
  static uint32_t secret_a[8] = { 0x106BBF67, 0xED1E6C2E, 0xA5958DCA, 0x5B3DDBAC, 0xAB7F9B52, 0xA86B1F61, 0xC89E1E2E, 0xAEAD966A };
  do {
    memcpy(state.a, secret_a, sizeof(uint32_t) * 8);
    PT_SPAWN(&(ecdh_test.pt), &(state.pt), ecc_compare(&state));
  } while(state.result != PKA_STATUS_A_LT_B);


  /*
   * Prepare Points
   */
  static ecc_multiply_state_t side_a = {
    .process    = &ecdh_test,
    .curve_info = &nist_p_256,
  };
  memcpy(side_a.point_in.x, nist_p_256.x, sizeof(uint32_t) * 8);
  memcpy(side_a.point_in.y, nist_p_256.y, sizeof(uint32_t) * 8);
  memcpy(side_a.secret, secret_a, sizeof(secret_a));


	printf("secret_a ");
	printCenas(secret_a);  	

  /*
   * Round 1
   */
  time = RTIMER_NOW();
  PT_SPAWN(&(ecdh_test.pt), &(side_a.pt), ecc_multiply(&side_a));
  time = RTIMER_NOW() - time;
  printf("Round 1, Side a: %i, %lu ms\n", (unsigned)side_a.result,
         (uint32_t)((uint64_t)time * 1000 / RTIMER_SECOND));


//static uint32_t ax[8] = { 0xF4280B5F, 0x8AE8932F, 0xDC3ACF21, 0x4520EB60, 0xCF267089, 0x1425BAFA, 0xF5399E1F, 0x4DB59315 };
//static uint32_t ay[8] = { 0x9E490EEA, 0x4D279486, 0x7917FE55, 0x0ADCE589, 0x1CF0B6F5, 0xFC3931F7, 0xEDC843DD, 0x1A0C4C69 };
static uint32_t bx[8] = { 0xF4280B5F, 0x8AE8932F, 0xDC3ACF21, 0x4520EB60, 0xCF267089, 0x1425BAFA, 0xF5399E1F, 0x4DB59315 };
static uint32_t by[8] = { 0x9E490EEA, 0x4D279486, 0x7917FE55, 0x0ADCE589, 0x1CF0B6F5, 0xFC3931F7, 0xEDC843DD, 0x1A0C4C69 };
     printf("Before Key Exchange \n ");

	printf("side_a.point_in.x ");
	printCenas(side_a.point_in.x); 
	printf("side_a.point_in.y ");
	printCenas(side_a.point_in.y); 
	printf("side_a.point_out.x ");
	printCenas(side_a.point_out.x); 
	printf("side_a.point_out.y ");
	printCenas(side_a.point_out.y); 

  /*
   * Key Exchange
   */

  //memcpy(side_a.point_in.x, ax, sizeof(uint32_t) * 8);
  //memcpy(side_a.point_in.y, ay, sizeof(uint32_t) * 8);

  memcpy(side_a.point_in.x, bx, sizeof(uint32_t) * 8);
  memcpy(side_a.point_in.y, by, sizeof(uint32_t) * 8);




   printf("Key Exchange \n ");


	printf("side_a.point_in.x ");
	printCenas(side_a.point_in.x); 
	printf("side_a.point_in.y ");
	printCenas(side_a.point_in.y); 
	printf("side_a.point_out.x ");
	printCenas(side_a.point_out.x); 
	printf("side_a.point_out.y ");
	printCenas(side_a.point_out.y); 
	//printf("side_b.point_in.x ");
	//printCenas(side_b.point_in.x); 
	//printf("side_b.point_in.y ");
	//printCenas(side_b.point_in.y);  
  /*
   * Round 2
   */
  time = RTIMER_NOW();
  PT_SPAWN(&(ecdh_test.pt), &(side_a.pt), ecc_multiply(&side_a));
  time = RTIMER_NOW() - time;
  printf("Round 2, Side a: %i, %lu ms\n", (unsigned)side_a.result,
         (uint32_t)((uint64_t)time * 1000 / RTIMER_SECOND));

  /*
   * Check Result
   */
  memcpy(state.a, side_a.point_out.x, sizeof(uint32_t) * 8);
  //memcpy(state.b, side_b.point_out.x, sizeof(uint32_t) * 8);

	printf("side_a.point_out.x ");
	printCenas(side_a.point_out.x);  
  


  puts("-----------------------------------------\n"
       "Disabling pka...");
  pka_disable();

  puts("Done!");

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
/**
 * @}
 * @}
 */
