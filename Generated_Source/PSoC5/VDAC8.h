/*******************************************************************************
* File Name: VDAC8.h
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

#if !defined(CY_DVDAC_VDAC8_H)
#define CY_DVDAC_VDAC8_H

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
} VDAC8_BACKUP_STRUCT;

/* Component init state */
extern uint8 VDAC8_initVar;


/***************************************
*        API constants
***************************************/
#define VDAC8_RESOLUTION                 (12u)
#define VDAC8_INTERNAL_CLOCK_USED        (1u)

#define VDAC8_INTEGER_PORTION_SHIFT      (VDAC8_RESOLUTION - 8u)
#define VDAC8_DITHERED_ARRAY_SIZE        ((uint8)(1u << VDAC8_INTEGER_PORTION_SHIFT))
#define VDAC8_FRACTIONAL_PORTION_MASK    (VDAC8_DITHERED_ARRAY_SIZE - 1u)
#define VDAC8_INTEGER_PORTION_MAX_VALUE  (0xFFu)

/* DMA Configuration */
#define VDAC8_DMA_BYTES_PER_BURST        (1u)
#define VDAC8_DMA_REQUEST_PER_BURST      (1u)
#define VDAC8_DMA_SRC_BASE               (CYDEV_SRAM_BASE)
#define VDAC8_DMA_DST_BASE               (CYDEV_PERIPH_BASE)


#if   (VDAC8_RESOLUTION ==  9u)
    #define VDAC8_DVDAC_MAX_VALUE        (0x1FEu)
#elif (VDAC8_RESOLUTION == 10u)
    #define VDAC8_DVDAC_MAX_VALUE        (0x3FCu)
#elif (VDAC8_RESOLUTION == 11u)
    #define VDAC8_DVDAC_MAX_VALUE        (0x7F8u)
#else   /* Resolution 12 bits */
    #define VDAC8_DVDAC_MAX_VALUE        (0xFF0u)
#endif  /* (VDAC8_RESOLUTION ==  9u) */


/***************************************
*        Function Prototypes
***************************************/

void VDAC8_Init(void)                ;
void VDAC8_Enable(void)              ;
void VDAC8_Start(void)               ;
void VDAC8_Stop(void)                ;
void VDAC8_SetValue(uint16 value)    ;
void VDAC8_Sleep(void)               ;
void VDAC8_Wakeup(void)              ;
void VDAC8_SaveConfig(void)          ;
void VDAC8_RestoreConfig(void)       ;

#endif /* CY_DVDAC_VDAC8_H */


/* [] END OF FILE */
