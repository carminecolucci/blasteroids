/**
 * @Author:	Carmine
 * @Date:	02/01/2022
 * @Name:	spaceship.h
 */

#if !defined(_SPACESHIP_H_)
#define _SPACESHIP_H_

#include <allegro5/color.h>
#include <allegro5/keycodes.h>

#include "asteroid.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPACESHIP_MAX_SPEED 3
#define SPACESHIP_SPEED 0.4
#define SPACESHIP_W 16
#define SPACESHIP_H 20

typedef struct spaceship_t {
	float sx, sy;
	float heading;
	float speed;
	int lives;
	int invincible_timer;
	bool gone;
	ALLEGRO_COLOR color;
} Spaceship;

void initSpaceship(Spaceship *s, float x, float y);
void drawSpaceship(Spaceship *s);
void updateSpaceship(Spaceship *s, unsigned char keys[ALLEGRO_KEY_MAX]);
bool collideSpaceship(Spaceship *s, Asteroid *a);

#ifdef __cplusplus
}
#endif

#endif /* _SPACESHIP_H_ */
