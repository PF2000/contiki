#include "contiki.h"
#include <stdio.h>
 
 PROCESS(teste, "Example process");
 PROCESS(example_process, "Example process");
 //AUTOSTART_PROCESSES(&teste,&example_process);
AUTOSTART_PROCESSES(&teste);
 
 const char msg[] = "Data";
 
 static void
 example_function(void)
 {
 
   /* Start "Example process", and send it a NULL
      pointer. */
 
   process_start(&example_process, NULL);
  
   /* Send the PROCESS_EVENT_MSG event synchronously to
      "Example process", with a pointer to the message in the
      array 'msg'. */
   //process_post_synch(&example_process, PROCESS_EVENT_CONTINUE, &msg);
  
   /* Send the PROCESS_EVENT_MSG event asynchronously to 
      "Example process", with a pointer to the message in the
      array 'msg'. */
   //process_post(&example_process,PROCESS_EVENT_CONTINUE, msg);
 
   /* Poll "Example process". */
   //process_poll(&example_process);
 }


PROCESS_THREAD(teste, ev, data)
{
	PROCESS_BEGIN();

	while(1) {
		PROCESS_WAIT_EVENT();
		
		example_function();
		printf("dsf \n");
	}

 	PROCESS_END();
}
 



//AUTOSTART_PROCESSES(&example_process);
 
PROCESS_THREAD(example_process, ev, data)
{
  PROCESS_BEGIN();
 

    printf("Got event number %d \n", ev);
    printf("Got event number %s \n", (char*) msg);

 
  PROCESS_END();
}
