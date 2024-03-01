/**
 * @Author:	Carmine
 * @Date:	02/01/2022
 * @Name:	asteroids.c
 */

#include <math.h>

#include <allegro5/allegro_primitives.h>

#include "asteroid.h"
#include "blasteroids.h"

// float asteroid_shape[][2] = {
//     {-20, 18},
//     {-20, 15},
//     {-18, 5},
// {-20, -13},
// {-12, -15},
// {-10, -20},
// {5, -20},
// {8, -16},
// {20, -14},
// {20, -5},
// {16, 0},
// {20, 10},
// {15, 20},
// {10, 20},
// {0, 16},
// {-15, 20},
// {-20, 18}
// };

void initAsteroids(Asteroid asteroids[MAX_ASTEROIDS]) {
	for (int i = 0; i < MAX_ASTEROIDS; ++i) {
		asteroids[i].gone = true;
		asteroids[i].color = al_map_rgb_f(1.0, 1.0, 1.0);
	}
}

void createAsteroid(Asteroid asteroids[MAX_ASTEROIDS]) {
	Asteroid *a;
	// Create only MAX_ASTEROIDS/2, so they can split in two
	// TODO: Spawn off screen to avoid spawn-kills
	for (int i = 0; i < MAX_ASTEROIDS / 2; ++i) {
		a = asteroids + i;
		if (a->gone) {
			// puts("Asteroid");
#if 1
			if (rand() % 2) {
				a->sx = randomBetween(-ASTEROID_W, WIDTH);
				a->sy = rand() % 2 ? -ASTEROID_H : HEIGHT;
			} else {
				a->sx = rand() % 2 ? -ASTEROID_W : WIDTH;
				a->sy = randomBetween(-ASTEROID_H, HEIGHT);
			}
#else
			a->sx = randomBetween(0, WIDTH - ASTEROID_W);
			a->sy = randomBetween(0, HEIGHT - ASTEROID_H);
#endif
			a->heading = randomBetween(0, 360);
			a->speed = randomBetween(1, 4);
			a->scale = randomBetween(1, 3);
			a->twist = randomBetween(0, 360);
			a->rot_velocity = randomBetween(-3, 3);
			a->gone = false;
			return;
		}
	}
}

void drawAsteroids(Asteroid asteroids[MAX_ASTEROIDS]) {
	ALLEGRO_TRANSFORM transform;
	al_identity_transform(&transform);
	Asteroid *a;
	for (int i = 0; i < MAX_ASTEROIDS; ++i) {
		a = asteroids + i;
		if (a->gone)
			continue;

		al_scale_transform(&transform, a->scale, a->scale);
		al_rotate_transform(&transform, DEG2RAD(a->twist));
		al_translate_transform(&transform, a->sx, a->sy);
		al_use_transform(&transform);
		al_draw_line(-20, 18, -20, 12, a->color, 2.0f);
		al_draw_line(-20, 12, -15, 5, a->color, 2.0f);
		al_draw_line(-15, 5, -20, -13, a->color, 2.0f);
		al_draw_line(-20, -13, -12, -15, a->color, 2.0f);
		al_draw_line(-12, -15, -10, -20, a->color, 2.0f);
		al_draw_line(-10, -20, 5, -20, a->color, 2.0f);
		al_draw_line(5, -20, 8, -16, a->color, 2.0f);
		al_draw_line(8, -16, 18, -14, a->color, 2.0f);
		al_draw_line(18, -14, 20, -5, a->color, 2.0f);
		al_draw_line(20, -5, 14, 0, a->color, 2.0f);
		al_draw_line(14, 0, 20, 10, a->color, 2.0f);
		al_draw_line(20, 10, 15, 20, a->color, 2.0f);
		al_draw_line(15, 20, 10, 20, a->color, 2.0f);
		al_draw_line(10, 20, 0, 16, a->color, 2.0f);
		al_draw_line(0, 16, -15, 20, a->color, 2.0f);
		al_draw_line(-15, 20, -20, 18, a->color, 2.0f);
#if DRAW_HITBOX
		al_draw_rectangle(-ASTEROID_W / 2, -ASTEROID_H / 2, ASTEROID_W / 2, ASTEROID_H / 2, al_map_rgb_f(1.0, 0, 0), 1.0);
#endif
		al_identity_transform(&transform);
		al_use_transform(&transform);
	}
}

void updateAsteroids(Asteroid asteroids[MAX_ASTEROIDS]) {
	Asteroid *a;
	for (int i = 0; i < MAX_ASTEROIDS; ++i) {
		a = asteroids + i;
		if (a->gone)
			continue;

		if (a->sx <= 0 - ASTEROID_W)
			a->sx += WIDTH + ASTEROID_W;
		if (a->sy <= 0 - ASTEROID_H)
			a->sy += HEIGHT + ASTEROID_H;
		if (a->sx >= WIDTH)
			a->sx -= WIDTH;
		if (a->sy >= HEIGHT)
			a->sy -= HEIGHT;

		a->sx += a->speed * sin(DEG2RAD(a->heading));
		a->sy -= a->speed * cos(DEG2RAD(a->heading));
		a->twist += a->rot_velocity;
	}
}

bool collideAsteroid(Asteroid *a, Blast *b) {
	// Point-Rect collision should be accurate enough
	if (a->gone || b->gone)
		return false;

	if (b->sx > a->sx + ASTEROID_W / 2 * a->scale) return false;
	if (b->sx < a->sx - ASTEROID_W / 2 * a->scale) return false;
	if (b->sy - BLAST_LEN > a->sy + ASTEROID_H / 2 * a->scale) return false;
	if (b->sy < a->sy - ASTEROID_H / 2 * a->scale) return false;

	return true;
	// if (b->sx >= a->sx - ASTEROID_W/2 && b->sx <= a->sx + ASTEROID_W/ 2 &&
	//     b->sy >= a->sy - ASTEROID_H/2 && b->sy <= a->sy + ASTEROID_H/ 2)
	//      return true;
}

int countAlive(Asteroid asteroids[MAX_ASTEROIDS]) {
	int num = 0;
	for (int i = 0; i < MAX_ASTEROIDS; ++i) {
		if (!asteroids[i].gone)
			++num;
	}
	return num;
}

void addAsteroid(Asteroid asteroids[MAX_ASTEROIDS], Asteroid *a) {
	for (int i = 0; i < MAX_ASTEROIDS; ++i) {
		if (asteroids[i].gone) {
			asteroids[i] = *a;
			return;
		}
	}
}
