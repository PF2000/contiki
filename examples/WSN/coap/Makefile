all: er-example-server

<<<<<<< HEAD
CONTIKI = ../../..
=======
#CONTIKI = ../../../..
CONTIKI = ../../..

>>>>>>> 423fd4b7372f15b5eed77aa6a05c9f1ba7cf876a

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"


TARGET_LIBFILES += -lm 

# Automatically build RESTful resources
REST_RESOURCES_DIR = ./resources

REST_RESOURCES_FILES = $(notdir $(shell find $(REST_RESOURCES_DIR) -name '*.c'))
PROJECTDIRS += $(REST_RESOURCES_DIR)
PROJECT_SOURCEFILES +=  AESMessage.c 
PROJECT_SOURCEFILES +=  $(REST_RESOURCES_FILES)  


CONTIKI_TARGET_SOURCEFILES += sht25.c

MODULES += core/net/llsec/adaptivesec

# linker optimizations
SMALL = 1

# REST Engine shall use Erbium CoAP implementation
APPS += er-coap
APPS += rest-engine


CONTIKI_WITH_IPV6 = 1
include $(CONTIKI)/Makefile.include

