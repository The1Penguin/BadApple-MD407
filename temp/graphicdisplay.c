/*
 * 	graphicdisplay.c
 *
 */

#include "types.h"
#include "ports.h"
#include "delay.h"

//Controlregister bits
#define B_E				0x40	// Enable
#define B_RST			0x20	// Reset
#define B_CS2			0x10	// Controller Select 2
#define B_CS1			0x08	// Controller Select 1
#define B_SELECT		0x04	// 0 Graphics, 1 ASCII
#define B_RW			0x02	// 0 Write, 1 Read
#define B_RS			0x01	// 0 Command, 1 Data

//Commands to display
#define LCD_ON			0x3F	// Display on
#define LCD_OFF			0x3E	// Display off
#define LCD_SET_ADD		0x40	// Set horizontal coordinate
#define LCD_SET_PAGE	0xB8	// Set vertical coordinate
#define LCD_DISP_START	0xC0	// Start address
#define LCD_BUSY		0x80	// Read busy status

//Controlregister
static void graphic_ctrl_bit_set(uint8_t x) {
	uint8_t c;
	c = GPIO_E.odrLow;
	c &= ~B_SELECT;
	c |= ( ~B_SELECT & x );
	GPIO_E.odrLow = c;
}

static void graphic_ctrl_bit_clear(uint8_t x) {
	uint8_t c;
	c = GPIO_E.odrLow;
	c &= ~B_SELECT;
	c &= ~x;
	GPIO_E.odrLow = c;
}

//Select chip
static void select_controller(uint8_t controller) {
	switch(controller) {
		case 0:
			graphic_ctrl_bit_clear(B_CS1);
			graphic_ctrl_bit_clear(B_CS2);
			break;
		case B_CS1:
			graphic_ctrl_bit_set(B_CS1);
			graphic_ctrl_bit_clear(B_CS2);
			break;
		case B_CS2:
			graphic_ctrl_bit_clear(B_CS1);
			graphic_ctrl_bit_set(B_CS2);
			break;
		case (B_CS1 | B_CS2):
			graphic_ctrl_bit_set(B_CS1);
			graphic_ctrl_bit_set(B_CS2);
			break;
	}
}


static void graphic_wait_ready(void) {
	
	uint8_t c;
	graphic_ctrl_bit_clear(B_E);
	GPIO_E.moder = 0x00005555;		// 15-8 input, 7-0 output
	graphic_ctrl_bit_clear(B_RS);
	graphic_ctrl_bit_set(B_RW);
	delay_500ns();
	
	while (1) {
		graphic_ctrl_bit_set(B_E);
		delay_500ns();
		c = GPIO_E.idrHigh & LCD_BUSY;
		graphic_ctrl_bit_clear(B_E);
		delay_500ns();
		if (c == 0) break;
	}
	
	graphic_ctrl_bit_set(B_E);												//?
	GPIO_E.moder = 0x55555555;		// 15-0 output
}

//Read
static uint8_t graphic_read(uint8_t controller) {
	uint8_t c;
	graphic_ctrl_bit_clear(B_E);
	GPIO_E.moder = 0x00005555;		// 15-8 input, 7-0 output
	graphic_ctrl_bit_set(B_RS);
	graphic_ctrl_bit_set(B_RW);
	select_controller(controller);
	delay_500ns();
	graphic_ctrl_bit_set(B_E);
	delay_500ns();
	c = GPIO_E.idrHigh;
	graphic_ctrl_bit_clear(B_E);
	GPIO_E.moder = 0x55555555;		// 15-0 output
	
	if (controller & B_CS1) {
		select_controller(B_CS1);
		graphic_wait_ready();
	} else if (controller & B_CS2) {
		select_controller(B_CS2);
		graphic_wait_ready();
	}
	return c;
}

static uint8_t graphic_read_data(uint8_t controller) {
	graphic_read(controller);
	return graphic_read(controller);
}

//Write
static void graphic_write(uint8_t value, uint8_t controller) {
	GPIO_E.odrHigh = value;
	select_controller(controller);
	delay_500ns();
	graphic_ctrl_bit_set(B_E);
	delay_500ns();
	graphic_ctrl_bit_clear( B_E );
	if(controller & B_CS1) {
		select_controller(B_CS1);
		graphic_wait_ready();
	}
	if(controller & B_CS2) {
		select_controller(B_CS2);
		graphic_wait_ready();
	}
	GPIO_E.odrHigh = 0;														//?
	graphic_ctrl_bit_set(B_E);												//?
	select_controller(0);													//?
}

static void graphic_write_command(uint8_t command, uint8_t controller) {
	graphic_ctrl_bit_clear(B_E);
	select_controller(controller);
	graphic_ctrl_bit_clear(B_RS|B_RW);
	graphic_write(command, controller);
}

