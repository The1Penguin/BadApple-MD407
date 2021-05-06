/*
 * 	object.h
 *
 */
#ifndef OBJECT_H
#define OBJECT_H

#define MAX_POINTS 25

typedef struct {
	unsigned char x, y;
} POINT;

typedef struct {
	int numpoints;
	int sizex;
	int sizey;
	POINT px[MAX_POINTS];
} GEOMETRY, *PGEOMETRY;

typedef struct tObj{
	PGEOMETRY geo;
	int dirx, diry;
	int posx, posy;
	int maxSpeed;
	int score;
	void (*draw)(struct tObj*);
	void (*clear)(struct tObj*);
	void (*move)(struct tObj*);
	void (*set_speed)(struct tObj*, int, int);
} OBJECT, *POBJECT;

extern OBJECT player1Object;
extern OBJECT player2Object;
extern OBJECT AI1Object;

int xcheckCollison(POBJECT o);
int ycheckCollison(POBJECT o);

void draw_objectSim(POBJECT o);		//SIMULATOR

#endif // OBJECT_H