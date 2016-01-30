/*****************************************************************************
**
**  VERSION:            @(#)stm32303c-eval-blinky.c	1.4 15/07/08
**
**  IN PACKAGE:         ARM
**
**  COPYRIGHT:          Copyright 2013 Altium BV
**
**  DESCRIPTION:        Blinky project for STM32303C-EVAL board,
**                      Night RiderTM style scanner light on the four leds
**                      of this board, the speed is controlled using the on
**                      board potentiometer.
**
*****************************************************************************/

#include <stdint.h>
#include <limits.h>

#include <stm32f30x.h>

uint32_t led_values[] = { 0, 1, 3, 6, 12, 8, 0, 8, 12, 6, 3, 1 };
uint32_t led_index;

uint32_t delay;
uint32_t last_tick;

/* for testing the value range */
uint32_t min_delay = UINT_MAX;
uint32_t max_delay = 0;

/* to have a stoppable endless loop */
volatile uint32_t done;

/* SytTick interrupt handler counter */
volatile uint32_t num_systicks;

void delay10us(void)
{
	volatile int cnt = 720 / 6;
	while (cnt-- != 0);
}

__interrupt(15) void SysTick_Handler(void)
{
	num_systicks++;
}

void Init_SysTick(void)
{
	/* at 72MHz clock a 2msec tick */
	SysTick_Config(144000);
}

void Init_Potentiometer(void)
{
	/* RCC_AHBENR, Bit 28 ADC12EN: ADC1 and ADC2 enable */
	RCC->AHBENR |= (1 << 28);

	/* potentiometer on PC1 */
	/* RCC_AHBENR, Bit 19 IOPCEN: I/O port C clock enable */
	RCC->AHBENR |= (1 << 19);
	/* GPIO port mode register */
	/* GPIO ->MODERy 	11: Analog mode */
	GPIOC->MODER = (GPIOC->MODER & 0xFFFFFFF3) | (3 << 2);
	GPIOC->PUPDR = (GPIOC->PUPDR & 0xFFFFFFF3);

	/* RCC_AHBRSTR, Bit 28 ADC12RST: ADC1 and ADC2 reset */
	/* Enable ADC1 reset state */
//	RCC->AHBRSTR |= (1 << 28);
	/* Release ADC1 from reset state */
//	RCC->AHBRSTR &= ~(1 << 28);

	/* Bits 29:28 ADVREGEN[1:0]: ADC voltage regulator enable */
	/* 00: Intermediate state */
	ADC1->CR = ADC1->CR & 0xcfffffff;
	/* 01: ADC Voltage regulator enabled */
	ADC1->CR = ADC1->CR | (1 << 28);
	/* wait at least 10 usec for voltage regulator o settle */
	delay10us();

	ADC1_2->CCR = (ADC1_2->CCR & 0x00030000) | (1 << 16);

	/* Enable STM32 ADC, Bit 0 ADEN: ADC enable control */
	ADC1->CR |= (1 << 0);

	/* wait for ADRDY flag */
	/* ADCx_ISR, Bit 0 ADRDY: ADC ready */
	while (!(ADC1->ISR & (1 << 0)));

	/* Bits 3:0 L[3:0]: Regular channel sequence length, 0000: 1 conversion (= reset value) */
	/* Bits 10:6 SQ1[4:0]: 1st conversion in regular sequence IN7	*/
	ADC1->SQR1 |= (7 << 6);

	/* STM32 ADC regular channel configuration */
	/* ADC1->SMPR1, SMP7[2:0] 23-21, 100: 19.5 ADC clock cycles */
	ADC1->SMPR1 = (ADC1->SMPR1 & 0xff1fffff) | (4 << 21);

}

