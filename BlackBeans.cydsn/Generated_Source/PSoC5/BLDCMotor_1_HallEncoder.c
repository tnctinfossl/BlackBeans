/*******************************************************************************
* File Name: BLDCMotor_1_HallEncoder.c  
* Version 3.0
*
*  Description:
*     The Counter component consists of a 8, 16, 24 or 32-bit counter with
*     a selectable period between 2 and 2^Width - 1.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "BLDCMotor_1_HallEncoder.h"

uint8 BLDCMotor_1_HallEncoder_initVar = 0u;


/*******************************************************************************
* Function Name: BLDCMotor_1_HallEncoder_Init
********************************************************************************
* Summary:
*     Initialize to the schematic state
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void BLDCMotor_1_HallEncoder_Init(void) 
{
        #if (!BLDCMotor_1_HallEncoder_UsingFixedFunction && !BLDCMotor_1_HallEncoder_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!BLDCMotor_1_HallEncoder_UsingFixedFunction && !BLDCMotor_1_HallEncoder_ControlRegRemoved) */
        
        #if(!BLDCMotor_1_HallEncoder_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 BLDCMotor_1_HallEncoder_interruptState;
        #endif /* (!BLDCMotor_1_HallEncoder_UsingFixedFunction) */
        
        #if (BLDCMotor_1_HallEncoder_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            BLDCMotor_1_HallEncoder_CONTROL &= BLDCMotor_1_HallEncoder_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                BLDCMotor_1_HallEncoder_CONTROL2 &= ((uint8)(~BLDCMotor_1_HallEncoder_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                BLDCMotor_1_HallEncoder_CONTROL3 &= ((uint8)(~BLDCMotor_1_HallEncoder_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (BLDCMotor_1_HallEncoder_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                BLDCMotor_1_HallEncoder_CONTROL |= BLDCMotor_1_HallEncoder_ONESHOT;
            #endif /* (BLDCMotor_1_HallEncoder_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            BLDCMotor_1_HallEncoder_CONTROL2 |= BLDCMotor_1_HallEncoder_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            BLDCMotor_1_HallEncoder_RT1 &= ((uint8)(~BLDCMotor_1_HallEncoder_RT1_MASK));
            BLDCMotor_1_HallEncoder_RT1 |= BLDCMotor_1_HallEncoder_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            BLDCMotor_1_HallEncoder_RT1 &= ((uint8)(~BLDCMotor_1_HallEncoder_SYNCDSI_MASK));
            BLDCMotor_1_HallEncoder_RT1 |= BLDCMotor_1_HallEncoder_SYNCDSI_EN;

        #else
            #if(!BLDCMotor_1_HallEncoder_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = BLDCMotor_1_HallEncoder_CONTROL & ((uint8)(~BLDCMotor_1_HallEncoder_CTRL_CMPMODE_MASK));
            BLDCMotor_1_HallEncoder_CONTROL = ctrl | BLDCMotor_1_HallEncoder_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = BLDCMotor_1_HallEncoder_CONTROL & ((uint8)(~BLDCMotor_1_HallEncoder_CTRL_CAPMODE_MASK));
            
            #if( 0 != BLDCMotor_1_HallEncoder_CAPTURE_MODE_CONF)
                BLDCMotor_1_HallEncoder_CONTROL = ctrl | BLDCMotor_1_HallEncoder_DEFAULT_CAPTURE_MODE;
            #else
                BLDCMotor_1_HallEncoder_CONTROL = ctrl;
            #endif /* 0 != BLDCMotor_1_HallEncoder_CAPTURE_MODE */ 
            
            #endif /* (!BLDCMotor_1_HallEncoder_ControlRegRemoved) */
        #endif /* (BLDCMotor_1_HallEncoder_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!BLDCMotor_1_HallEncoder_UsingFixedFunction)
            BLDCMotor_1_HallEncoder_ClearFIFO();
        #endif /* (!BLDCMotor_1_HallEncoder_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        BLDCMotor_1_HallEncoder_WritePeriod(BLDCMotor_1_HallEncoder_INIT_PERIOD_VALUE);
        #if (!(BLDCMotor_1_HallEncoder_UsingFixedFunction && (CY_PSOC5A)))
            BLDCMotor_1_HallEncoder_WriteCounter(BLDCMotor_1_HallEncoder_INIT_COUNTER_VALUE);
        #endif /* (!(BLDCMotor_1_HallEncoder_UsingFixedFunction && (CY_PSOC5A))) */
        BLDCMotor_1_HallEncoder_SetInterruptMode(BLDCMotor_1_HallEncoder_INIT_INTERRUPTS_MASK);
        
        #if (!BLDCMotor_1_HallEncoder_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)BLDCMotor_1_HallEncoder_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            BLDCMotor_1_HallEncoder_WriteCompare(BLDCMotor_1_HallEncoder_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            BLDCMotor_1_HallEncoder_interruptState = CyEnterCriticalSection();
            
            BLDCMotor_1_HallEncoder_STATUS_AUX_CTRL |= BLDCMotor_1_HallEncoder_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(BLDCMotor_1_HallEncoder_interruptState);
            
        #endif /* (!BLDCMotor_1_HallEncoder_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BLDCMotor_1_HallEncoder_Enable
