/*
 * 	object.c
 *
 */
#ifdef SIMULATOR
	#define	MAXSPEED	80
	#define	AIMAXSPEED	20
#else
	#define	MAXSPEED	4
	#define	AIMAXSPEED	2
#endif	
 
#include <math.h>
#include "graphicdisplay.h"
#include "object.h"
#include "math.h"
#include "AI.h"
#include "level.h"

unsigned int rand(void);
extern POINT currentDot;

static void draw_object(POBJECT o){
	for(int i=0; i < o->geo->numpoints; i++){
		pixel(o->posx + o->geo->px[i].x, o->posy + o->geo->px[i].y);
	}
}

static int xcheckDotCollision(POBJECT o) {
	int xmin = o->posx;						// left side coord
	int xmax = o->posx + o->geo->sizex - 1;	// right side coord
	int ymin = o->posy;						// upper side coord
	
	if (o->dirx > 0) {						// if moving right
		for (int i=0; i<4; i++) {
			if(xmax+1 == currentDot.x*5-3+2 && ymin+i == currentDot.y*5-2+2) {
				return 1;
			}
		}
	} else if (o->dirx < 0) {				// if moving left
		for (int i=0; i<4; i++) {
			if(xmin-1 == currentDot.x*5-3+2 && ymin+i == currentDot.y*5-2+2) {
				return 1;
			}
		}
	}
	return 0;
}

static int ycheckDotCollision(POBJECT o) {
	int xmin = o->posx;						// left side coord
	int ymin = o->posy;						// upper side coord
	int ymax = o->posy + o->geo->sizey - 1;	// lower side coord
	
	if (o->diry > 0) {						// if moving right
		for (int i=0; i<4; i++) {
			if(xmin+i == currentDot.x*5-3+2 && ymax+1 == currentDot.y*5-2+2) {
				return 1;
			}
		}
	} else if (o->diry < 0) {				// if moving left
		for (int i=0; i<4; i++) {
			if(xmin+i == currentDot.x*5-3+2 && ymin-1 == currentDot.y*5-2+2) {
				return 1;
			}
		}
	}
	return 0;
}

int xcheckCollison(POBJECT o) {
	int xmin = o->posx;						// left side coord
	int xmax = o->posx + o->geo->sizex - 1;	// right side coord
	int ymin = o->posy;						// upper side coord
	
	if (o->dirx > 0) {						// if moving right
		if ( (xmax + 1) > 128 ) {
			return 1;
		} else {
			for (int i=0; i<4; i++) {
				if(read_pixel(xmax+1, ymin+i)) {
					return 1;
				}
			}
		}
	} else if (o->dirx < 0) {				// if moving left
		if ( (xmin - 1) < 1 ) {
			return 1;
		} else {
			for (int i=0; i<4; i++) {
				if(read_pixel(xmin-1, ymin+i)) {
					return 1;
				}
			}
		}
	}
	return 0;
}

int ycheckCollison(POBJECT o) {
	int ymin = o->posy;						// upper side coord
	int ymax = o->posy + o->geo->sizey - 1;	// lower side coord
	int xmin = o->posx;						// left side coord
	
	if (o->diry > 0) {						// if moving down
		if ( (ymax + 1) > 64 ) {
			return 1;
		} else {
			for (int i=0; i<4; i++) {
				if(read_pixel(xmin+i, ymax+1)) {
					return 1;
				}
			}
		}
	} else if (o->diry < 0) {				// if moving up
		if ( (ymin - 1) < 1 ) {
			return 1;
		} else {
			for (int i=0; i<4; i++) {
				if(read_pixel(xmin+i, ymin-1)) {
					return 1;
				}
			}
		}
	}
	return 0;
}

static void move_player(POBJECT o){
	int xsteps = abs(o->dirx);
	int ysteps = abs(o->diry);
	
	for(int i = 0; i < xsteps; i++)
	{
		if(xcheckDotCollision(o)) {
			o->score++;
			chooseDot(rand());
		}
		if (xcheckCollison(o)) {
			o->set_speed(o, 0, o->diry);
			break;
		}
		
		if (o->dirx > 0)
			o->posx++;
		else if (o->dirx < 0)
			o->posx--;
	}
	
	for(int i = 0; i < ysteps; i++)
	{
		if(ycheckDotCollision(o)) {
			o->score++;
			chooseDot(rand());
		}
		if (ycheckCollison(o)) {
			o->set_speed(o, o->dirx, 0);
			break;
		}
		
		if (o->diry > 0)
			o->posy++;
		else if (o->diry < 0)
			o->posy--;
	}
	
}




