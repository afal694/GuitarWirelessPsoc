/*******************************************************************************
* File Name: VDAC81_PM.c  
* Version 1.90
*
* Description:
*  This file provides the power management source code to API for the
*  VDAC8.  
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "VDAC81.h"

static VDAC81_backupStruct VDAC81_backup;


/*******************************************************************************
* Function Name: VDAC81_SaveConfig
********************************************************************************
* Summary:
*  Save the current user configuration
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void VDAC81_SaveConfig(void) 
{
    if (!((VDAC81_CR1 & VDAC81_SRC_MASK) == VDAC81_SRC_UDB))
    {
        VDAC81_backup.data_value = VDAC81_Data;
    }
}


/*******************************************************************************
* Function Name: VDAC81_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
*******************************************************************************/
void VDAC81_RestoreConfig(void) 
{
    if (!((VDAC81_CR1 & VDAC81_SRC_MASK) == VDAC81_SRC_UDB))
    {
        if((VDAC81_Strobe & VDAC81_STRB_MASK) == VDAC81_STRB_EN)
        {
            VDAC81_Strobe &= (uint8)(~VDAC81_STRB_MASK);
            VDAC81_Data = VDAC81_backup.data_value;
            VDAC81_Strobe |= VDAC81_STRB_EN;
        }
        else
        {
            VDAC81_Data = VDAC81_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: VDAC81_Sleep
********************************************************************************
* Summary:
*  Stop and Save the user configuration
*
* Parameters:  
*  void:  
*
* Return: 
*  void
*
* Global variables:
*  VDAC81_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void VDAC81_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(VDAC81_ACT_PWR_EN == (VDAC81_PWRMGR & VDAC81_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        VDAC81_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        VDAC81_backup.enableState = 0u;
    }
    
    VDAC81_Stop();
    VDAC81_SaveConfig();
}


/*******************************************************************************
* Function Name: VDAC81_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  VDAC81_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void VDAC81_Wakeup(void) 
{
    VDAC81_RestoreConfig();
    
    if(VDAC81_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        VDAC81_Enable();

        /* Restore the data register */
        VDAC81_SetValue(VDAC81_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
