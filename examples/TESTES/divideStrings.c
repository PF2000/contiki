#include "contiki.h"
#include "dev/ecc-algorithm.h"
#include "dev/ecc-curve.h"
#include "lib/random.h"
#include "sys/rtimer.h"
#include "sys/pt.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static void
printCenas(uint32_t *cenas)
{
	int i;
	//for (i=5; i>=0; i--){
	for (i=0; i<6; i++){
		printf("%06X", (unsigned int)cenas[i]); //0x%02x
	}
	printf("\n");
}

 PROCESS(example_process, "Example process");
 AUTOSTART_PROCESSES(&example_process);
 
 PROCESS_THREAD(example_process, ev, data)
 {
   PROCESS_BEGIN();
 
	const char *keys = NULL;	
	uint32_t brPubKeyX[6] ;// = NULL;	
	uint32_t brPubKeyY[6] ;// = NULL;	

	//REST.get_post_variable(request, "keys", &keys); 
	keys = "11FA2B68851DEDA9B0CE4D6EFD76F4623DD4600FEB5824EF;1B2585D62B7E6055C8534362A55F7F4F6EAB50F376CF18CE";

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
	printCenas(brPubKeyX);
	printf("PUB2 \n");
	printCenas(brPubKeyY);

	char keysJson[97] ;
	int j;
	
	for(j=5;j>=0;j--){
		if(j == 5){
			sprintf(keysJson,"%06X",(unsigned int)brPubKeyX[j]);
		}else{
			sprintf(keysJson + strlen(keysJson),"%06X",(unsigned int)brPubKeyX[j]);
		}
	}
	sprintf(keysJson + strlen(keysJson),";");
	for(j=5;j>=0;j--){
		printf("%d \n",strlen(keysJson));
		sprintf(keysJson + strlen(keysJson),"%06X",(unsigned int)brPubKeyY[j]);
	}
	printf("%s",keysJson);
	
	
 
   PROCESS_END();
 }
