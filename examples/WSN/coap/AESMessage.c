#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/aes-128.h"
#include "math.h"


#include "AESMessage.h"


uint8_t key[16];

int
encriptMessage(char *in, uint8_t *hexSend)
{

		//ONLY SENDS DATA UP TO 128 CHARS
		
		//COMUNICATION KEY
    const	uint8_t MYKEY[16] = { 0x00 , 0x01 , 0x02 , 0x03 , 0x04 , 0x05 , 0x06 , 0x07 , 0x08 , 0x09 , 0x0A , 0x0B ,  0x0C , 0x0D , 0x0E , 0x0F };


		char str[200];
		int i,j = 0;

		for(i=0;i<200;i++){
			str[i] = (char)0;
		}
		
 		strcpy(str, in);

    printf("\nString is: %s\n",str);
        
    uint8_t hex[16];
    //uint8_t hexSend[256];
		int posActualAntes = 0;
		int posActualDepois = 0;
		
		int resto = 0 ;
		if(strlen(str)%16 == 0){
			resto =1;
		}
		
		int block = (strlen(str)/16)+1 - resto;		

		int length = (int)16* block ;


		//ecriptar
	 //const uint8_t key[16] = { 0x00 , 0x01 , 0x02 , 0x03 ,
   //                   0x04 , 0x05 , 0x06 , 0x07 ,
   //                   0x08 , 0x09 , 0x0A , 0x0B ,
   //                   0x0C , 0x0D , 0x0E , 0xFF };

	printf("length %d block %d \n", length ,block);

	//muda a chave 
	AES_128.set_key(key);


	for(i=0;i<length;i++)
	{
		hexSend[i] = 0;       
	}


	for(j=0 ; j < block ; j ++){

		for(i=0;i<16;i++)
		{
			hex[i] = 0;       
		}

		for(i=0;i<16;i++)
		{
			hex[i] = str[posActualAntes];
			posActualAntes ++;     
		}

		AES_128.encrypt(hex);

		for(i=0;i<16;i++)
		{
			hexSend[posActualDepois] = hex[i];  
      posActualDepois ++;   
		}
	}


	//print hex cifrado estar cifrado
	for(i=0;i<length;i++)
  {
      printf("%02X", hexSend[i]);      
  }
 
	printf("\n FIM %d\n", strlen(str));

	//resets to old key (Comunication key)
	AES_128.set_key(MYKEY); 

	return length;

}

void
printKeyuint8(uint8_t *key)
{
	int i;
	for (i=0; i<16; i++){
		printf("%02X", (unsigned int)key[i]); //0x%02x
	}
	printf("\n");
}
