/**
 * @Author:	Carmine
 * @Date:	02/01/2022
 * @Name:	blast.h
 */

#if !defined(_BLAST_H_)
#define _BLAST_H_

#include <allegro5/color.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_BLASTS 6
#define BLAST_LEN 45

typedef struct blast_t {
	float sx, sy;
	float speed;
	// float dx, dy;
	float heading;
	bool gone;
	ALLEGRO_COLOR color;
} Blast;

void initBlasts(Blast blasts[MAX_BLASTS]);
void drawBlasts(Blast blasts[MAX_BLASTS]);
void updateBlasts(Blast blasts[MAX_BLASTS]);

#ifdef __cplusplus
}
#endif

#endif /* _BLAST_H_ */
