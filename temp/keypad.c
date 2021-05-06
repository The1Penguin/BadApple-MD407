/*
 * 	keypad.c
 *
 */
#ifndef KEYPAD_C
#define KEYPAD_C

#include "ports.h"

//set one of b4-b7 to 1
static void activateRow(int row) {
	GPIO_D.odrHigh &= ~0xF0;
	GPIO_D.odrHigh |= 0x08<<row;
}

//return the column of the pressed key
static int readColumn(void) {
	for(int i=0; i<=4; i++) {
		if ( (GPIO_D.idrHigh & 0x0F)>>i == 0 ) {
			return i;
		}
	}
	return 0;
}

static int readColumn1_2(void) {
	for(int i=0; i<=2; i++) {
		if ( (GPIO_D.idrHigh & 0x03)>>i == 0 ) {
			return i;
		}
	}
	return 0;
}

static int readColumn3_4(void) {
	for(int i=0; i<=4; i++) {
		if ( (GPIO_D.idrHigh & 0x0C)>>i == 0 ) {
			return i;
		}
	}
	return 0;
}

// return the keycode of the pressed key
unsigned char keyb(void) {
	for(int row = 1; row<=4; row++) {
		activateRow(row);
		int column = readColumn();
		
		if (column != 0) {
			if (row<4 && column<4) {
				return (row-1)*3+column;
			} else if (column==4) {
				return 9+row;
			} else {
				if (column==1) {
					return 14;
				} else if (column==2) {
					return 0;
				} else {
					return 15;
				}
			}
		}
		
	}
	return 0xFF;
}

// return the keycode of the pressed key from column 1 or 2
unsigned char keyb1_2(void) {
	for(int row = 1; row<=4; row++) {
		activateRow(row);
		int column = readColumn1_2();
		
		if (column != 0) {
			if (row<4 && column<4) {
				return (row-1)*3+column;
			} else if (column==4) {
				return 9+row;
			} else {
				if (column==1) {
					return 14;
				} else if (column==2) {
					return 0;
				} else {
					return 15;
				}
			}
		}
		
	}
	return 0xFF;
}

// return the keycode of the pressed key from column 3 or 4
unsigned char keyb3_4(void) {
	for(int row = 1; row<=4; row++) {
		activateRow(row);
		int column = readColumn3_4();
		
		if (column != 0) {
			if (row<4 && column<4) {
				return (row-1)*3+column;
			} else if (column==4) {
				return 9+row;
			} else {
				if (column==1) {
					return 14;
				} else if (column==2) {
					return 0;
				} else {
					return 15;
				}
			}
		}
		
	}
	return 0xFF;
}

#endif // KEYPAD_C