static void graphic_write_data(uint8_t data, uint8_t controller) {
	graphic_ctrl_bit_clear(B_E);
	select_controller(controller);
	graphic_ctrl_bit_set(B_RS);
	graphic_ctrl_bit_clear(B_RW);
	graphic_write(data, controller);
}

void init_graphic(void) {
	graphic_ctrl_bit_set(B_E);
	delay_micro(10);
	graphic_ctrl_bit_clear(B_CS1|B_CS2|B_RST|B_E);
	delay_milli(30);
	graphic_ctrl_bit_set(B_RST);
	delay_milli(100);														//?
	graphic_write_command(LCD_OFF,			B_CS1|B_CS2);
	graphic_write_command(LCD_ON,			B_CS1|B_CS2);
	graphic_write_command(LCD_DISP_START,	B_CS1|B_CS2);
	graphic_write_command(LCD_SET_ADD,		B_CS1|B_CS2);
	graphic_write_command(LCD_SET_PAGE,		B_CS1|B_CS2);
	select_controller(0);
}

void graphic_clear_screen(void) {
	uint8_t page, add;
	for(page=0; page<=7; page++) {
		graphic_write_command(LCD_SET_PAGE	| page,		B_CS1|B_CS2);
		graphic_write_command(LCD_SET_ADD	| 0,		B_CS1|B_CS2);
		for(add=0; add<=63; add++) {
			graphic_write_data(0, B_CS1|B_CS2);
		}
	}
}

// Set or clear a single pixel
void pixelSim(int x, int y, int set) {		// SIMULATOR
	uint8_t mask, c, controller;
	int index;
	if((x < 1) || (x > 128) || (y < 1) || (y > 64)) return;
	x-=1;
	y-=1;
	
	index = y/8;
	mask = 1 << (y%8);
	
	if(set == 0)
		mask = ~mask;
	
	if(x > 63) {
		controller = B_CS2;
		x -= 64;
	}else {
		controller = B_CS1;
	}
	graphic_write_command(LCD_SET_ADD	| x,		controller);
	graphic_write_command(LCD_SET_PAGE	| index,	controller);
	c = graphic_read_data(controller);
	graphic_write_command(LCD_SET_ADD	| x,		controller);	// counteracts auto incrementation
	
	if(set==1)
		mask |= c;
	else
		mask &= c;
	
	graphic_write_data(mask, controller);
}

/*-----------------Backbuffer Implementation-------------------*/

uint8_t frameBuffer0[1024], frameBuffer1[1024]; // 1024 = 128 * 64 / 8
uint8_t *backBuffer = frameBuffer0;
uint8_t *frontBuffer = frameBuffer1;

void clearBuffer(void) {
	for (int i = 0; i < 1024; i++)
		backBuffer[i] = 0;
}

void clearBuffers(void) {
	for (int i = 0; i < 1024; i++)
		backBuffer[i] = frontBuffer[i] = 0;
}

void pixel(int x, int y) {
	if((x < 1) || (x > 128) || (y < 1) || (y > 64))
		return;
	x-=1;
	y-=1;
	
	uint8_t mask = 1 << (y%8);
	
	int index = 0;
	
	if(x >= 64) {
		x -= 64;
		index = 512;
	}
	index += x + (y/8)*64;
	
	backBuffer[index] |= mask;
}

int read_pixel(int x, int y) {
	uint8_t mask;
	int index = 0;
	if((x < 1) || (x > 128) || (y < 1) || (y > 64)) return;
	x-=1;
	y-=1;
	
	mask = 1 << (y%8);
	
	if(x > 63) {
		x -= 64;
		index = 512;
	}
	
	index += x + (y/8)*64;
	
	if ( (backBuffer[index] & mask) == 0)
		return 0;
	else
		return 1;
}

static void graphic_draw_screen(void) {
	uint8_t i, j, controller, c;
	bool bUpdateAddr = true;
	unsigned int k = 0;
	for(c = 0; c < 2; c++) {							// loop over both controllers (the two displays)
		controller = (c == 0) ? B_CS1 : B_CS2;
		for(j = 0; j < 8; j++) {						// loop over pages
			graphic_write_command(LCD_SET_PAGE | j, controller);
			graphic_write_command(LCD_SET_ADD | 0, controller);
			for(i = 0; i <= 63; i++, k++) {				// loop over addresses
				// update display only where it is different from last frame
				if(backBuffer[k] != frontBuffer[k]) {
					if(bUpdateAddr)
						graphic_write_command(LCD_SET_ADD | i, controller);
					graphic_write_data(backBuffer[k], controller);
					bUpdateAddr = false;
				} else {
					bUpdateAddr = true;
				}
			}
		}
	}
}

void swapBuffers(void) {
	graphic_draw_screen();
	unsigned char* tmp = frontBuffer; // swap front/backbuffers
	frontBuffer = backBuffer;
	backBuffer = tmp;
}


/*--------------------------------------------------------*/