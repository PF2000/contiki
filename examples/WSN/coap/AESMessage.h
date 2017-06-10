#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/aes-128.h"
#include "math.h"

extern uint8_t key[16]; 

int
encriptMessage(char *in, uint8_t *hexSend);

void
printKeyuint8(uint8_t *key);
