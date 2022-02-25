/**
 * CLT03.h
 *
 
 */
//*****user variable declaration*****
#ifndef CLTO3_H
#define CLT03_H 

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

static uint16_t tmp_data;
static uint8_t pos;
static uint16_t CLT03_16Bit_Data ;

//*****user function declaration*****

bool Clt03_ReadChannel(uint8_t channelindex);
ST_PLC_ErrorCode Clt03_ReadNibble(uint8_t *Data);


#ifdef __cplusplus
}
#endif

#endif
