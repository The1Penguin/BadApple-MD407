/*
 * 	level.c
 *
 */
#include "graphicdisplay.h"
#include "object.h"


typedef struct {
	int numpoints;
	int numdots;
	POINT px[100];
	POINT dots[100];
} LEVEL, *PLEVEL;

PLEVEL currentLevel;
POINT currentDot;

static LEVEL level1 = 
{
	32,		// nBlocks
	16,		// nDots
	{		// Blocks
			{1,1}, {2,1}, {14,1}, {15,1}, {25,1}, {1,2}, {9,2}, {18,2}, {4,3}, {12,3},
			{6,4}, {21,4}, {3,5}, {22,5}, {10,6}, {13,6}, {13,7}, {16,7}, {4,8}, {23,8},
			{5,9}, {20,9}, {14,10}, {22,10}, {8,11}, {17,11}, {25,11}, {1,12}, {11,12}, {12,12},
			{24,12}, {25,12}
	},
	{		// Dots
			{2,2}, {19,2}, {7,3}, {23,3}, {11,5}, {21,5}, {9,6}, {12,6}, {14,7}, {17,7},
			{5,8}, {15,8}, {3,10}, {19,10}, {7,11}, {24,11}
	}
};
static LEVEL level2 = {
	46,
	20,
	{
		{1,1}, {10,1}, {14,1}, {19,1}, {20,1}, {21,1}, {25,1}, {5,2}, {10,2}, {21,2},
		{7,3}, {8,3}, {16,3}, {20,4}, {1,5}, {15,5}, {22,5}, {25,5}, {4,6}, {11,6},
		{15,6}, {11,7}, {15,7}, {20,7}, {1,8}, {7,8}, {13,8}, {23,8}, {25,8}, {6,9},
		{7,9}, {19,9}, {21,10}, {22,10}, {2,11}, {3,11}, {9,11}, {10,11}, {17,11}, {5,12},
		{12,12}, {14,12}, {22,12}, {23,12}, {24,12}, {25,12}
	},
	{
		{7,1}, {13,2}, {20,2}, {24,2}, {2,3}, {4,3}, {21,3}, {8,5}, {1,6}, {12,6},
		{18,6}, {13,7}, {23,7}, {6,8}, {14,8}, {24,10}, {12,11}, {1,12}, {7,12}, {21,12}
	}
};
static LEVEL level3 = {
	0,
	2,
	{},
	{
		{15,6}, {11,7}
	}
};
static LEVEL level4 = {
	0,
	4,
	{},
	{
		{1,1}, {25,1}, {1,12}, {25,12}
	}
};

void chooseLevel(int level) {
	switch(level) {
		case 1:
			currentLevel = &level1;
			break;
		case 2:
			currentLevel = &level2;
			break;
		case 3:
			currentLevel = &level3;
			break;
		case 4:
			currentLevel = &level4;
			break;
		default:
			currentLevel = &level1;
			
	}
}

void chooseDot(int r) {
	int dot = r % currentLevel->numdots;
	currentDot = currentLevel->dots[dot];
}

void drawDot(void) {
	int x = currentDot.x;	// x and y in a 25x12 grid
	int y = currentDot.y;
	if((x < 1) || (x > 25) || (y < 1) || (y > 12)) return;
	unsigned int i, j;
	
	// coords for top left corner pixel
	int xmin = x*5 - 3;
	int ymin = y*5 - 2;
	
	pixel(xmin+2, ymin+1);
	pixel(xmin+1, ymin+2);
	pixel(xmin+2, ymin+2);
	pixel(xmin+3, ymin+2);
	pixel(xmin+2, ymin+3);
}

static void drawFrame(void) {			// 4 side borders
	unsigned int i;
	
	for(i=2; i<=63; i++)				// left border
		pixel(1, i);
	for(i=1; i<=127; i++) {				// top border
		pixel(i, 1);
		pixel(i, 2);
	}
	for(i=2; i<=63; i++) {				// right border
		pixel(127, i);
		pixel(128, i);
	}
	for(i=1; i<=127; i++) {				// bottom border
		pixel(i, 63);
		pixel(i, 64);
	}
}

static void drawBlock(int x, int y) {	// x and y in a 25x12 grid, one square = 5x5 pixels
	if((x < 1) || (x > 25) || (y < 1) || (y > 12)) return;
	unsigned int i, j;
	
	// coords for top left corner pixel
	int xmin = x*5 - 3;
	int ymin = y*5 - 2;
	
	for (i=0; i<5; i++) {				// 5x5 block
		for (j=0; j<5; j++) {
			pixel(xmin+i, ymin+j);
		}
	}
}

void drawLevel(void) {
	drawFrame();
	for (int i = 0; i < currentLevel->numpoints; i++)
		drawBlock(currentLevel->px[i].x, currentLevel->px[i].y);
}

static void drawFrameMiniature(int xoff, int yoff) {	// SIMULATOR, 4 miniature side borders
	unsigned int i;
	
	for(i=1+yoff; i<=26+yoff; i++)		// left border
		pixel(1+xoff, i);
	for(i=1+xoff; i<=52+xoff; i++)		// top border
		pixel(i, 1+yoff);
	for(i=1+yoff; i<=26+yoff; i++)		// right border
		pixel(52+xoff, i);
	for(i=1+xoff; i<=52+xoff; i++)		// bottom border
		pixel(i, 26+yoff);
}

static void drawBlockMiniature(int x, int y) {	// x and y in a 64x32 grid, one square = 2x2 pixels
	if((x < 1) || (x > 64) || (y < 1) || (y > 32)) return;
	unsigned int i, j;
	
	// coords for top left corner pixel
	int xmin = x*2 - 1;
	int ymin = y*2 - 1;
	
	for (i=0; i<2; i++) {				// 5x5 block
		for (j=0; j<2; j++) {
			pixel(xmin+i, ymin+j);
		}
	}
}

