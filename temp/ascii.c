/*
 * 	ascii.c
 *	
 */

#include "types.h"
#include "ports.h"
#include "delay.h"

//Controlregister bits
#define B_E				0x40	// Enable
#define B_SELECT		0x04	// 0 Graphics, 1 ASCII
#define B_RW			0x02	// 0 Write, 1 Read
#define B_RS			0x01	// 0 Command, 1 Data

static void ascii_ctrl_bit_set(unsigned char x) {
	unsigned char c;
	c = GPIO_E.odrLow;
	c |= ( B_SELECT | x );
	GPIO_E.odrLow = c;
}

static void ascii_ctrl_bit_clear(unsigned char x) {
	unsigned char c;
	c = GPIO_E.odrLow;
	c = B_SELECT | ( c & ~x );
	GPIO_E.odrLow = c;
}

static void ascii_write_controller(unsigned char byte) {
	delay_250ns();
	ascii_ctrl_bit_set(B_E);
	GPIO_E.odrHigh = byte;
	delay_250ns();
	ascii_ctrl_bit_clear(B_E);
}

static unsigned char ascii_read_controller(void) {
	unsigned char rv;
	ascii_ctrl_bit_set(B_E);
	delay_250ns(); //min 360ns
	delay_250ns();
	rv = GPIO_E.idrHigh;
	
	*((unsigned char *) 0x40020C15) = rv;
	
	ascii_ctrl_bit_clear(B_E);
	return rv;
}

static void ascii_write_cmd(unsigned char command) {
	ascii_ctrl_bit_clear(B_RS);
	ascii_ctrl_bit_clear(B_RW);
	ascii_write_controller(command);
}

static void ascii_write_data(unsigned char data) {
	ascii_ctrl_bit_set(B_RS);
	ascii_ctrl_bit_clear(B_RW);
	ascii_write_controller(data);
}

static unsigned char ascii_read_status(void) {
	GPIO_E.moder = 0x00005555;
	ascii_ctrl_bit_clear(B_RS);
	ascii_ctrl_bit_set(B_RW);
	unsigned char rv = ascii_read_controller();
	GPIO_E.moder = 0x55555555;
	return rv;
}

static unsigned char ascii_read_data(void) {
	GPIO_E.moder = 0x00005555;
	ascii_ctrl_bit_set(B_RS);
	ascii_ctrl_bit_set(B_RW);
	unsigned char rv = ascii_read_controller();

	GPIO_E.moder = 0x55555555;
	return rv;
}

static void ascii_command(unsigned char command, int usdelay) {
	*((unsigned char *) 0x40020C14) = 0x03;
	while ( (ascii_read_status() & 0x80) == 0x80)
	{}
	*((unsigned char *) 0x40020C14) = 0x0F;
	delay_micro(8);
	ascii_write_cmd(command);
	delay_micro(usdelay);
}

void init_ascii(void) {
	ascii_command(0x38, 39);	//Function Set
	ascii_command(0x0E, 39);	//Display Control
	ascii_command(0x01, 1530);	//Clear Display
	ascii_command(0x06, 39);	//Entry Mode Set
}

static void ascii_gotoxy(int x, int y) {
	int address = x-1;
	if (y == 2) {
		address += 0x40;
	}
	ascii_command(0x80 | address, 39);
}

static void ascii_write_char(unsigned char c) {
	while ( (ascii_read_status() & 0x80) == 0x80)
	{}
	delay_micro(8);
	ascii_write_data(c);
	delay_micro(43);
}

void ascii_clear(void) {
	ascii_command(0x01, 1530);	//Clear Display
}

void ascii_write(char string1[], char string2[]) {
	char *s;
	
	ascii_gotoxy(1,1);
	s = string1;
	while(*s) {
		ascii_write_char(*s++);
	}
	ascii_gotoxy(1,2);
	s = string2;
	while(*s){
		ascii_write_char(*s++);
	}
}