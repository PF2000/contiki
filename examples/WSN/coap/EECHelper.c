#include "EECHelper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/random.h"

#include "math.h"


uint32_t brPubKeyX[6] ;	
uint32_t brPubKeyY[6] ;	
uint32_t MYPrivateKey[6];
uint32_t MyPubKeyX[6] ;	
uint32_t MyPubKeyY[6] ;	

void
printKey192(uint32_t *key)
{
	int i;
	for (i=5; i>=0; i--){
	//for (i=0; i<6; i++){
		printf("%06X", (unsigned int)key[i]); //0x%02x
	}
	printf("\n");
}

void
getMyPubKeys(char *keys)
{
	//Para concatenar as chaves publicas
	int j;
	for(j=5;j>=0;j--){
		if(j == 5){
			sprintf(keys,"%06X",(unsigned int)MyPubKeyX[j]);
		}else{
			sprintf(keys + strlen(keys),"%06X",(unsigned int)MyPubKeyX[j]);
		}
	}
	sprintf(keys + strlen(keys),";");
	for(j=5;j>=0;j--){
		sprintf(keys + strlen(keys),"%06X",(unsigned int)MyPubKeyY[j]);
	}
	printf("%s\n",keys);
}


void
ecc_set_random(uint32_t *secret)
{
  int i;
  for(i = 0; i < 6; ++i) {
		do{
			secret[i] = (uint32_t)random_rand() | (uint32_t)random_rand() << 16 ;
		}while (secret[i] < 268435456);
  }
}

void 
printBrPubKeyX(void)
{
    printKey192(brPubKeyX);
}
