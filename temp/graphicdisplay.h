/*
 * 	graphicdisplay.h
 *
 */
#ifndef GRAPHICDISPLAY_H
#define GRAPHICDISPLAY_H

#include "types.h"

extern uint8_t backBuffer[];

void init_graphic(void);
void graphic_clear_screen(void);
void swapBuffers(void);
void clearBuffer(void);
void clearBuffers(void);
void pixel(int x, int y);
int read_pixel(int x, int y);

void pixelSim(int x, int y, int set);		// SIMULATOR

#endif // GRAPHICDISPLAY_H