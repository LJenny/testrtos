/*
 * Handler.c
 *
 *  Created on: Sep 22, 2018
 *      Author: maanu
 */

#include "stdbool.h"
#include "stdint.h"
#include "HalInterrupt.h"

void Irq_Handler(void)
{
    Hal_interrupt_run_handler();
}

void Fiq_Handler(void)
{
    while(true);
}
