/**
 * @Author:	Carmine
 * @Date:	02/01/2022
 * @Name:	asteroids.h
 */

#if !defined(_ASTEROID_H_)
#define _ASTEROID_H_

#include <allegro5/color.h>

#include "blast.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_ASTEROIDS 12
#define ASTEROID_W 40
#define ASTEROID_H 40

typedef struct asteroid_t {
	float sx, sy;
	float heading;
	float twist;
	float speed;
	float rot_velocity;
	float scale;
	bool gone;
	ALLEGRO_COLOR color;
} Asteroid;

void initAsteroids(Asteroid asteroids[MAX_ASTEROIDS]);
void createAsteroid(Asteroid asteroids[MAX_ASTEROIDS]);
void drawAsteroids(Asteroid asteroids[MAX_ASTEROIDS]);
void updateAsteroids(Asteroid asteroids[MAX_ASTEROIDS]);
bool collideAsteroid(Asteroid *a, Blast *b);
int countAlive(Asteroid asteroids[MAX_ASTEROIDS]);
void addAsteroid(Asteroid asteroids[MAX_ASTEROIDS], Asteroid *a);

#ifdef __cplusplus
}
#endif

#endif /* _ASTEROID_H_ */
