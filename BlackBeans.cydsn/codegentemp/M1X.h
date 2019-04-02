/*******************************************************************************
* File Name: M1X.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_M1X_H) /* Pins M1X_H */
#define CY_PINS_M1X_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "M1X_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 M1X__PORT == 15 && ((M1X__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    M1X_Write(uint8 value);
void    M1X_SetDriveMode(uint8 mode);
uint8   M1X_ReadDataReg(void);
uint8   M1X_Read(void);
void    M1X_SetInterruptMode(uint16 position, uint16 mode);
uint8   M1X_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the M1X_SetDriveMode() function.
     *  @{
     */
        #define M1X_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define M1X_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define M1X_DM_RES_UP          PIN_DM_RES_UP
        #define M1X_DM_RES_DWN         PIN_DM_RES_DWN
        #define M1X_DM_OD_LO           PIN_DM_OD_LO
        #define M1X_DM_OD_HI           PIN_DM_OD_HI
        #define M1X_DM_STRONG          PIN_DM_STRONG
        #define M1X_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define M1X_MASK               M1X__MASK
#define M1X_SHIFT              M1X__SHIFT
#define M1X_WIDTH              1u

/* Interrupt constants */
#if defined(M1X__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in M1X_SetInterruptMode() function.
     *  @{
     */
        #define M1X_INTR_NONE      (uint16)(0x0000u)
        #define M1X_INTR_RISING    (uint16)(0x0001u)
        #define M1X_INTR_FALLING   (uint16)(0x0002u)
        #define M1X_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define M1X_INTR_MASK      (0x01u) 
#endif /* (M1X__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define M1X_PS                     (* (reg8 *) M1X__PS)
/* Data Register */
#define M1X_DR                     (* (reg8 *) M1X__DR)
/* Port Number */
#define M1X_PRT_NUM                (* (reg8 *) M1X__PRT) 
/* Connect to Analog Globals */                                                  
#define M1X_AG                     (* (reg8 *) M1X__AG)                       
/* Analog MUX bux enable */
#define M1X_AMUX                   (* (reg8 *) M1X__AMUX) 
/* Bidirectional Enable */                                                        
#define M1X_BIE                    (* (reg8 *) M1X__BIE)
/* Bit-mask for Aliased Register Access */
#define M1X_BIT_MASK               (* (reg8 *) M1X__BIT_MASK)
/* Bypass Enable */
#define M1X_BYP                    (* (reg8 *) M1X__BYP)
/* Port wide control signals */                                                   
#define M1X_CTL                    (* (reg8 *) M1X__CTL)
/* Drive Modes */
#define M1X_DM0                    (* (reg8 *) M1X__DM0) 
#define M1X_DM1                    (* (reg8 *) M1X__DM1)
#define M1X_DM2                    (* (reg8 *) M1X__DM2) 
/* Input Buffer Disable Override */
#define M1X_INP_DIS                (* (reg8 *) M1X__INP_DIS)
/* LCD Common or Segment Drive */
#define M1X_LCD_COM_SEG            (* (reg8 *) M1X__LCD_COM_SEG)
/* Enable Segment LCD */
#define M1X_LCD_EN                 (* (reg8 *) M1X__LCD_EN)
/* Slew Rate Control */
#define M1X_SLW                    (* (reg8 *) M1X__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define M1X_PRTDSI__CAPS_SEL       (* (reg8 *) M1X__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define M1X_PRTDSI__DBL_SYNC_IN    (* (reg8 *) M1X__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define M1X_PRTDSI__OE_SEL0        (* (reg8 *) M1X__PRTDSI__OE_SEL0) 
#define M1X_PRTDSI__OE_SEL1        (* (reg8 *) M1X__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define M1X_PRTDSI__OUT_SEL0       (* (reg8 *) M1X__PRTDSI__OUT_SEL0) 
#define M1X_PRTDSI__OUT_SEL1       (* (reg8 *) M1X__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define M1X_PRTDSI__SYNC_OUT       (* (reg8 *) M1X__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(M1X__SIO_CFG)
    #define M1X_SIO_HYST_EN        (* (reg8 *) M1X__SIO_HYST_EN)
    #define M1X_SIO_REG_HIFREQ     (* (reg8 *) M1X__SIO_REG_HIFREQ)
    #define M1X_SIO_CFG            (* (reg8 *) M1X__SIO_CFG)
    #define M1X_SIO_DIFF           (* (reg8 *) M1X__SIO_DIFF)
#endif /* (M1X__SIO_CFG) */

/* Interrupt Registers */
#if defined(M1X__INTSTAT)
    #define M1X_INTSTAT            (* (reg8 *) M1X__INTSTAT)
    #define M1X_SNAP               (* (reg8 *) M1X__SNAP)
    
	#define M1X_0_INTTYPE_REG 		(* (reg8 *) M1X__0__INTTYPE)
#endif /* (M1X__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_M1X_H */


/* [] END OF FILE */
