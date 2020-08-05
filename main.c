/*
 * 00_Basic_Rx
 * 
 * The nrf24 is configured as Rx, it will print the received data via UART.
 */

#include <main.h>
#include "project.h"
#include <project.h>
#include <string.h>
#include <stdio.h>

#include "project.h"
#include <stdbool.h>
#include <stdio.h>
    
volatile bool irq_flag = false;
uint16 offset;
char dato;


void LoadFilter (uint8 fltr){

	// Put DFB RAM on the bus
    Filter_RAM_DIR_REG = Filter_RAM_DIR_BUS;

	if (fltr == 1)
    {cymemcpy(Filter_DB_RAM,Filter100_data_b, Filter_DB_RAM_SIZE);
    offset=0;}
	else if (fltr == 2)
    {cymemcpy(Filter_DB_RAM,Filter200_data_b, Filter_DB_RAM_SIZE);
    offset=0;}
	else if (fltr == 3)
    {cymemcpy(Filter_DB_RAM,Filter400_data_b, Filter_DB_RAM_SIZE);
    offset=0;}
	else if (fltr == 4)
    {   cymemcpy(Filter_CSB_RAM,Filter_HP_control, Filter_CSB_RAM_SIZE);
        cymemcpy(Filter_DB_RAM,FilterTP2800_data_b, Filter_DB_RAM_SIZE);
        offset=2047;
        }

    else if (fltr == 5)
    {cymemcpy(Filter_CSB_RAM,Filter_HP_control, Filter_CSB_RAM_SIZE);
    cymemcpy(Filter_DB_RAM,Filter65_data_b, Filter_DB_RAM_SIZE);
    offset=2047;}
    else if (fltr == 6)
    cymemcpy(Filter_DB_RAM,Filter65_1000_data_b, Filter_DB_RAM_SIZE);
    else if (fltr == 7)
    cymemcpy(Filter_DB_RAM,Filter65_1360_data_b, Filter_DB_RAM_SIZE);
			
	// Take DFB RAM off the bus
	Filter_RAM_DIR_REG = Filter_RAM_DIR_DFB;
}
CY_ISR(int_Rx){
    
    dato=UART_GetChar();
    //LCD_ClearDisplay();
    
    switch(dato){
        case '1':
            //LCD_PutChar(dato);
            UART_PutChar(dato);
            LoadFilter(1);
            break;
        case '2':
            //LCD_PrintString("2");
            UART_PutChar(dato);
            LoadFilter(2);
            break;
        case '3':
            //LCD_PrintString("3");
            UART_PutChar(dato);
            LoadFilter(3);
            break;
        case '4':
            //LCD_PrintString("4");
            UART_PutChar(dato);
            LoadFilter(4);
            break;
        case '5':
            //LCD_PrintString("5");
            UART_PutChar(dato);
            LoadFilter(5);
            break;
        
        default:
            break;
    }
}
void print_status(void);

// here we will store the received data
//unsigned char data;
uint8 data;
uint8 dataFinal[2];
// Executed when the IRQ pin triggers an interrupt
CY_ISR_PROTO(IRQ_Handler);

int main(void)
{
    const uint8_t RX_ADDR[5]= {0xBA, 0xAD, 0xC0, 0xFF, 0xEE};
    
    // Set the Handler for the IRQ interrupt
    isr_IRQ_StartEx(IRQ_Handler);
    isr_Rx_StartEx(int_Rx);
    
    CyGlobalIntEnable;
    SPI_Start();
    //UART_Start();
    //Filter_Start();
    Opamp_Start();
    DVDAC_Start();
    nRF24_start();
    nRF24_set_rx_pipe_address(NRF_ADDR_PIPE1, RX_ADDR, 5);
    nRF24_start_listening();
    //LoadFilter(1);
    
    
    while (1) {
        //print_status();
        //UART_PutString("Waiting for data...\r\n");
        
        while(false == irq_flag);
        //print_status();
            
        // Get and clear the flag that caused the IRQ interrupt,
        nrf_irq flag = nRF24_get_irq_flag();
        nRF24_clear_irq_flag(flag);
        
        // get the data from the transmitter
        nRF24_get_rx_payload(dataFinal, 2);
        
        uint16 data2 = (dataFinal[1]<<8) + dataFinal[0]; 
        //Filter_Write16(Filter_CHANNEL_A,data2);
        //DVDAC_SetValue(Filter_Read16(Filter_CHANNEL_A)+offset);
        DVDAC_SetValue(data2);
        
        irq_flag = false;    
    }
}

CY_ISR(IRQ_Handler)
{
    irq_flag = true;
    IRQ_ClearInterrupt();
}

//void print_status(void)
//{
//    char array[10];
//    uint8_t sts = nRF24_get_status();
//    UART_PutString("Status: 0x");
//    sprintf(array, "%02X\r\n", sts);
//    UART_PutString(array);
//}
/* [] END OF FILE */
