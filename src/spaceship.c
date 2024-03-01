/**
 * @Author:	Carmine
 * @Date:	02/01/2022
 * @Name:	spaceship.c
 */

#include <math.h>
#include <stdio.h>

#include <allegro5/allegro_primitives.h>
#include <allegro5/transformations.h>

#include "asteroid.h"
#include "blasteroids.h"
#include "spaceship.h"

#define ROT_ANGLE 4

void initSpaceship(Spaceship *s, float x, float y) {
	s->sx = x;
	s->sy = y;
	s->speed = 0;
	s->heading = 0;
	s->lives = 3;
	s->color = al_map_rgb_f(0.0, 1.0, 0.0);
}

void drawSpaceship(Spaceship *s) {
	if (s->gone) return;

	if ((s->invincible_timer / 3) % 4 == 1) return;	 // *Blinks* a frame every 12

	ALLEGRO_TRANSFORM transform;
	al_identity_transform(&transform);
	al_rotate_transform(&transform, DEG2RAD(s->heading));
	al_scale_transform(&transform, 1.6, 1.6);
	al_translate_transform(&transform, s->sx, s->sy);
	al_use_transform(&transform);

	al_draw_line(-8, 9, 0, -11, s->color, 2.0f);
	al_draw_line(0, -11, 8, 9, s->color, 2.0f);
	al_draw_line(-6, 4, -1, 4, s->color, 2.0f);
	al_draw_line(6, 4, 1, 4, s->color, 2.0f);
#if DRAW_HITBOX
	al_draw_rectangle(-SPACESHIP_W / 2, -SPACESHIP_H / 2, SPACESHIP_W / 2, SPACESHIP_H / 2, al_map_rgb_f(1.0, 0, 0), 1.0);	// Right
#endif
	al_identity_transform(&transform);
	al_use_transform(&transform);
}

void updateSpaceship(Spaceship *s, unsigned char keys[ALLEGRO_KEY_MAX]) {
	if (s->gone)
		return;

	if (keys[ALLEGRO_KEY_LEFT])
		s->heading -= ROT_ANGLE;
	if (keys[ALLEGRO_KEY_RIGHT])
		s->heading += ROT_ANGLE;
	if (keys[ALLEGRO_KEY_UP])
		s->speed += SPACESHIP_SPEED;
	if (keys[ALLEGRO_KEY_DOWN])
		s->speed -= SPACESHIP_SPEED;

	if (s->speed > SPACESHIP_MAX_SPEED)
		s->speed = SPACESHIP_MAX_SPEED;
	if (s->speed < 0)
		s->speed = 0;

	if (s->sx < 0)
		s->sx += WIDTH;
	if (s->sy < 0)
		s->sy += HEIGHT;
	if (s->sx > WIDTH)
		s->sx -= WIDTH;
	if (s->sy > HEIGHT)
		s->sy -= HEIGHT;

	s->sx += s->speed * sin(DEG2RAD(s->heading));
	s->sy -= s->speed * cos(DEG2RAD(s->heading));
}

bool collideSpaceship(Spaceship *s, Asteroid *a) {
	if (s->gone || a->gone)
		return false;
	// Rect collision
	if (s->sx - SPACESHIP_W / 2 > a->sx + ASTEROID_W / 2 * a->scale) return false;
	if (s->sx + SPACESHIP_W / 2 < a->sx - ASTEROID_W / 2 * a->scale) return false;
	if (s->sy - SPACESHIP_H / 2 > a->sy + ASTEROID_H / 2 * a->scale) return false;
	if (s->sy + SPACESHIP_H / 2 < a->sy - ASTEROID_H / 2 * a->scale) return false;

#if DEBUG
	printf("Spaceship[X1:%f\tY1:%f\tX2:%f\tY2:%f]\nAsteroid[X1:%f\tY1:%f\tX2:%f\tY2:%f]\n",
		   s->sx, s->sy, s->sx + SPACESHIP_W, s->sy + SPACESHIP_H, a->sx, a->sy, a->sx + ASTEROID_W, a->sy + ASTEROID_H);
#endif
	return true;
}
