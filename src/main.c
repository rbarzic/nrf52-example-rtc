// Simple RTC code that will toggle a pin every second

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "bsp.h"

#include "nrf_delay.h"

#include "nrf_clock.h"
#include "nrf_rtc.h"
#include "nrf_gpio.h"

// Autocompletion of nrf specific symbols and eldoc work much better when those
// files are included on top level
// This is not heeded if your code is using only SDK code and not accessing
// CMSIS stuff directly
#ifdef NRF51
#include "nrf51.h"
#include "core_cm0.h"
#endif

#ifdef NRF52
#include "nrf52.h"
#include "core_cm4.h"
#endif

#define PIN_LED 19 // LED1   

static unsigned irq_counter = 0;

void RTC0_IRQHandler(void) {
    nrf_rtc_event_clear(NRF_RTC0,NRF_RTC_EVENT_TICK);
    irq_counter++;
    // every 8 IRQs, we toggle the led -> 1Hz rate
    if(irq_counter %8 == 0) nrf_gpio_pin_toggle(PIN_LED); 
}

int main(void)
{


    nrf_gpio_cfg_output(PIN_LED);
    nrf_gpio_pin_set(PIN_LED); // LED is off

    
    // Irq setup
    NVIC_SetPriority(RTC0_IRQn, 15); // Lowes priority
    NVIC_ClearPendingIRQ(RTC0_IRQn);
    NVIC_EnableIRQ(RTC0_IRQn);

    // Start LFCLK clock
    nrf_clock_lf_src_set(NRF_CLOCK_LF_SRC_RC); // 32KHz RC
    nrf_clock_task_trigger(NRF_CLOCK_TASK_LFCLKSTART);


    // Set prescaler to the max value (12-bit)
    // -> 8Hz counter frequency
    nrf_rtc_prescaler_set(NRF_RTC0,(1<<12) -1);
    nrf_rtc_event_enable(NRF_RTC0, NRF_RTC_EVENT_TICK);
    nrf_rtc_int_enable(NRF_RTC0,NRF_RTC_INT_TICK_MASK);
    nrf_rtc_task_trigger(NRF_RTC0,NRF_RTC_TASK_START);

    while(1);
}
