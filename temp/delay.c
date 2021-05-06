/*
 * 	delay.c
 *
 */
#define STK				0xE000E010
#define STK_CTRL		((volatile unsigned int *)		(STK))
#define STK_LOAD		((volatile unsigned int *)		(STK+0x4))
#define STK_VAL			((volatile unsigned int *)		(STK+0x8))

void delay_250ns(void) {
	*STK_CTRL = 0;
	*STK_LOAD = ( (168/4) -1 );
	*STK_VAL = 0;
	*STK_CTRL = 5;
	while( (*STK_CTRL & 0x10000)== 0 )
	{}
	*STK_CTRL = 0;
}

void delay_500ns(void) {
	*STK_CTRL = 0;
	*STK_LOAD = ( (168/2) -1 );
	*STK_VAL = 0;
	*STK_CTRL = 5;
	while( (*STK_CTRL & 0x10000)== 0 )
	{}
	*STK_CTRL = 0;
}

void delay_micro(unsigned int us) {
	while(us--) {
		delay_250ns();
		delay_250ns();
		delay_250ns();
		delay_250ns();
	}
}

void delay_milli(unsigned int ms) {
	while(ms--)
	{
#ifdef SIMULATOR
		delay_micro(2);
#else
		delay_micro(1000);
#endif
	}
}