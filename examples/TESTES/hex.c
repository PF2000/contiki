/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
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
 *
 */

/**
 * \file
 *         An example sending a UDP stream
 * \author
 *         Simon Duquennoy <simonduq@sics.se>
 */

#include "contiki.h"
#include "contiki-net.h"
#include "uip.h"
#include "net/rpl/rpl.h"
#include <stdio.h>
#include <string.h>


/*---------------------------------------------------------------------------*/
PROCESS(hex_process, "Hex Process");
AUTOSTART_PROCESSES(&hex_process);
PROCESS_THREAD(hex_process, ev, data)
{

  PROCESS_BEGIN();
  PROCESS_PAUSE();

    char str[100],strH[200],strH1[200];;

    int i,j;

    strcpy(str,"Hello World");

    printf("\nString is: %s\n",str);
    
    uint8_t hex[200];
		
		//string para hex
    for(i=0;i<strlen(str);i++)
    {
        hex[i] = str[i];        
    }
    
		// hex para  string 
    for(i=0 ; i<strlen(str); i++)
    {
        strH1[i] = hex[i];        
    }
    strH1[i]='\0'; /*adding NULL in the end*/

    printf("Hexadecimal converted string is: \n");
   
    printf("%s\n",strH1);

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
