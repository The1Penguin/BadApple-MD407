/*
 * 	AI.c
 *	
 */

#include "math.h"
#include "object.h"

int player1Eaten = 0;
int player2Eaten = 0;

static int set_AI_speed_left(POBJECT o) {
	o->set_speed(o, -(o->maxSpeed), 0);
	return xcheckCollison(o);
}
static int set_AI_speed_right(POBJECT o) {
	o->set_speed(o, o->maxSpeed, 0);
	return xcheckCollison(o);
}
static int set_AI_speed_up(POBJECT o) {
	o->set_speed(o, 0, -(o->maxSpeed));
	return ycheckCollison(o);
}
static int set_AI_speed_down(POBJECT o) {
	o->set_speed(o, 0, o->maxSpeed);
	return ycheckCollison(o);
}


static void set_speed_AI_prio(POBJECT o, int (*prio1)(POBJECT), int (*prio2)(POBJECT), int (*prio3)(POBJECT), int (*prio4)(POBJECT) ) {
	if (!prio1(o)) return;
	if (!prio2(o)) return;
	if (!prio3(o)) return;
	if (!prio4(o)) return;
}

void chooseAIDirection(POBJECT o, POBJECT p1, POBJECT p2) {
	int p1Distancex = fabs((o->posx + o->geo->sizex/2)-(p1->posx + p1->geo->sizex/2));
	int p1Distancey = fabs((o->posy + o->geo->sizey/2)-(p1->posy + p1->geo->sizey/2));
	int p2Distancex = fabs((o->posx + o->geo->sizex/2)-(p2->posx + p2->geo->sizex/2));
	int p2Distancey = fabs((o->posy + o->geo->sizey/2)-(p2->posy + p2->geo->sizey/2));
	int p1Distance = sqrt(pow(p1Distancex, 2) + pow(p1Distancey, 2));
	int p2Distance = sqrt(pow(p2Distancex, 2) + pow(p2Distancey, 2));
	
	if(p1Distance < p2Distance) {
		if(p1Distancey < p1Distancex) {
			if(p1->posx < o->posx) {
				if(p1->posy < o->posy)
					set_speed_AI_prio(o, set_AI_speed_left, set_AI_speed_up, set_AI_speed_down, set_AI_speed_right);
				else
					set_speed_AI_prio(o, set_AI_speed_left, set_AI_speed_down, set_AI_speed_up, set_AI_speed_right);
			} else {
				if(p1->posy < o->posy)
					set_speed_AI_prio(o, set_AI_speed_right, set_AI_speed_up, set_AI_speed_down, set_AI_speed_left);
				else
					set_speed_AI_prio(o, set_AI_speed_right, set_AI_speed_down, set_AI_speed_up, set_AI_speed_left);
			}
		} else {
			if(p1->posy < o->posy) {
				if(p1->posx < o->posx)
					set_speed_AI_prio(o, set_AI_speed_up, set_AI_speed_left, set_AI_speed_right, set_AI_speed_down);
				else
					set_speed_AI_prio(o, set_AI_speed_up, set_AI_speed_right, set_AI_speed_left, set_AI_speed_down);
			} else {
				if(p1->posx < o->posx)
					set_speed_AI_prio(o, set_AI_speed_down, set_AI_speed_left, set_AI_speed_right, set_AI_speed_up);
				else
					set_speed_AI_prio(o, set_AI_speed_down, set_AI_speed_right, set_AI_speed_left, set_AI_speed_up);
			}
		}
	}
	
	else {
		if(p2Distancey < p2Distancex) {
			if(p2->posx < o->posx) {
				if(p2->posy < o->posy)
					set_speed_AI_prio(o, set_AI_speed_left, set_AI_speed_up, set_AI_speed_down, set_AI_speed_right);
				else
					set_speed_AI_prio(o, set_AI_speed_left, set_AI_speed_down, set_AI_speed_up, set_AI_speed_right);
			} else {
				if(p2->posy < o->posy)
					set_speed_AI_prio(o, set_AI_speed_right, set_AI_speed_up, set_AI_speed_down, set_AI_speed_left);
				else
					set_speed_AI_prio(o, set_AI_speed_right, set_AI_speed_down, set_AI_speed_up, set_AI_speed_left);
			}
		} else {
			if(p2->posy < o->posy) {
				if(p2->posx < o->posx)
					set_speed_AI_prio(o, set_AI_speed_up, set_AI_speed_left, set_AI_speed_right, set_AI_speed_down);
				else
					set_speed_AI_prio(o, set_AI_speed_up, set_AI_speed_right, set_AI_speed_left, set_AI_speed_down);
			} else {
				if(p2->posx < o->posx)
					set_speed_AI_prio(o, set_AI_speed_down, set_AI_speed_left, set_AI_speed_right, set_AI_speed_up);
				else
					set_speed_AI_prio(o, set_AI_speed_down, set_AI_speed_right, set_AI_speed_left, set_AI_speed_up);
			}
		}
	}
	
}

