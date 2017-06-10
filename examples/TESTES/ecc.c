#include "contiki.h"
#include "contiki-net.h"
#include "uip.h"
#include "net/rpl/rpl.h"
#include <stdio.h>
#include <string.h>


PROCESS(hex_process, "Hex Process");
AUTOSTART_PROCESSES(&hex_process);
PROCESS_THREAD(hex_process, ev, data)
{

  PROCESS_BEGIN();
  PROCESS_PAUSE();

   const char src[50] = "http://www.tutorialspoint.com";
   char dest[50];

   printf("Before memcpy dest = %s\n", dest);
   memcpy(dest, src, strlen(src)+1);
   printf("After memcpy dest = %s\n", dest);
	

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
