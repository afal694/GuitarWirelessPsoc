/*******************************************************************************
* File Name: DVDAC.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the DVDAC component.
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_DVDAC_DVDAC_H)
#define CY_DVDAC_DVDAC_H

#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cytypes.h"


/***************************************
*   Data Struct Definition
***************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} DVDAC_BACKUP_STRUCT;

/* Component init state */
extern uint8 DVDAC_initVar;


/***************************************
*        API constants
***************************************/
#define DVDAC_RESOLUTION                 (12u)
#define DVDAC_INTERNAL_CLOCK_USED        (1u)

#define DVDAC_INTEGER_PORTION_SHIFT      (DVDAC_RESOLUTION - 8u)
#define DVDAC_DITHERED_ARRAY_SIZE        ((uint8)(1u << DVDAC_INTEGER_PORTION_SHIFT))
#define DVDAC_FRACTIONAL_PORTION_MASK    (DVDAC_DITHERED_ARRAY_SIZE - 1u)
#define DVDAC_INTEGER_PORTION_MAX_VALUE  (0xFFu)

/* DMA Configuration */
#define DVDAC_DMA_BYTES_PER_BURST        (1u)
#define DVDAC_DMA_REQUEST_PER_BURST      (1u)
#define DVDAC_DMA_SRC_BASE               (CYDEV_SRAM_BASE)
#define DVDAC_DMA_DST_BASE               (CYDEV_PERIPH_BASE)


#if   (DVDAC_RESOLUTION ==  9u)
    #define DVDAC_DVDAC_MAX_VALUE        (0x1FEu)
#elif (DVDAC_RESOLUTION == 10u)
    #define DVDAC_DVDAC_MAX_VALUE        (0x3FCu)
#elif (DVDAC_RESOLUTION == 11u)
    #define DVDAC_DVDAC_MAX_VALUE        (0x7F8u)
#else   /* Resolution 12 bits */
    #define DVDAC_DVDAC_MAX_VALUE        (0xFF0u)
#endif  /* (DVDAC_RESOLUTION ==  9u) */


/***************************************
*        Function Prototypes
***************************************/

void DVDAC_Init(void)                ;
void DVDAC_Enable(void)              ;
void DVDAC_Start(void)               ;
void DVDAC_Stop(void)                ;
void DVDAC_SetValue(uint16 value)    ;
void DVDAC_Sleep(void)               ;
void DVDAC_Wakeup(void)              ;
void DVDAC_SaveConfig(void)          ;
void DVDAC_RestoreConfig(void)       ;

#endif /* CY_DVDAC_DVDAC_H */


/* [] END OF FILE */