void drawLevelSelectScreen(void) {
	unsigned int i;
	
	int xoff = 9;
	int yoff = 3;
	drawFrameMiniature(xoff, yoff);
	for (i = 0; i < level1.numpoints; i++)
		drawBlockMiniature(level1.px[i].x+(xoff+1)/2, level1.px[i].y+(yoff+1)/2);
		
	xoff = 67;
	yoff = 3;
	drawFrameMiniature(xoff, yoff);
	for (i = 0; i < level2.numpoints; i++)
		drawBlockMiniature(level2.px[i].x+(xoff+1)/2, level2.px[i].y+(yoff+1)/2);
		
	xoff = 9;
	yoff = 35;
	drawFrameMiniature(xoff, yoff);
	for (i = 0; i < level3.numpoints; i++)
		drawBlockMiniature(level3.px[i].x+(xoff+1)/2, level3.px[i].y+(yoff+1)/2);
		
	xoff = 67;
	yoff = 35;
	drawFrameMiniature(xoff, yoff);
	for (i = 0; i < level4.numpoints; i++)
		drawBlockMiniature(level4.px[i].x+(xoff+1)/2, level4.px[i].y+(yoff+1)/2);
}

/* --------------------SIM-------------------- */
void drawDotSim(void) {
	int x = currentDot.x;	// x and y in a 25x12 grid
	int y = currentDot.y;
	if((x < 1) || (x > 25) || (y < 1) || (y > 12)) return;
	unsigned int i, j;
	
	// coords for top left corner pixel
	int xmin = x*5 - 3;
	int ymin = y*5 - 2;
	
	pixelSim(xmin+2, ymin+1, 1);
	pixelSim(xmin+1, ymin+2, 1);
	pixelSim(xmin+2, ymin+2, 1);
	pixelSim(xmin+3, ymin+2, 1);
	pixelSim(xmin+2, ymin+3, 1);
}

static void drawFrameSim(void) {		// SIMULATOR, 4 side borders
	unsigned int i;
	
	for(i=2; i<=63; i=i+4)				// left border
		pixelSim(1, i, 1);
	for(i=1; i<=127; i=i+4)				// top border
		pixelSim(i, 2, 1);
	for(i=2; i<=63; i=i+4)				// right border
		pixelSim(127, i, 1);
	for(i=1; i<=127; i=i+4)				// bottom border
		pixelSim(i, 63, 1);
}

static void drawBlockSim(int x, int y) {	// SIMULATOR, 25x12 level, one square = 5x5 pixels
	unsigned int i, j;
	if((x < 1) || (x > 25) || (y < 1) || (y > 12)) return;
	
	// coords for top left corner pixel
	int xmin = x*5 - 3;
	int ymin = y*5 - 2;
	
	for (i=0; i<5; i=i+4) {				// 5x5 block
		for (j=0; j<5; j=j+4) {
			pixelSim(xmin+i, ymin+j, 1);
		}
	}
}

void drawLevelSim(void) {				// SIMULATOR
	drawFrameSim();
	for (int i = 0; i < (currentLevel->numpoints); i++)
		drawBlockSim(currentLevel->px[i].x, currentLevel->px[i].y);
}

static void drawFrameMiniatureSim(int xoff, int yoff) {	// SIMULATOR, 4 miniature side borders
	unsigned int i;
	
	for(i=1+yoff; i<=26+yoff; i++)		// left border
		pixelSim(1+xoff, i, 1);
	for(i=1+xoff; i<=52+xoff; i++)		// top border
		pixelSim(i, 1+yoff, 1);
	for(i=1+yoff; i<=26+yoff; i++)		// right border
		pixelSim(52+xoff, i, 1);
	for(i=1+xoff; i<=52+xoff; i++)		// bottom border
		pixelSim(i, 26+yoff, 1);
}

static void drawBlockMiniatureSim(int x, int y) {	// x and y in a 64x32 grid, one square = 2x2 pixels
	if((x < 1) || (x > 64) || (y < 1) || (y > 32)) return;
	unsigned int i, j;
	
	// coords for top left corner pixel
	int xmin = x*2 - 1;
	int ymin = y*2 - 1;
	
	for (i=0; i<2; i++) {				// 5x5 block
		for (j=0; j<2; j++) {
			pixelSim(xmin+i, ymin+j, 1);
		}
	}
}

void drawStartScreenSim(void) {
	unsigned int i;
	
	int xoff = 9;
	int yoff = 3;
	drawFrameMiniatureSim(xoff, yoff);
	for (i = 0; i < level1.numpoints; i++)
		drawBlockMiniatureSim(level1.px[i].x+(xoff+1)/2, level1.px[i].y+(yoff+1)/2);
		
	xoff = 67;
	yoff = 3;
	drawFrameMiniatureSim(xoff, yoff);
	for (i = 0; i < level2.numpoints; i++)
		drawBlockMiniatureSim(level2.px[i].x+(xoff+1)/2, level2.px[i].y+(yoff+1)/2);
		
	xoff = 9;
	yoff = 35;
	drawFrameMiniatureSim(xoff, yoff);
	for (i = 0; i < level3.numpoints; i++)
		drawBlockMiniatureSim(level3.px[i].x+(xoff+1)/2, level3.px[i].y+(yoff+1)/2);
		
	xoff = 67;
	yoff = 35;
	drawFrameMiniatureSim(xoff, yoff);
	for (i = 0; i < level4.numpoints; i++)
		drawBlockMiniatureSim(level4.px[i].x+(xoff+1)/2, level4.px[i].y+(yoff+1)/2);
}