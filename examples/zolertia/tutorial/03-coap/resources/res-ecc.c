/*
 * Copyright (c) 2013, Institute for Pervasive Computing, ETH Zurich
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *      Example resource
 * \author
 *      Matthias Kovatsch <kovatsch@inf.ethz.ch>
 */

#include "contiki.h"

#include "dev/ecc-algorithm.h"
#include "dev/ecc-curve.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rest-engine.h"
#include "dev/leds.h"
#include "sys/timer.h"
#include "sys/process.h"


#include "AESMessage.h"
#include "EECHelper.h"


uint32_t brPubKeyX[6] ;	
uint32_t brPubKeyY[6] ;	
uint32_t MyPubKeyX[6] ;	
uint32_t MyPubKeyY[6] ;	


//PROCESS(simple_ecdh, "Simple-ECDH");

static void res_post_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);

/*
 * A handler function named [resource name]_handler must be implemented for each RESOURCE.
 * A buffer for the response payload is provided through the buffer pointer. Simple resources can ignore
 * preferred_size and offset, but must respect the REST_MAX_CHUNK_SIZE limit for the buffer.
 * If a smaller block size is requested for CoAP, the REST framework automatically splits the data.
 */
RESOURCE(res_ecc,
         "title=\"Hello world: ?len=0..\";rt=\"Text\"",
         NULL,
         res_post_put_handler,
         res_post_put_handler,
         NULL);


static void
res_post_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{

	const char *keys = NULL;	

	REST.get_post_variable(request, "keys", &keys); 
	//keys = "11FA2B68851DEDA9B0CE4D6EFD76F4623DD4600FEB5824EF;1B2585D62B7E6055C8534362A55F7F4F6EAB50F376CF18CE";

 	const char s[2] = ";";
	char *token;
			
	token = strtok((char *)keys, s);

   int i=0;
   while( token != NULL ) 
   {      
			printf( "token %d - %s\n",i ,token );
			int j,k;
			char aux[8] ;
   		if(i==0){
				k=5;
				for(j=0;j<48;j+=8){
					strncpy(aux, &token[j] , 8);
					brPubKeyX[k] = strtoul(aux,NULL,16);
					k--;				
				}
			}else if(i==1){
				k=5;
				for(j=0;j<48;j+=8){
					strncpy(aux, &token[j] , 8);
					brPubKeyY[k] = strtoul(aux,NULL,16);
					k--;				
				}
			}
  
      token = strtok(NULL, s);
  		i++; 	   
	}
	
	printf("PUB1 \n");
	printKey192(brPubKeyX);
	printf("PUB2 \n");
	printKey192(brPubKeyY);
	
	
	//Já tenho as da box chaves vou gerar as minhas !!
	//TESTES
	//uint32_t brX[6] = { 0xEB5824EF, 0x3DD4600F, 0xFD76F462, 0xB0CE4D6E, 0x851DEDA9, 0x11FA2B68 }; //side_a.point_out.x PUB Box	
	//uint32_t brY[6] = { 0x76CF18CE, 0x6EAB50F3, 0xA55F7F4F, 0xC8534362, 0x2B7E6055, 0x1B2585D6 }; //side_a.point_out.y PUB Box
	
	//memcpy(brPubKeyX, brX, sizeof(uint32_t) * 6);
	//memcpy(brPubKeyY, brY, sizeof(uint32_t) * 6);
	//TESTES
	//PT_THREAD(ecc_compare(ecc_compare_state_t *state)){



	
  //Chama processo para calcular!!    
  //process_start(&simple_ecdh, NULL);  
  
  //PROCESS_CONTEXT_BEGIN(&simple_ecdh);
	//process_post_synch(PROCESS_CURRENT(),PROCESS_EVENT_INIT, NULL);
	//PT_WAIT_UNTIL(PROCESS_CURRENT(), process_is_running(&simple_ecdh));
     
   
//while(	process_is_running(&simple_ecdh) )
//{
	//printf("sdfsdf\n");
	//clock_wait(CLOCK_SECOND);	
//}    
 	//printf("MyPubKeyX ");
	//printCenas(MyPubKeyX); 
	//printf("MyPubKeyY ");
	//printCenas(MyPubKeyY); 
         

  //char const *message = "Olá Mundo!12345678901234567890123456789012345678901234567890!!12345678901234567890!!12345678901234567890";
  //int length = 150;

	//memcpy(buffer, message, length);


  REST.set_header_content_type(response, REST.type.TEXT_PLAIN); 
  //REST.set_response_payload(response, buffer, length);

}


//--------------------------------------------------------------------------------------------------------------------
//------------------------------ECDH----------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------




/*

PROCESS_THREAD(simple_ecdh, ev, data) {
  PROCESS_BEGIN();


 
	puts("-----------------------------------------\n"
	     "Initializing pka...");
	pka_init();

	static ecc_compare_state_t state = {
	  .process = &simple_ecdh,
	  .size    = 6,
	};

	memcpy(state.b, nist_p_192.n, sizeof(uint32_t) * 6);
	//FORCAR CAHVE igual para testes depois tem de ser random
	//static uint32_t secret_a[6] = { 0x62EDF96D, 0x269509CD, 0xF3CE3AEF, 0x23CF0515, 0x120147FD, 0x9DA32D80 };
	static uint32_t secret_a[6] ;
	do {
		ecc_set_random(secret_a);
	  memcpy(state.a, secret_a, sizeof(uint32_t) * 6);
	  PT_SPAWN(&(simple_ecdh.pt), &(state.pt), ecc_compare(&state));
	} while(state.result != PKA_STATUS_A_LT_B);



	static ecc_multiply_state_t side_a = {
	  .process    = &simple_ecdh,
	  .curve_info = &nist_p_192,
	};
	memcpy(side_a.point_in.x, nist_p_192.x, sizeof(uint32_t) * 6);
	memcpy(side_a.point_in.y, nist_p_192.y, sizeof(uint32_t) * 6);
	memcpy(side_a.secret, secret_a, sizeof(secret_a));


	printf("secret_a ");
	printKey192(secret_a);  	

	//Gera pontos para chave publica
	PT_SPAWN(&(simple_ecdh.pt), &(side_a.pt), ecc_multiply(&side_a));


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
	PT_SPAWN(&(simple_ecdh.pt), &(side_a.pt), ecc_multiply(&side_a));


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
*/
