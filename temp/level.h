/*
 * 	level.h
 *
 */
#ifndef LEVEL_H
#define LEVEL_H

#include "object.h"

void chooseLevel(int level);
void chooseDot(int r);
void drawDot(void);
void drawLevel(void);
void drawLevelSelectScreen(void);

void drawDotSim(void);				//SIMULATOR
void drawLevelSim(void);			//SIMULATOR
void drawStartScreenSim(void);		//SIMULATOR

#endif // LEVEL_H