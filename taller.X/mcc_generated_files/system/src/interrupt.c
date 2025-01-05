/**
 * Interrupt Manager Generated Driver File
 *
 * @file interrupt.c
 * 
 * @ingroup interrupt 
 * 
 * @brief This file contains the API implementation for the Interrupt Manager driver.
 * 
 * @version Interrupt Manager Driver Version 2.0.6
*/

/*
© [2025] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#include "../../system/interrupt.h"
#include "../../system/system.h"
#include "../pins.h"

void (*INT_InterruptHandler)(void);

void  INTERRUPT_Initialize (void)
{
    // Clear the interrupt flag
    // Set the external interrupt edge detect
    EXT_INT_InterruptFlagClear();   
    EXT_INT_risingEdgeSet();
    // Set Default Interrupt Handler
    INT_SetInterruptHandler(INT_DefaultInterruptHandler);
    // EXT_INT_InterruptEnable();

}

/**
 * @ingroup interrupt
 * @brief Services the Interrupt Service Routines (ISR) of enabled interrupts and is called every time an interrupt is triggered.
 * @pre Interrupt Manager is initialized.
 * @param None.
 * @return None.
 */
uint8_t led_state[4] = {0, 0, 0, 0};  // 0 = OFF, 1 = ON

void __interrupt() INTERRUPT_InterruptManager (void)
{
    // interrupt handler
    if(INTCONbits.PEIE == 1)
    {
        if(PIE3bits.TXIE == 1 && PIR3bits.TXIF == 1)
        {
            EUSART_TxInterruptHandler();
        } 
        else if(PIE3bits.RCIE == 1 && PIR3bits.RCIF == 1)
        {
            
            uint8_t received_char;

    // Leer el carácter recibido
    received_char = RCREG;  // El valor del dato recibido

    // Accionar según el carácter recibido
    switch (received_char) {
        case 'a':
            led_state[0] = !led_state[0];  // Alternar el estado del LED 1
            LATAbits.LATA0 = led_state[0];  // Controlar el LED físico (ejemplo en el puerto A0)
            break;
        case 'b':
            led_state[1] = !led_state[1];  // Alternar el estado del LED 2
            LATAbits.LATA1 = led_state[1];  // Controlar el LED físico (ejemplo en el puerto A1)
            break;
        case 'c':
            led_state[2] = !led_state[2];  // Alternar el estado del LED 3
            LATAbits.LATA2 = led_state[2];  // Controlar el LED físico (ejemplo en el puerto A2)
            break;
        case 'd':
            led_state[3] = !led_state[3];  // Alternar el estado del LED 4
            LATAbits.LATA3 = led_state[3];  // Controlar el LED físico (ejemplo en el puerto A3)
            break;
        default:
            // Si no es uno de los caracteres esperados, no hacer nada
            break;
    }

    // Limpiar la bandera de interrupción de recepción
    PIR3bits.RCIF = 0;
            
            
            
            EUSART_RxInterruptHandler();
        } 
        else if(PIE1bits.ADIE == 1 && PIR1bits.ADIF == 1)
        {
            ADC_ISR();
        } 
        else if(PIE1bits.ADTIE == 1 && PIR1bits.ADTIF == 1)
        {
            ADC_ThresholdISR();
        } 
        else
        {
            //Unhandled Interrupt
        }
    }      
    else
    {
        //Unhandled Interrupt
    }
}

void INT_ISR(void)
{
    EXT_INT_InterruptFlagClear();

    // Callback function gets called everytime this ISR executes
    INT_CallBack();    
}


void INT_CallBack(void)
{
    // Add your custom callback code here
    if(INT_InterruptHandler)
    {
        INT_InterruptHandler();
    }
}

void INT_SetInterruptHandler(void (* InterruptHandler)(void)){
    INT_InterruptHandler = InterruptHandler;
}

void INT_DefaultInterruptHandler(void){
    // add your INT interrupt custom code
    // or set custom function using INT_SetInterruptHandler()
}
/**
 End of File
*/