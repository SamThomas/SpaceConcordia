#include "comms.h"

/** The buffer used for receiving stuff. */
char* comms_buffer; 
uint32_t comms_checksum; 


void comms_exec()
{
  int ret;  // Return value 
  printf("Communications Module Executing...\n");

  // Check if everything is ok with circuitry! 
  ret = comms_OK(); 
  if (!ret){
    
    printf("  :: Comms module [ OK ] \n");

    // Try to send something to base 
    printf("  :: Attempting to send message... "); 
    if (comms_send()) {
      printf(" [DONE] \n");
    } else {
      printf(" [FAIL] \n");  
    }
    
    // Try to receive something from base 
    printf("  :: Attempting to receive messasge... ");
    
    ret = comms_receive(); 

    if (!ret) {
      printf(" [DONE] : ");
      printf("[%s]\n", comms_buffer); 
      printf("  :: Checksum of buffer received   : %x \n", zerog_get_hash()); 
      printf("  :: Checksum of buffer calculated : %x \n", crc32(comms_buffer, strlen(comms_buffer))); 
    } else {
      printf(" [FAIL] Error : %d \n", ret); 
    }

  }
  else 
    printf("  :: Comms PROBLEM! Error number : %d\n", ret);
}

/** Simple routine to see if Communication circuitry is fried or not */
short comms_OK()
{
  // TODO - Proper code probably has to go here :P 
  switch (rand_get_u8()%30){
    case 1: // FATAL
      return 1; 
      break;
    default: // OK
      return 0; 
      break; 
  }
  return 0; // so compiler doesn't have the chance to complain - if it does, change your compiler. 
}

/** Send a message back to base station. */
short comms_send()
{
  // So this should probably 
  //   1) Send something and wait
  //   2) Double check with base that that was indeed what was sent
  //  ----- I'm not sure if we will be able to check that the base recieved the last message. Will have to check with the comms guys (SPACEACK?)
  
  if(rand_get_u8() > 64) // number is there for whatever reason - it's just to demo fail/succ scenarios 
    return 1; // Success 

  return 0; // Fail 
}

/** Gentlemen. ALL YOUR BASE R BELONG TO US. */
short comms_receive()
{
  comms_buffer = zerog_comms_receive();
  return 0; // TODO
}

/** Some checking to see if message is ok. */
short comms_check()
{
  return 0; // TODO 
}


/** Check yourself before you wreck yourself 

-> TODO I'm not a C wizard, but could the data pointer be turned into a void 
pointer in order to have some sort of templating? I've seen that before, but I'm
not sure if that's possible. 

-- a void pointer is useful in being able to point to anything. When you say templating
I'm not sure what you mean exactly but you can definitely do some common operations to 
a set of data types that are yet to be defined using void pointers. In this case one 
example would be to pass an array of bytes, 16 bit words, 32 bit words etc. In that case
you would have to have a (void * data, size_t dataSize, size_t numElem)
 
-- Sweet! Thanks for the info. 

**/
uint32_t crc32(char * data, size_t numBytes) //crc is fast might want crc16 instead
{
  MHASH td;
  uint32_t hash;

  td = mhash_init(MHASH_CRC32);

  if (td == MHASH_FAILED){//this might not be meaningful
    perror("mhash_init");
    return 0;
  }

  mhash(td, data, numBytes);

  mhash_deinit(td, &hash); 

  return hash;
}


