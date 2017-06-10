/*
 * Copyright (c) 2013, Institute for Pervasive Computing, ETH Zurich
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
 */
/*---------------------------------------------------------------------------*/
/**
 * \file
 *      Erbium (Er) example project configuration.
 * \author
 *      Matthias Kovatsch <kovatsch@inf.ethz.ch>
 */
/*---------------------------------------------------------------------------*/
#ifndef __PROJECT_ERBIUM_CONF_H__
#define __PROJECT_ERBIUM_CONF_H__

/* This should match the Border Router configuration */
#undef UIP_CONF_BUFFER_SIZE
#define UIP_CONF_BUFFER_SIZE           256

#undef IEEE802154_CONF_PANID
#define IEEE802154_CONF_PANID          0xABCD

/* Disabling RDC for demo purposes */
#undef NETSTACK_CONF_RDC
#define NETSTACK_CONF_RDC              nullrdc_driver

/* Disabling TCP on CoAP nodes. */
#undef UIP_CONF_TCP
#define UIP_CONF_TCP                   0

/* Increase rpl-border-router IP-buffer when using more than 64. */
#undef REST_MAX_CHUNK_SIZE

#if CONTIKI_TARGET_ZOUL

#undef CC2538_RF_CONF_CHANNEL
#define CC2538_RF_CONF_CHANNEL         26

#define REST_MAX_CHUNK_SIZE            128//96

#else /* Default is Z1 */
/* Override this due to RAM overflow */
#undef NBR_TABLE_CONF_MAX_NEIGHBORS
#define NBR_TABLE_CONF_MAX_NEIGHBORS   5
#undef UIP_CONF_MAX_ROUTES
#define UIP_CONF_MAX_ROUTES            5
#undef COAP_MAX_OBSERVERS
#define COAP_MAX_OBSERVERS             3

/* Change Radio default parameters */
#undef RF_CHANNEL
#define RF_CHANNEL	               26

#undef CC2420_CONF_CHANNEL
#define CC2420_CONF_CHANNEL            26

#define REST_MAX_CHUNK_SIZE            48
#endif

/* Multiplies with chunk size, be aware of memory constraints. */
#undef COAP_MAX_OPEN_TRANSACTIONS
#define COAP_MAX_OPEN_TRANSACTIONS     4

/* Filtering .well-known/core per query can be disabled to save space. */
#undef COAP_LINK_FORMAT_FILTERING
#define COAP_LINK_FORMAT_FILTERING     0
#undef COAP_PROXY_OPTION_PROCESSING
#define COAP_PROXY_OPTION_PROCESSING   0

/* Enable client-side support for COAP observe */
#define COAP_OBSERVE_CLIENT 1



/**
* \brief Maximum PM
*
* The SoC will never drop to a Power Mode deeper than the one specified
here.
* 0 for PM0, 1 for PM1 and 2 for PM2
*/
#ifndef LPM_CONF_MAX_PM
#define LPM_CONF_MAX_PM 2
#endif



/* Security */



#ifndef CRYPTO_CONF_INIT
#define CRYPTO_CONF_INIT        1 
#endif

#ifndef AES_128_CONF
#define AES_128_CONF            cc2538_aes_128_driver 
#endif

#ifndef CCM_STAR_CONF
#define CCM_STAR_CONF           cc2538_ccm_star_driver 
#endif

#undef LLSEC802154_CONF_ENABLED
#define LLSEC802154_CONF_ENABLED          1

#undef NETSTACK_CONF_FRAMER
#define NETSTACK_CONF_FRAMER              noncoresec_framer

#undef NETSTACK_CONF_LLSEC
#define NETSTACK_CONF_LLSEC               noncoresec_driver

#undef NONCORESEC_CONF_SEC_LVL
#define NONCORESEC_CONF_SEC_LVL          7

#define NONCORESEC_CONF_KEY { 0x00 , 0x01 , 0x02 , 0x03 , \
                               0x04 , 0x05 , 0x06 , 0x07 , \
                               0x08 , 0x09 , 0x0A , 0x0B , \
                               0x0C , 0x0D , 0x0E , 0x0F }



#endif
