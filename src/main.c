/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 * @defgroup uart_example_main main.c
 * @{
 * @ingroup uart_example
 * @brief UART Example Application main file.
 *
 * This file contains the source code for a sample application using UART.
 * 
 */


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// Autocompletion of nrf specific symbols and eldoc work much better when those files are included on top level
#ifdef NRF51
#include "core_cm0.h"
#include "nrf51.h"
#endif


#ifdef NRF52
#include "core_cm4.h"
#include "nrf52.h"
#endif


#include "nrf.h"


#include "bsp.h"


#include "app_uart.h"
#include "app_error.h"
#include "nrf_delay.h"

#include "nrf_rtc.h"
#include "nrf_gpio.h"







#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 1                           /**< UART RX buffer size. */




#ifdef PRINTF_USES_UART
int _write(int file, char *ptr, int len)
{

    int i=0;
    uint8_t cr;
    for(i=0 ; i<len ; i++) {
        cr = *ptr++;
        while(app_uart_put(cr) != NRF_SUCCESS);
    }
    return len;
}
#endif




void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}



void RTC0_IRQHandler(void) {
    printf("RTC irq handler called !");
}

int main(void)
{
    uint32_t err_code;
    const app_uart_comm_params_t comm_params =
      {
          RX_PIN_NUMBER,
          TX_PIN_NUMBER,
          RTS_PIN_NUMBER,
          CTS_PIN_NUMBER,
          APP_UART_FLOW_CONTROL_ENABLED,
          false,
          UART_BAUDRATE_BAUDRATE_Baud38400
      };

    APP_UART_FIFO_INIT(&comm_params,
                         UART_RX_BUF_SIZE,
                         UART_TX_BUF_SIZE,
                         uart_error_handle,
                         APP_IRQ_PRIORITY_LOW,
                         err_code);

    APP_ERROR_CHECK(err_code);

    printf("Hello world ! (%s)\n",__DATE__);

    
    
    //int i =0;
    // while(1) {
    //     
    //     nrf_delay_ms(1000);
    //}

    // RTC0 init

    // Irq setup
    NVIC_SetPriority(RTC0_IRQn, APP_IRQ_PRIORITY_LOW);
    NVIC_ClearPendingIRQ(RTC0_IRQn);
    NVIC_EnableIRQ(RTC0_IRQn);
    
    int i = Reset_IRQn;
    printf("i=%d",i);

    nrf_delay_ms(1000);
    nrf_rtc_counter_get(NRF_RTC0);
    
    
}




/** @} */
