/**
 * @Author:	Carmine
 * @Date:	02/01/2022
 * @Name:	blast.c
 */

#include <math.h>

#include <allegro5/allegro_primitives.h>
#include <allegro5/transformations.h>

#include "blast.h"
#include "blasteroids.h"

void initBlasts(Blast blasts[MAX_BLASTS]) {
	for (int i = 0; i < MAX_BLASTS; ++i) {
		blasts[i].gone = true;
		// blasts[i].color = al_map_rgb_f(1.0, 1.0, 1.0); // Keep random color for each frame?
	}
}

void drawBlasts(Blast blasts[MAX_BLASTS]) {
	ALLEGRO_TRANSFORM transform;
	al_identity_transform(&transform);
	Blast *b;
	for (int i = 0; i < MAX_BLASTS; ++i) {
		b = blasts + i;
		if (b->gone)
			continue;

		al_rotate_transform(&transform, DEG2RAD(b->heading));
		al_translate_transform(&transform, b->sx, b->sy);
		al_use_transform(&transform);
		b->color = al_map_rgb(rand() % 255, rand() % 255, rand() % 255);
		al_draw_line(0, 10, 0, 5, b->color, 4.0);
		al_draw_line(0, 0, 0, -5, b->color, 4.0);
		al_draw_line(0, -10, 0, -15, b->color, 4.0);
		al_draw_line(0, -20, 0, -25, b->color, 4.0);
		al_draw_line(0, -30, 0, -35, b->color, 4.0);
		al_identity_transform(&transform);
		al_use_transform(&transform);
	}
}

void updateBlasts(Blast blasts[MAX_BLASTS]) {
	Blast *b;
	for (int i = 0; i < MAX_BLASTS; ++i) {
		b = blasts + i;
		if (b->gone)
			continue;

		if (b->sx < 0 || b->sx > WIDTH || b->sy < 0 || b->sy > HEIGHT) {
			b->gone = true;
			continue;
		}

		b->sx += b->speed * sin(DEG2RAD(b->heading));
		b->sy -= b->speed * cos(DEG2RAD(b->heading));
	}
}
