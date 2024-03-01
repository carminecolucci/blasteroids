/**
 * @Author:	Carmine
 * @Date:	02/01/2022
 * @Name:	blasteroids.h
 */

#if !defined(_BLASTEROIDS_H_)
#define _BLASTEROIDS_H_

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include "asteroid.h"
#include "blast.h"
#include "spaceship.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PI 3.141592653589793
#define DEG2RAD(headings) ((headings)*(PI/180))

#define WIDTH 1024
#define HEIGHT 768
#define REFRESH_HZ 60
#define FPS (1.0/REFRESH_HZ)

#define randomBetween(a, b) ((a) + (rand() % ((b) - (a))))
#define DEBUG 0
#define DRAW_HITBOX DEBUG

void assertInit(bool value, const char *name);
void initAudio(void);
void destroyAudio(void);
void updateKeyboard(ALLEGRO_EVENT *event);
void updateHud(long frames, long score);
void drawHud(Spaceship *s, ALLEGRO_FONT *font);
void createBlast(Blast blasts[MAX_BLASTS], float x, float y, float heading);
long collideAsteroidsBlasts(Asteroid asteroids[MAX_ASTEROIDS], Blast blasts[MAX_BLASTS]);
void collideSpaceshipAsteroids(Spaceship *s, Asteroid asteroids[MAX_ASTEROIDS]);

#ifdef __cplusplus
}
#endif

#endif /* _BLASTEROIDS_H_ */
