
 #include "contiki.h"
 #include <string.h>
 #include <stdio.h>


 PROCESS(newexample_process, "Example222 process");
 AUTOSTART_PROCESSES(&newexample_process);
 

 PROCESS_THREAD(newexample_process, ev, data)
 {
   PROCESS_BEGIN();
   
		uint32_t secret[6];

   while(1) {
     PROCESS_WAIT_EVENT();
			int i;
			for(i = 0; i < 6; ++i) {
				do{
					secret[i] = (uint32_t)random_rand() | (uint32_t)random_rand() << 16 ;
				}while (secret[i] < 268435456);
			}
			
			for (i=5; i>=0; i--){
				printf("%06X ", (unsigned int)secret[i]); //0x%02x
			}
			printf("\n");
   }

   PROCESS_END();
 }