static int xcollisionWithPlayer(POBJECT o, POBJECT player) {
	int ymin = o->posy;						// upper side coord
	int ymax = o->posy + o->geo->sizey - 1;	// lower side coord
	int xmin = o->posx;						// left side coord
	int xmax = o->posx + o->geo->sizex - 1;	// right side coord
	int pymin = player->posy;
	int pymax = player->posy + player->geo->sizey - 1;
	int pxmin = player->posx;
	int pxmax = player->posx + player->geo->sizex - 1;
	
	if( (xmin-1 == pxmax || xmax+1 == pxmin) && (ymin <= pymax && ymax >= pymin) ) {
		return 1;
	}
	return 0;
}

static int ycollisionWithPlayer(POBJECT o, POBJECT player) {
	int ymin = o->posy;						// upper side coord
	int ymax = o->posy + o->geo->sizey - 1;	// lower side coord
	int xmin = o->posx;						// left side coord
	int xmax = o->posx + o->geo->sizex - 1;	// right side coord
	int pymin = player->posy;
	int pymax = player->posy + player->geo->sizey - 1;
	int pxmin = player->posx;
	int pxmax = player->posx + player->geo->sizex - 1;
	
	if( (ymin-1 == pymax || ymax+1 == pymin) && (xmin <= pxmax && xmax >= pxmin) ) {
		return 1;
	}
	return 0;
}

void move_AI(POBJECT o) {
	POBJECT p1 = &player1Object;		// p1 and p2 for catching check
	POBJECT p2 = &player2Object;
	int xsteps = abs(o->dirx);
	int ysteps = abs(o->diry);
	for(int i = 0; i < xsteps; i++)
	{
		if (xcheckCollison(o)) {
			if(xcollisionWithPlayer(o, p1)) {
				player1Eaten = 1;
				p1->geo->numpoints=0;		// hide player
			} else if(xcollisionWithPlayer(o, p2)) {
				player2Eaten = 1;
				p2->geo->numpoints=0;		// hide player
			} else {
				o->set_speed(o, 0, o->diry);
				break;
			}
		}
		
		if (o->dirx > 0)
			o->posx++;
		else if (o->dirx < 0)
			o->posx--;
	}
	
	for(int i = 0; i < ysteps; i++)
	{
		if (ycheckCollison(o)) {
			if(ycollisionWithPlayer(o, p1)) {
				player1Eaten = 1;
				p1->geo->numpoints=0;		// hide player
			} else if(ycollisionWithPlayer(o, p2)) {
				player2Eaten = 1;
				p2->geo->numpoints=0;		// hide player
			} else {
				o->set_speed(o, o->dirx, 0);
				break;
			}
		}
		
		if (o->diry > 0)
			o->posy++;
		else if (o->diry < 0)
			o->posy--;
	}
	
}