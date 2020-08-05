/*******************************************************************************
* File Name: VDAC8_PM.c
* Version 2.10
*
* Description:
*  This file provides the power management source code to the API for the
*  DVDAC component.
*
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "VDAC8.h"
#include "VDAC8_VDAC8.h"

static VDAC8_BACKUP_STRUCT  VDAC8_backup;


/*******************************************************************************
* Function Name: VDAC8_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. The
*  VDAC8_Sleep() API saves the current component state. Then it
*  calls the VDAC8_Stop() function and calls
*  VDAC8_SaveConfig() to save the hardware configuration. Call the
*  VDAC8_Sleep() function before calling the CyPmSleep() or the
*  CyPmHibernate() function.
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
void VDAC8_Sleep(void) 
{
    /* Save VDAC8's enable state */
    if(0u != (VDAC8_VDAC8_PWRMGR & VDAC8_VDAC8_ACT_PWR_EN))
    {
        VDAC8_backup.enableState = 1u;
    }
    else
    {
        VDAC8_backup.enableState = 0u;
    }

    VDAC8_Stop();
    VDAC8_SaveConfig();
}


/*******************************************************************************
* Function Name: VDAC8_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when
*  VDAC8_Sleep() was called. The VDAC8_Wakeup() function
*  calls the VDAC8_RestoreConfig() function to restore the
*  configuration. If the component was enabled before the
*  VDAC8_Sleep() function was called, the DVDAC_Wakeup() function
*  will also re-enable the component.
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
void VDAC8_Wakeup(void) 
{
    VDAC8_RestoreConfig();

    if(VDAC8_backup.enableState == 1u)
    {
        VDAC8_Enable();
    }
}


/*******************************************************************************
* Function Name: VDAC8_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component configuration and non-retention registers.
*  This function is called by the VDAC8_Sleep() function.
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
void VDAC8_SaveConfig(void) 
{
    VDAC8_VDAC8_SaveConfig();
}


/*******************************************************************************
* Function Name: VDAC8_RestoreConfig
********************************************************************************
*
* Summary:
*  This function restores the component configuration and non-retention
*  registers. This function is called by the VDAC8_Wakeup() function.
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
void VDAC8_RestoreConfig(void) 
{
    VDAC8_VDAC8_RestoreConfig();
}


/* [] END OF FILE */