void Init_Leds(void)
{
	/* 4 leds LED1, LED2, LED3, LED4 - PE8, PE9, PE10, PE11 */
	/* RCC_AHBENR, Bit 21 IOPEEN: I/O port E clock enable */
	RCC->AHBENR |= (1 << 21);

	/* pins 0123 Open drain pins							*/
	/* MODERy 	01: General purpose output					*/
	/* OTYPERy	 1: Output open drain			 			*/
	/* OSPEEDR	01: 10 MHz Medium speed						*/
	/* PUPDR	00: No pull-up, pull-down					*/

	/* GPIO port mode register */
	GPIOE->MODER = (GPIOC->MODER & 0xFFFFFF00) | (1 << 22) | (1 << 20) | (1 << 18) | (1 << 16);

	/* GPIO port output type register */
	GPIOE->OTYPER = (GPIOC->OTYPER | 0x00000F00);

	/* GPIO port output speed register */
	GPIOE->OSPEEDR = (GPIOC->OSPEEDR & 0xFFFFFF00) | (1 << 22) | (1 << 20) | (1 << 18) | (1 << 16);

	/* GPIO port pull-up/pull-down register */
	GPIOE->PUPDR = (GPIOC->PUPDR & 0xFF00FFFF);
}

void Set_Leds(uint32_t value)
{
	uint32_t bsrr;  /* value for port bit set/reset register	*/
	uint32_t odr;	/* current port output data register		*/

	/* 4 leds LED1, LED2, LED3, LED4 - PE8, PE9, PE10, PE11 */
	/* set or clear bit if current bit value is different form new bit value */
	/* Note LEDs are open drain (0 on, 1 off) */
	bsrr = 0;
	odr = GPIOE->ODR;
	if (value & (1 << 0))
	{
		if (odr & (1 << 8))		/* need to clear bit? */
		{
			bsrr |= (1 << (8 + 16));
		}
	}
	else
	{
		if (!(odr & (1 << 8)))	/* need to set bit? */
		{
			bsrr |= (1 << 8);
		}
	}

	if (value & (1 << 1))
	{
		if (odr & (1 << 9))		/* need to clear bit? */
		{
			bsrr |= (1 << (9 + 16));
		}
	}
	else
	{
		if (!(odr & (1 << 9)))	/* need to set bit? */
		{
			bsrr |= (1 << 9);
		}
	}

	if (value & (1 << 2))
	{
		if (odr & (1 << 10))		/* need to clear bit? */
		{
			bsrr |= (1 << (10 + 16));
		}
	}
	else
	{
		if (!(odr & (1 << 10)))	/* need to set bit? */
		{
			bsrr |= (1 << 10);
		}
	}

	if (value & (1 << 3))
	{
		if (odr & (1 << 11))		/* need to clear bit? */
		{
			bsrr |= (1 << (11 + 16));
		}
	}
	else
	{
		if (!(odr & (1 << 11)))	/* need to set bit? */
		{
			bsrr |= (1 << 11);
		}
	}

	GPIOE->BSRR = bsrr;
}

void Handle_Potentiometer(void)
{
	/* Bit 4 OVR: ADC overrun, clear when it is set */
	if (ADC1->ISR & (1 << 4))
	{
		ADC1->ISR &= (1 << 4);
	}

	/* Bit 2 ADSTART: ADC start of regular conversion */
	ADC1->CR |= (1 << 2);

	/* wait for conversion bit to go zero again */
	while (ADC1->CR & (1 << 2));

	/* data value in bits 0-15 */
	delay = (ADC1->DR) & 0x0000ffff;

	if (delay < min_delay)
	{
		min_delay = delay;
	}
	if (delay > max_delay)
	{
		max_delay = delay;
	}
}

void Handle_Ticks(void)
{
	if (delay != 0)
	{
		if (last_tick + (delay / 8) < num_systicks)
		{
			last_tick = num_systicks;
			Set_Leds(led_values[led_index]);
			led_index++;
			if (led_index >= (sizeof(led_values) / sizeof(led_values[0])))
			{
				led_index = 0;
			}
		}
	}
}

int main(void)
{
	Init_SysTick();
	Init_Potentiometer();
	Init_Leds();

	Set_Leds(15);

	done = 0;
	do
	{
		Handle_Potentiometer();
		Handle_Ticks();
	} while (!done);

	Set_Leds(0);

	return 0;
}
