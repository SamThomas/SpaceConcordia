#ifndef _COMMS_H_
#define _COMMS_H_

// Libs 
#include <stdio.h>
#include <stdint.h>
#include <mhash.h> //linux hash library

// modules
#include "zerog_module.h"

/** The buffer used for receiving stuff. */
char* comms_buffer; 
uint32_t comms_checksum; 

void comms_exec();

short comms_OK(); 
short comms_send();
short comms_receive();
short comms_check();

//checksum
uint32_t crc32(char * data, size_t bytes);

#endif
