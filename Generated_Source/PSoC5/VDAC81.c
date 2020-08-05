/*******************************************************************************
* File Name: VDAC81.c  
* Version 1.90
*
* Description:
*  This file provides the source code to the API for the 8-bit Voltage DAC 
*  (VDAC8) User Module.
*
* Note:
*  Any unusual or non-standard behavior should be noted here. Other-
*  wise, this section should remain blank.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "VDAC81.h"

#if (CY_PSOC5A)
#include <CyLib.h>
#endif /* CY_PSOC5A */

uint8 VDAC81_initVar = 0u;

#if (CY_PSOC5A)
    static uint8 VDAC81_restoreVal = 0u;
#endif /* CY_PSOC5A */

#if (CY_PSOC5A)
    static VDAC81_backupStruct VDAC81_backup;
#endif /* CY_PSOC5A */


/*******************************************************************************
* Function Name: VDAC81_Init
********************************************************************************
* Summary:
*  Initialize to the schematic state.
* 
* Parameters:
*  void:
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void VDAC81_Init(void) 
{
    VDAC81_CR0 = (VDAC81_MODE_V );

    /* Set default data source */
    #if(VDAC81_DEFAULT_DATA_SRC != 0 )
        VDAC81_CR1 = (VDAC81_DEFAULT_CNTL | VDAC81_DACBUS_ENABLE) ;
    #else
        VDAC81_CR1 = (VDAC81_DEFAULT_CNTL | VDAC81_DACBUS_DISABLE) ;
    #endif /* (VDAC81_DEFAULT_DATA_SRC != 0 ) */

    /* Set default strobe mode */
    #if(VDAC81_DEFAULT_STRB != 0)
        VDAC81_Strobe |= VDAC81_STRB_EN ;
    #endif/* (VDAC81_DEFAULT_STRB != 0) */

    /* Set default range */
    VDAC81_SetRange(VDAC81_DEFAULT_RANGE); 

    /* Set default speed */
    VDAC81_SetSpeed(VDAC81_DEFAULT_SPEED);
}


/*******************************************************************************
* Function Name: VDAC81_Enable
********************************************************************************
* Summary:
*  Enable the VDAC8
* 
* Parameters:
*  void
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void VDAC81_Enable(void) 
{
    VDAC81_PWRMGR |= VDAC81_ACT_PWR_EN;
    VDAC81_STBY_PWRMGR |= VDAC81_STBY_PWR_EN;

    /*This is to restore the value of register CR0 ,
    which is modified  in Stop API , this prevents misbehaviour of VDAC */
    #if (CY_PSOC5A)
        if(VDAC81_restoreVal == 1u) 
        {
             VDAC81_CR0 = VDAC81_backup.data_value;
             VDAC81_restoreVal = 0u;
        }
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: VDAC81_Start
********************************************************************************
*
* Summary:
*  The start function initializes the voltage DAC with the default values, 
*  and sets the power to the given level.  A power level of 0, is the same as
*  executing the stop function.
*
* Parameters:
*  Power: Sets power level between off (0) and (3) high power
*
* Return:
*  void 
*
* Global variables:
*  VDAC81_initVar: Is modified when this function is called for the 
*  first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void VDAC81_Start(void)  
{
    /* Hardware initiazation only needs to occure the first time */
    if(VDAC81_initVar == 0u)
    { 
        VDAC81_Init();
        VDAC81_initVar = 1u;
    }

    /* Enable power to DAC */
    VDAC81_Enable();

    /* Set default value */
    VDAC81_SetValue(VDAC81_DEFAULT_DATA); 
}


/*******************************************************************************
* Function Name: VDAC81_Stop
********************************************************************************
*
* Summary:
*  Powers down DAC to lowest power state.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void VDAC81_Stop(void) 
{
    /* Disble power to DAC */
    VDAC81_PWRMGR &= (uint8)(~VDAC81_ACT_PWR_EN);
    VDAC81_STBY_PWRMGR &= (uint8)(~VDAC81_STBY_PWR_EN);

    /* This is a work around for PSoC5A  ,
    this sets VDAC to current mode with output off */
    #if (CY_PSOC5A)
        VDAC81_backup.data_value = VDAC81_CR0;
        VDAC81_CR0 = VDAC81_CUR_MODE_OUT_OFF;
        VDAC81_restoreVal = 1u;
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: VDAC81_SetSpeed
********************************************************************************
*
* Summary:
*  Set DAC speed
*
* Parameters:
*  power: Sets speed value
*
* Return:
*  void
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void VDAC81_SetSpeed(uint8 speed) 
{
    /* Clear power mask then write in new value */
    VDAC81_CR0 &= (uint8)(~VDAC81_HS_MASK);
    VDAC81_CR0 |=  (speed & VDAC81_HS_MASK);
}


/*******************************************************************************
* Function Name: VDAC81_SetRange
********************************************************************************
*
* Summary:
*  Set one of three current ranges.
*
* Parameters:
*  Range: Sets one of Three valid ranges.
*
* Return:
*  void 
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void VDAC81_SetRange(uint8 range) 
{
    VDAC81_CR0 &= (uint8)(~VDAC81_RANGE_MASK);      /* Clear existing mode */
    VDAC81_CR0 |= (range & VDAC81_RANGE_MASK);      /*  Set Range  */
    VDAC81_DacTrim();
}


/*******************************************************************************
* Function Name: VDAC81_SetValue
********************************************************************************
*
* Summary:
*  Set 8-bit DAC value
*
* Parameters:  
*  value:  Sets DAC value between 0 and 255.
*
* Return: 
*  void 
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void VDAC81_SetValue(uint8 value) 
{
    #if (CY_PSOC5A)
        uint8 VDAC81_intrStatus = CyEnterCriticalSection();
    #endif /* CY_PSOC5A */

    VDAC81_Data = value;                /*  Set Value  */

    /* PSOC5A requires a double write */
    /* Exit Critical Section */
    #if (CY_PSOC5A)
        VDAC81_Data = value;
        CyExitCriticalSection(VDAC81_intrStatus);
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: VDAC81_DacTrim
********************************************************************************
*
* Summary:
*  Set the trim value for the given range.
*
* Parameters:
*  range:  1V or 4V range.  See constants.
*
* Return:
*  void
*
* Theory: 
*
* Side Effects:
*
*******************************************************************************/
void VDAC81_DacTrim(void) 
{
    uint8 mode;

    mode = (uint8)((VDAC81_CR0 & VDAC81_RANGE_MASK) >> 2) + VDAC81_TRIM_M7_1V_RNG_OFFSET;
    VDAC81_TR = CY_GET_XTND_REG8((uint8 *)(VDAC81_DAC_TRIM_BASE + mode));
}


/* [] END OF FILE */
