#include "main.h"



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ISO8200_H
#define ISO8200_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
     
/* Exported Constants --------------------------------------------------------*/

/** @defgroup ISO8200_Exported_Constants ISO8200AQ Exported Constants 
  * @{
  */   
/** Current FW major version */
#define ISO8200AQ_FW_MAJOR_VERSION (uint8_t)(1)
/** Current FW minor version */
#define ISO8200AQ_FW_MINOR_VERSION (uint8_t)(0)
/** Current FW patch version */
#define ISO8200AQ_FW_PATCH_VERSION (uint8_t)(0)
/** Current FW version */
#define ISO8200AQ_FW_VERSION       (uint32_t)((ISO8200AQ_FW_MAJOR_VERSION<<16)|\
                                              (ISO8200AQ_FW_MINOR_VERSION<<8)|\
                                              (ISO8200AQ_FW_PATCH_VERSION))

/** Number of channels by relay ISO8200AQ */
#define ISO8200AQ_NB_CHANNELS   (8U)

  /**
  * @}
  */

/* Exported Types  -------------------------------------------------------*/
 typedef struct
{
  /**Initial SPI frequency in Hz */
  uint32_t spiFreq;   
  /**Initial Tcss delay in us */
  uint8_t timingTcss;  
  /**Initial channel enable bitmap */
  uint8_t chanEnBitmap;  
} iso8200aq_Init_t; 

/** Device Parameters Structure Type */
typedef struct {
  /**channel enable output bitmap */
  uint8_t chanEnBitmap;                        
  /**channel duty cycle table  */
  uint8_t channelDc[ISO8200AQ_NB_CHANNELS];    
  /**channel frequency table */
  uint16_t channelFreq[ISO8200AQ_NB_CHANNELS]; 
}deviceParams_t; 

/* Exported functions --------------------------------------------------------*/

ST_PLC_ErrorCode ST_PLC_Isofaultdetect(uint8_t ModID, uint8_t *Faultbits);
ST_PLC_ErrorCode Iso8200aq_WriteBytes(uint8_t Module_ID , uint8_t *pByteToTransmit);


#ifdef __cplusplus
  }
#endif

#endif /* #ifndef ISO8200_H */


