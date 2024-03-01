/**
 * @Author:	Carmine
 * @Date:	02/01/2022
 * @Name:	blasteroids.c
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include "asteroid.h"
#include "blast.h"
#include "blasteroids.h"
#include "spaceship.h"

// TODO: Add explosions effect

void assertInit(bool value, const char *name) {
	if (value) return;

	fprintf(stderr, "Couldn't initialize %s\n", name);
	exit(1);
}

void initDisplay(ALLEGRO_DISPLAY **display) {
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

	*display = al_create_display(WIDTH, HEIGHT);
	al_set_window_title(*display, "Blasteroids - by Carmine Colucci");
	assertInit(display, "display");
}

ALLEGRO_SAMPLE *sample_shot;
ALLEGRO_SAMPLE *sample_explode[2];

void initAudio(void) {
	assertInit(al_install_audio(), "audio");
	assertInit(al_init_acodec_addon(), "audio codec");
	al_reserve_samples(16);
	sample_shot = al_load_sample("..\\res\\shot.flac");
	assertInit(sample_shot, "shot sample");
	sample_explode[0] = al_load_sample("..\\res\\explode1.flac");
	assertInit(sample_explode[0], "explode1 sample");
	sample_explode[1] = al_load_sample("..\\res\\explode2.flac");
	assertInit(sample_explode[1], "explode2 sample");
}

void destroyAudio(void) {
	al_destroy_sample(sample_shot);
	al_destroy_sample(sample_explode[0]);
	al_destroy_sample(sample_explode[1]);
}

#define KEY_PRESSED 1
#define KEY_RELEASED 2
unsigned char keys[ALLEGRO_KEY_MAX] = {0};

void updateKeyboard(ALLEGRO_EVENT *event) {
	switch (event->type) {
		case ALLEGRO_EVENT_TIMER:
			for (int i = 0; i < ALLEGRO_KEY_MAX; ++i)
				keys[i] &= KEY_PRESSED;
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			keys[event->keyboard.keycode] = KEY_PRESSED | KEY_RELEASED;
			break;
		case ALLEGRO_EVENT_KEY_UP:
			keys[event->keyboard.keycode] &= KEY_RELEASED;
			break;
	}
}

long score_display = 0;

void updateHud(long frames, long score) {
	if (frames % 2)
		return;

	long diff;
	for (long i = 5; i >= 0; --i) {
		diff = 1 << i;
		if (score_display <= score - diff) {
			score_display += diff;
		}
	}
}

void drawHud(Spaceship *s, ALLEGRO_FONT *font) {
	al_draw_textf(font, al_map_rgb_f(1.0, 1.0, 1.0), 5, 5, 0, "Score: %09li", score_display);

	ALLEGRO_TRANSFORM transform;
	al_identity_transform(&transform);
#define SPACING 30
	for (int i = 0; i < s->lives; ++i) {
		al_translate_transform(&transform, 30 + i * SPACING, 50);
		al_use_transform(&transform);
		al_draw_line(-8, 9, 0, -11, s->color, 3.0f);
		al_draw_line(0, -11, 8, 9, s->color, 3.0f);
		al_draw_line(-6, 4, -1, 4, s->color, 3.0f);
		al_draw_line(6, 4, 1, 4, s->color, 3.0f);
		al_identity_transform(&transform);
		al_use_transform(&transform);
	}
	if (s->gone)
		al_draw_text(font, al_map_rgb_f(1.0, 1.0, 1.0), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "GAME OVER!");
}

void createBlast(Blast blasts[MAX_BLASTS], float x, float y, float heading) {
	Blast *b;
	for (int i = 0; i < MAX_BLASTS; ++i) {
		b = blasts + i;
		if (b->gone) {
			al_play_sample(sample_shot, 0.3, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			b->sx = x;
			b->sy = y;
			b->gone = false;
			b->heading = heading;
			b->speed = SPACESHIP_MAX_SPEED * 3;
			return;
		}
	}
}

long collideAsteroidsBlasts(Asteroid asteroids[MAX_ASTEROIDS], Blast blasts[MAX_BLASTS]) {
	long score = 0;
	Asteroid a = {};
#define SPLIT_ANGLE 15.0
	for (int i = 0; i < MAX_ASTEROIDS; ++i) {
		for (int j = 0; j < MAX_BLASTS; ++j) {
			if (collideAsteroid(asteroids + i, blasts + j)) {
				al_play_sample(sample_explode[0], 0.75, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				score += 100;
				blasts[j].gone = true;	// NOTE: Maybe add powerups?
				if (asteroids[i].scale == 2) {
					// Split in two, add a small asteroid
					if (countAlive(asteroids) < MAX_ASTEROIDS) {
						a.sx = asteroids[i].sx;
						a.sy = asteroids[i].sy;
						a.heading = asteroids[i].heading + SPLIT_ANGLE;
						a.twist = randomBetween(0, 360);
						a.speed = asteroids[i].speed;
						a.rot_velocity = asteroids[i].rot_velocity;
						a.scale = 1;
						a.gone = false;
						a.color = al_map_rgb_f(1.0, 1.0, 1.0);
						addAsteroid(asteroids, &a);
					}
					asteroids[i].scale = 1;
					asteroids[i].heading -= SPLIT_ANGLE;
				} else {
					asteroids[i].gone = true;
				}
			}
		}
	}
	return score;
}

void collideSpaceshipAsteroids(Spaceship *s, Asteroid asteroids[MAX_ASTEROIDS]) {
	if (s->invincible_timer)
		--s->invincible_timer;
	else {
		for (int i = 0; i < MAX_ASTEROIDS; ++i) {
			if (collideSpaceship(s, asteroids + i)) {
				al_play_sample(sample_explode[1], 0.75, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				--s->lives;
				s->invincible_timer = REFRESH_HZ * 5;
				if (s->lives == 0) {
					s->gone = true;
				}
				break;
			}
		}
	}
}

int main(void) {
	assertInit(al_init(), "allegro");
	assertInit(al_install_keyboard(), "keyboard");

	ALLEGRO_TIMER *timer = al_create_timer(FPS);
	assertInit(timer, "timer");

	ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
	assertInit(queue, "queue");

	ALLEGRO_DISPLAY *display = NULL;
	initDisplay(&display);

	initAudio();

	assertInit(al_init_font_addon(), "font addon");
	assertInit(al_init_ttf_addon(), "ttf addon");

	assertInit(al_init_primitives_addon(), "primitives");

	ALLEGRO_FONT *font = al_load_font("..\\res\\NineteenNinetySeven.ttf", 24, 0);
	assertInit(font, "font");

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	Spaceship spaceship = {};
	initSpaceship(&spaceship, WIDTH / 2, HEIGHT / 2);
	Blast blasts[MAX_BLASTS] = {};
	initBlasts(blasts);

	Asteroid asteroids[MAX_ASTEROIDS] = {};
	initAsteroids(asteroids);

	ALLEGRO_EVENT event;

	srand(time(NULL));
	bool run = true;
	bool redraw = true;
	long frames = 0;
	long score = 0;
	al_start_timer(timer);
	while (run) {
		al_wait_for_event(queue, &event);

		switch (event.type) {
			case ALLEGRO_EVENT_TIMER:
				if (frames % (REFRESH_HZ * 2) && rand() % 100 > 95) {
					createAsteroid(asteroids);
				}

				updateBlasts(blasts);
				updateAsteroids(asteroids);
				score += collideAsteroidsBlasts(asteroids, blasts);
				updateSpaceship(&spaceship, keys);
				collideSpaceshipAsteroids(&spaceship, asteroids);
				updateHud(frames, score);
				++frames;
				redraw = true;
				break;
			case ALLEGRO_EVENT_KEY_DOWN:
				if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
					if (!spaceship.gone) {
						createBlast(blasts, spaceship.sx, spaceship.sy, spaceship.heading);
					}
				}
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				run = false;
				break;
		}

		if (!run)
			break;

		updateKeyboard(&event);

		if (redraw && al_is_event_queue_empty(queue)) {
			al_clear_to_color(al_map_rgb(0, 0, 0));

			drawAsteroids(asteroids);
			drawBlasts(blasts);
			drawSpaceship(&spaceship);
			drawHud(&spaceship, font);

			al_flip_display();
			redraw = false;
		}
	}

	al_destroy_display(display);
	destroyAudio();
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	al_destroy_font(font);
	return 0;
}
