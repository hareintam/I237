#include <avr/pgmspace.h>

#ifndef HMI_MSG_H_
#define HMI_MSG_H_

#define VER_LIBC "Version: " FW_VERSION " built on: " __DATE__" "__TIME__"\n\r"
#define VER_FW "avr-libc version: "__AVR_LIBC_VERSION_STRING__ "\n\r"

extern PGM_P const stud_name[];
extern PGM_P const numbers[];
#endif /* HMI_MSG_H_ */
