#ifndef AI_H
#define AI_H

#include "object.h"

extern int player1Eaten;
extern int player2Eaten;

void move_AI(POBJECT o);
void chooseAIDirection(POBJECT o, POBJECT p1, POBJECT p2);

#endif // AI_H