********************************************************************************
* Summary:
*     Enable the Counter
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: 
*   If the Enable mode is set to Hardware only then this function has no effect 
*   on the operation of the counter.
*
*******************************************************************************/
void BLDCMotor_1_HallEncoder_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (BLDCMotor_1_HallEncoder_UsingFixedFunction)
        BLDCMotor_1_HallEncoder_GLOBAL_ENABLE |= BLDCMotor_1_HallEncoder_BLOCK_EN_MASK;
        BLDCMotor_1_HallEncoder_GLOBAL_STBY_ENABLE |= BLDCMotor_1_HallEncoder_BLOCK_STBY_EN_MASK;
    #endif /* (BLDCMotor_1_HallEncoder_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!BLDCMotor_1_HallEncoder_ControlRegRemoved || BLDCMotor_1_HallEncoder_UsingFixedFunction)
        BLDCMotor_1_HallEncoder_CONTROL |= BLDCMotor_1_HallEncoder_CTRL_ENABLE;                
    #endif /* (!BLDCMotor_1_HallEncoder_ControlRegRemoved || BLDCMotor_1_HallEncoder_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: BLDCMotor_1_HallEncoder_Start
********************************************************************************
* Summary:
*  Enables the counter for operation 
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Global variables:
*  BLDCMotor_1_HallEncoder_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void BLDCMotor_1_HallEncoder_Start(void) 
{
    if(BLDCMotor_1_HallEncoder_initVar == 0u)
    {
        BLDCMotor_1_HallEncoder_Init();
        
        BLDCMotor_1_HallEncoder_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    BLDCMotor_1_HallEncoder_Enable();        
}


/*******************************************************************************
* Function Name: BLDCMotor_1_HallEncoder_Stop
********************************************************************************
* Summary:
* Halts the counter, but does not change any modes or disable interrupts.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the counter.
*
*******************************************************************************/
void BLDCMotor_1_HallEncoder_Stop(void) 
{
    /* Disable Counter */
    #if(!BLDCMotor_1_HallEncoder_ControlRegRemoved || BLDCMotor_1_HallEncoder_UsingFixedFunction)
        BLDCMotor_1_HallEncoder_CONTROL &= ((uint8)(~BLDCMotor_1_HallEncoder_CTRL_ENABLE));        
    #endif /* (!BLDCMotor_1_HallEncoder_ControlRegRemoved || BLDCMotor_1_HallEncoder_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (BLDCMotor_1_HallEncoder_UsingFixedFunction)
        BLDCMotor_1_HallEncoder_GLOBAL_ENABLE &= ((uint8)(~BLDCMotor_1_HallEncoder_BLOCK_EN_MASK));
        BLDCMotor_1_HallEncoder_GLOBAL_STBY_ENABLE &= ((uint8)(~BLDCMotor_1_HallEncoder_BLOCK_STBY_EN_MASK));
    #endif /* (BLDCMotor_1_HallEncoder_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BLDCMotor_1_HallEncoder_SetInterruptMode
********************************************************************************
* Summary:
* Configures which interrupt sources are enabled to generate the final interrupt
*
* Parameters:  
*  InterruptsMask: This parameter is an or'd collection of the status bits
*                   which will be allowed to generate the counters interrupt.   
*
* Return: 
*  void
*
*******************************************************************************/
void BLDCMotor_1_HallEncoder_SetInterruptMode(uint8 interruptsMask) 
{
    BLDCMotor_1_HallEncoder_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: BLDCMotor_1_HallEncoder_ReadStatusRegister
********************************************************************************
* Summary:
*   Reads the status register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the status register
*
* Side Effects:
*   Status register bits may be clear on read. 
*
*******************************************************************************/
uint8   BLDCMotor_1_HallEncoder_ReadStatusRegister(void) 
{
    return BLDCMotor_1_HallEncoder_STATUS;
}


#if(!BLDCMotor_1_HallEncoder_ControlRegRemoved)
/*******************************************************************************
* Function Name: BLDCMotor_1_HallEncoder_ReadControlRegister
********************************************************************************
* Summary:
*   Reads the control register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
uint8   BLDCMotor_1_HallEncoder_ReadControlRegister(void) 
{
    return BLDCMotor_1_HallEncoder_CONTROL;
}


/*******************************************************************************
* Function Name: BLDCMotor_1_HallEncoder_WriteControlRegister
********************************************************************************
* Summary:
*   Sets the bit-field of the control register.  This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
void    BLDCMotor_1_HallEncoder_WriteControlRegister(uint8 control) 
{
    BLDCMotor_1_HallEncoder_CONTROL = control;
}

#endif  /* (!BLDCMotor_1_HallEncoder_ControlRegRemoved) */


#if (!(BLDCMotor_1_HallEncoder_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: BLDCMotor_1_HallEncoder_WriteCounter
********************************************************************************
* Summary:
*   This funtion is used to set the counter to a specific value
*
* Parameters:  
*  counter:  New counter value. 
*
* Return: 
*  void 
*
*******************************************************************************/
void BLDCMotor_1_HallEncoder_WriteCounter(uint8 counter) \
                                   
{
    #if(BLDCMotor_1_HallEncoder_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (BLDCMotor_1_HallEncoder_GLOBAL_ENABLE & BLDCMotor_1_HallEncoder_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        BLDCMotor_1_HallEncoder_GLOBAL_ENABLE |= BLDCMotor_1_HallEncoder_BLOCK_EN_MASK;
        CY_SET_REG16(BLDCMotor_1_HallEncoder_COUNTER_LSB_PTR, (uint16)counter);
        BLDCMotor_1_HallEncoder_GLOBAL_ENABLE &= ((uint8)(~BLDCMotor_1_HallEncoder_BLOCK_EN_MASK));
    #else
        CY_SET_REG8(BLDCMotor_1_HallEncoder_COUNTER_LSB_PTR, counter);
    #endif /* (BLDCMotor_1_HallEncoder_UsingFixedFunction) */
}
#endif /* (!(BLDCMotor_1_HallEncoder_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: BLDCMotor_1_HallEncoder_ReadCounter
********************************************************************************
* Summary:
* Returns the current value of the counter.  It doesn't matter
* if the counter is enabled or running.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint8) The present value of the counter.
*
*******************************************************************************/
uint8 BLDCMotor_1_HallEncoder_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(BLDCMotor_1_HallEncoder_UsingFixedFunction)
		(void)CY_GET_REG16(BLDCMotor_1_HallEncoder_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(BLDCMotor_1_HallEncoder_COUNTER_LSB_PTR_8BIT);
	#endif/* (BLDCMotor_1_HallEncoder_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(BLDCMotor_1_HallEncoder_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(BLDCMotor_1_HallEncoder_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(BLDCMotor_1_HallEncoder_STATICCOUNT_LSB_PTR));
    #endif /* (BLDCMotor_1_HallEncoder_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BLDCMotor_1_HallEncoder_ReadCapture
********************************************************************************
* Summary:
*   This function returns the last value captured.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) Present Capture value.
*
*******************************************************************************/
uint8 BLDCMotor_1_HallEncoder_ReadCapture(void) 
{
    #if(BLDCMotor_1_HallEncoder_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(BLDCMotor_1_HallEncoder_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(BLDCMotor_1_HallEncoder_STATICCOUNT_LSB_PTR));
    #endif /* (BLDCMotor_1_HallEncoder_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BLDCMotor_1_HallEncoder_WritePeriod
********************************************************************************
* Summary:
* Changes the period of the counter.  The new period 
* will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period: (uint8) A value of 0 will result in
*         the counter remaining at zero.  
*
* Return: 
*  void
*
*******************************************************************************/
void BLDCMotor_1_HallEncoder_WritePeriod(uint8 period) 
{
    #if(BLDCMotor_1_HallEncoder_UsingFixedFunction)
        CY_SET_REG16(BLDCMotor_1_HallEncoder_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG8(BLDCMotor_1_HallEncoder_PERIOD_LSB_PTR, period);
    #endif /* (BLDCMotor_1_HallEncoder_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BLDCMotor_1_HallEncoder_ReadPeriod
********************************************************************************
* Summary:
* Reads the current period value without affecting counter operation.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint8) Present period value.
*
*******************************************************************************/
uint8 BLDCMotor_1_HallEncoder_ReadPeriod(void) 
{
    #if(BLDCMotor_1_HallEncoder_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(BLDCMotor_1_HallEncoder_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(BLDCMotor_1_HallEncoder_PERIOD_LSB_PTR));
    #endif /* (BLDCMotor_1_HallEncoder_UsingFixedFunction) */
}


#if (!BLDCMotor_1_HallEncoder_UsingFixedFunction)
/*******************************************************************************
* Function Name: BLDCMotor_1_HallEncoder_WriteCompare
********************************************************************************
* Summary:
* Changes the compare value.  The compare output will 
* reflect the new value on the next UDB clock.  The compare output will be 
* driven high when the present counter value compares true based on the 
* configured compare mode setting. 
*
* Parameters:  
*  Compare:  New compare value. 
*
* Return: 
*  void
*
*******************************************************************************/
void BLDCMotor_1_HallEncoder_WriteCompare(uint8 compare) \
                                   
{
    #if(BLDCMotor_1_HallEncoder_UsingFixedFunction)
        CY_SET_REG16(BLDCMotor_1_HallEncoder_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG8(BLDCMotor_1_HallEncoder_COMPARE_LSB_PTR, compare);
    #endif /* (BLDCMotor_1_HallEncoder_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BLDCMotor_1_HallEncoder_ReadCompare
********************************************************************************
* Summary:
* Returns the compare value.
*
* Parameters:  
*  void:
*
* Return: 
*  (uint8) Present compare value.
*
*******************************************************************************/
uint8 BLDCMotor_1_HallEncoder_ReadCompare(void) 
{
    return (CY_GET_REG8(BLDCMotor_1_HallEncoder_COMPARE_LSB_PTR));
}


#if (BLDCMotor_1_HallEncoder_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: BLDCMotor_1_HallEncoder_SetCompareMode
********************************************************************************
* Summary:
*  Sets the software controlled Compare Mode.
*
* Parameters:
*  compareMode:  Compare Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void BLDCMotor_1_HallEncoder_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    BLDCMotor_1_HallEncoder_CONTROL &= ((uint8)(~BLDCMotor_1_HallEncoder_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    BLDCMotor_1_HallEncoder_CONTROL |= compareMode;
}
#endif  /* (BLDCMotor_1_HallEncoder_COMPARE_MODE_SOFTWARE) */


#if (BLDCMotor_1_HallEncoder_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: BLDCMotor_1_HallEncoder_SetCaptureMode
********************************************************************************
* Summary:
*  Sets the software controlled Capture Mode.
*
* Parameters:
*  captureMode:  Capture Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void BLDCMotor_1_HallEncoder_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    BLDCMotor_1_HallEncoder_CONTROL &= ((uint8)(~BLDCMotor_1_HallEncoder_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    BLDCMotor_1_HallEncoder_CONTROL |= ((uint8)((uint8)captureMode << BLDCMotor_1_HallEncoder_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (BLDCMotor_1_HallEncoder_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: BLDCMotor_1_HallEncoder_ClearFIFO
********************************************************************************
* Summary:
*   This function clears all capture data from the capture FIFO
*
* Parameters:  
*  void:
*
* Return: 
*  None
*
*******************************************************************************/
void BLDCMotor_1_HallEncoder_ClearFIFO(void) 
{

    while(0u != (BLDCMotor_1_HallEncoder_ReadStatusRegister() & BLDCMotor_1_HallEncoder_STATUS_FIFONEMP))
    {
        (void)BLDCMotor_1_HallEncoder_ReadCapture();
    }

}
#endif  /* (!BLDCMotor_1_HallEncoder_UsingFixedFunction) */


/* [] END OF FILE */

