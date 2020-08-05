/*******************************************************************************
* File Name: VDAC8.c
* Version 2.10
*
* Description:
*  This file provides the source code of APIs for the DVDAC component.
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "VDAC8.h"
#include "VDAC8_VDAC8.h"
#include "CyDmac.h"
#include "VDAC8_DMA_dma.H"

#if(VDAC8_INTERNAL_CLOCK_USED)
    #include "VDAC8_IntClock.h"
#endif /* VDAC8_INTERNAL_CLOCK_USED */


static uint8 VDAC8_ditheredValues[16u] = {0x20u, 0x20u, 0x20u, 0x20u, 0x20u, 0x20u, 0x20u, 0x20u, 0x20u, 0x20u, 
                                          0x20u, 0x20u, 0x20u, 0x20u, 0x20u, 0x20u};


uint8 VDAC8_initVar = 0u;

static uint8 VDAC8_dmaChan;
static uint8 VDAC8_dmaTd = CY_DMA_INVALID_TD;

static void VDAC8_InitDma(void)  ;


/*******************************************************************************
* Function Name: VDAC8_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call VDAC8_Init() because
*  the VDAC8_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  All registers will be set to values according to the customizer Configure
*  dialog.
*
* Global variables:
*  None
*
*******************************************************************************/
void VDAC8_Init(void) 
{
    VDAC8_VDAC8_Init();

    if(CY_DMA_INVALID_TD == VDAC8_dmaTd)
    {
        VDAC8_InitDma();
    }
}


/*******************************************************************************
* Function Name: VDAC8_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary to
*  call VDAC8_Enable() because the VDAC8_Start() API calls
*  this function, which is the preferred method to begin component operation.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  None
*
*******************************************************************************/
void VDAC8_Enable(void) 
{
    (void) CyDmaChEnable(VDAC8_dmaChan, 1u);

    #if(VDAC8_INTERNAL_CLOCK_USED)
        VDAC8_IntClock_Start();
    #endif /* VDAC8_INTERNAL_CLOCK_USED */

    VDAC8_VDAC8_Enable();
}


/*******************************************************************************
* Function Name: VDAC8_Start
********************************************************************************
*
* Summary:
*  Performs all of the required initialization for the component and enables
*  power to the block. The first time the routine is executed, the component is
*  initialized to the configured settings. When called to restart the DVDAC
*  following a VDAC8_Stop() call, the current component parameter
*  settings are retained.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If the VDAC8_initVar variable is already set, this function only
*  calls the VDAC8_Enable() function.
*
* Global variables:
*  VDAC8_initVar - indicates whether the DVDAC has been initialized.
*  The variable is initialized to 0 and set to 1 the first time DVDAC_Start()
*  is called. This allows the component to restart without reinitialization
*  after the first call to the VDAC8_Start() routine. If
*  reinitialization of the component is required, then the
*  VDAC8_Init() function can be called before the
*  VDAC8_Start() or VDAC8_Enable() function.
*
*******************************************************************************/
void VDAC8_Start(void) 
{
    /* Hardware initiazation only needs to occure the first time */
    if(0u == VDAC8_initVar)
    {
        VDAC8_Init();
        VDAC8_initVar = 1u;
    }

    VDAC8_Enable();
}


/*******************************************************************************
* Function Name: VDAC8_Stop
********************************************************************************
*
* Summary:
*  Stops the component and turns off the analog blocks in the DVDAC.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  None
*
*******************************************************************************/
void VDAC8_Stop(void) 
{
    #if(VDAC8_INTERNAL_CLOCK_USED)
        VDAC8_IntClock_Stop();
    #endif /* VDAC8_INTERNAL_CLOCK_USED */

    (void) CyDmaChDisable(VDAC8_dmaChan);
    VDAC8_VDAC8_Stop();
}


/*******************************************************************************
* Function Name: VDAC8_SetValue
********************************************************************************
*
* Summary:
*  Sets the DVDACs output. The function populates the SRAM array based on the
*  value and the resolution setting. That array is then transferred to the
*  internal VDAC by DMA.
*
* Parameters:
*  (uint16) value:  The maximum value will be dependent on the resolution
*  selected:
*   Resolution       Valid range
*        9 bits       0x0 - 0x1FE
*       10 bits       0x0 - 0x3FC
*       11 bits       0x0 - 0x7F8
*       12 bits       0x0 - 0xFF0
*
*  This value includes an integer portion and a fractional portion that varies
*  depending on number of bits of resolution:
*   - 9-bits:  1 fractional bit
*   - 10-bits: 2 fractional bits
*   - 11-bits: 3 fractional bits
*   - 12-bits: 4 fractional bits
*
* Return:
*  None
*
* Global variables:
*  None
*
*******************************************************************************/
void VDAC8_SetValue(uint16 value) 
{
    uint8  lowCount  = (uint8)(value & VDAC8_FRACTIONAL_PORTION_MASK);
    uint8 highCount  = (uint8)(value >> VDAC8_INTEGER_PORTION_SHIFT);
    uint8 i;


    CYASSERT(value <= VDAC8_DVDAC_MAX_VALUE);

    if (VDAC8_INTEGER_PORTION_MAX_VALUE == highCount)
    {
        for (i = 0u; i < VDAC8_DITHERED_ARRAY_SIZE; i++)
        {
            VDAC8_ditheredValues[i] = VDAC8_INTEGER_PORTION_MAX_VALUE;
        }
    }
    else
    {
        for(i = 0u; i < VDAC8_DITHERED_ARRAY_SIZE; i++)
        {
            if(i < (VDAC8_DITHERED_ARRAY_SIZE - lowCount))
            {
                VDAC8_ditheredValues[i] = highCount;
            }
            else
            {
                VDAC8_ditheredValues[i] = highCount + 1u;
            }
        }
    }
}


/*******************************************************************************
* Function Name: VDAC8_InitDma
********************************************************************************
*
* Summary:
*  Configures a DMA transfer of channel values from memory to VDAC.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void VDAC8_InitDma(void)  
{
    /***************************************************************************
    * Transfers channel values from SRAM to VDAC register. One byte transfer,
    * each burst requires a request.
    ***************************************************************************/
    VDAC8_dmaChan = VDAC8_DMA_DmaInitialize(
                                    VDAC8_DMA_BYTES_PER_BURST,
                                    VDAC8_DMA_REQUEST_PER_BURST,
                                    HI16(VDAC8_DMA_SRC_BASE),
                                    HI16(VDAC8_DMA_DST_BASE));

    VDAC8_dmaTd = CyDmaTdAllocate();


    /***************************************************************************
    * One TD looping on itself, increment the source address, but not the
    * destination address.
    ***************************************************************************/
    (void) CyDmaTdSetConfiguration( VDAC8_dmaTd,
                                    VDAC8_DITHERED_ARRAY_SIZE,
                                    VDAC8_dmaTd,
                                    (uint8) CY_DMA_TD_INC_SRC_ADR);

    /* Transfers the value for each channel from memory to VDAC */
    (void) CyDmaTdSetAddress(   VDAC8_dmaTd,
                                LO16((uint32)VDAC8_ditheredValues),
                                LO16((uint32)VDAC8_VDAC8_Data_PTR));

    (void) CyDmaChSetInitialTd(VDAC8_dmaChan, VDAC8_dmaTd);
}

/* [] END OF FILE */