static void set_object_speed(POBJECT o, int dirx, int diry){
	o->dirx = dirx;
	o->diry = diry;
}

/*---------------------SIM------------------------*/
void draw_objectSim(POBJECT o){				// SIMULATOR
	for(int i=0; i < o->geo->numpoints; i++){
		pixelSim(o->posx + o->geo->px[i].x, o->posy + o->geo->px[i].y, 1);
	}
}

static void clear_objectSim(POBJECT o){		// SIMULATOR
	for(int i=0; i < o->geo->numpoints; i++){
		pixelSim(o->posx + o->geo->px[i].x, o->posy + o->geo->px[i].y, 0);
	}
}
/*------------------------------------------------*/

static GEOMETRY ball_geometry0 =
{
	25,			// numpoints
	5,5,		// sizex, sizey
	{			// px[]
		{0,0},	{1,0},	{2,0},	{3,0},	{4,0},
		{0,1},	{1,1},	{2,1},	{3,1},	{4,1},
		{0,2},	{1,2},	{2,2},	{3,2},	{4,2},
		{0,3},	{1,3},	{2,3},	{3,3},	{4,3},
		{0,4},	{1,4},	{2,4},	{3,4},	{4,4}
	}
};

static GEOMETRY ball_geometry1 =
{
	21,			// numpoints
	5,5,		// sizex, sizey
	{			// px[]
				{1,0},	{2,0},	{3,0},
		{0,1},	{1,1},	{2,1},	{3,1},	{4,1},
		{0,2},	{1,2},	{2,2},	{3,2},	{4,2},
		{0,3},	{1,3},	{2,3},	{3,3},	{4,3},
				{1,4},	{2,4},	{3,4}
	}
};

static GEOMETRY ball_geometry2 =
{
	16,			// numpoints
	5,5,		// sizex, sizey
	{			// px[]
				{1,0},	{2,0},	{3,0},
		{0,1},	{1,1},			{3,1},	{4,1},
		{0,2},							{4,2},
		{0,3},	{1,3},			{3,3},	{4,3},
				{1,4},	{2,4},	{3,4}
	}
};

static GEOMETRY ball_geometry3 =
{
	12,			// numpoints
	5,5,		// sizex, sizey
	{			// px[]
		{0,0},	{1,0},			{3,0},	{4,0},
		{0,1},							{4,1},
		
		{0,3},							{4,3},
		{0,4},	{1,4},			{3,4},	{4,4}
	}
};

static GEOMETRY ball_geometry4 =
{
	25,			// numpoints
	5,5,		// sizex, sizey
	{			// px[]
						{2,0},	{3,0},
		{0,1},	{1,1},	{2,1},	{3,1},
		{0,2},	{1,2},	{2,2},	{3,2},	{4,2},
				{1,3},	{2,3},	{3,3},	{4,3},
				{1,4},	{2,4}
	}
};

static GEOMETRY ball_geometryAI =
{
	19,			// numpoints
	5,5,		// sizex, sizey
	{			// px[]
				{1,0},	{2,0},	{3,0},
		{0,1},			{2,1},			{4,1},
		{0,2},	{1,2},	{2,2},	{3,2},	{4,2},
		{0,3},	{1,3},	{2,3},	{3,3},	{4,3},
		{0,4},			{2,4},			{4,4}
	}
};
	
OBJECT player1Object =
{
	&ball_geometry1,
	0,0,
	2,33,
	MAXSPEED,
	0,
	draw_object,
	clear_objectSim,
	move_player,
	set_object_speed
};

OBJECT player2Object =
{
	&ball_geometry4,
	0,0,
	122,33,
	MAXSPEED,
	0,
	draw_object,
	clear_objectSim,
	move_player,
	set_object_speed
};

OBJECT AI1Object =
{
	&ball_geometryAI,
	0,0,
	62,58,
	AIMAXSPEED,
	0,
	draw_object,
	clear_objectSim,
	move_AI,
	set_object_speed
};