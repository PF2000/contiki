#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/random.h"

//porque se usa extern
//https://stackoverflow.com/questions/1433204/how-do-i-use-extern-to-share-variables-between-source-files

extern uint32_t brPubKeyX[6];	
extern uint32_t brPubKeyY[6];
extern uint32_t MYPrivateKey[6];	
extern uint32_t MyPubKeyX[6];	
extern uint32_t MyPubKeyY[6];	

void
printKey192(uint32_t *key);

void
ecc_set_random(uint32_t *secret);

void 
printBrPubKeyX(void);

void
getMyPubKeys(char *keys);



