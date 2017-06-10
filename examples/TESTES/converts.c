
 #include "contiki.h"
 #include <string.h>
 #include <stdio.h>


 PROCESS(example_process, "Example process");
 AUTOSTART_PROCESSES(&example_process);


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
printKeyuint8(uint8_t *key)
{
	int i;
	for (i=0; i<16; i++){
	//for (i=0; i<6; i++){
		printf("%02X", (unsigned int)key[i]); //0x%02x
	}
	printf("\n");
}



 PROCESS_THREAD(example_process, ev, data)
 {
   PROCESS_BEGIN();

 
		uint32_t secret_a[6] = { 0x93B93A58, 0x75D44D32, 0xFA2AA79C, 0x4129039E, 0x4C5910DE, 0x4FD2153A };

		uint8_t key[16]= { 0x00 , 0x01 , 0x02 , 0x03 , 0x04 , 0x05 , 0x06 , 0x07 , 0x08 , 0x09 , 0x0A , 0x0B , 0x0C , 0x0D , 0x0E , 0xFF };
	
		uint8_t key1[16] ;
		
		printf("secret_a\n");
		printKey192(secret_a);
		printf("\n");

		printf("key\n");
		printKeyuint8(key);
		printf("\n");

		int i;
		int j=4;
		for (i=0; i<16; i++){
		
			//key1[i] = secret_a[j]
		
			if(i==5){
				j++;			
			}	
		}
		
		memcpy(key1 , &secret_a, sizeof(key1));

		printf("new key\n");
		printKeyuint8(key1);
		printf("\n");

	//CB2F8D6C8F41E6595258AAB866B9567183874EE8A58196B0
	//B09681A5E84E87837156B966B8AA5852


   PROCESS_END();
 }
