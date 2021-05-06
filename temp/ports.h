/*
 * 	ports.h
 *
 */
#ifndef PORTS_H
#define PORTS_H

#include "types.h"

typedef struct _gpio {
	uint32_t moder;
	uint32_t otyper;
	uint32_t ospeedr;
	uint32_t pupdr;
	union {
		uint32_t idr;
		struct {
			uint8_t idrLow;
			uint8_t idrHigh;
		};
	};
	union {
		uint32_t  odr;
		struct {
			uint8_t odrLow;
			uint8_t odrHigh;
		};
	};
} GPIO;
typedef volatile GPIO* gpioptr;

#define GPIO_D (*((gpioptr) 0x40020c00))
#define GPIO_E (*((gpioptr) 0x40021000))

#endif // PORTS_H