#pragma once

#ifndef _VOX2_DEFINES_H_
#define _VOX2_DEFINES_H_ 1

/* world configurations */

#define BLOCK_SIZE 			16
#define CHUNK_SIZE 			16
#define WORLD_SIZE_H 		256 // default = 1536
#define WORLD_SIZE_V 		256
#define MAX_NOISE_HEIGHT 	128
#define SEA_LEVEL           MAX_NOISE_HEIGHT - 5
#define LAVA_LEVEL			MAX_NOISE_HEIGHT - 64

/* defaults */

#define DEFAULT_TICK_SPEED			10
#define DEFAULT_WATER_SWIM_SPEED	0.85f // 1.50 blocks per microsecond
#define DEFAULT_LAVA_SWIM_SPEED		0.50f // 0.50 blocks per microsecond
#define DEFAULT_JUMP_HEIGHT			BLOCK_SIZE + 8

/* speeds */

#define SPEED_NORMAL		8.00f
#define SPEED_ONWATER		4.00f
#define SPEED_ONLAVA		2.00f

/* shadow height */

#define MAX_SHADOW_HEIGHT	16

/* typedefines */

typedef unsigned short UShort;
typedef unsigned int UInt;

#endif /* _VOX2_DEFINES_H_ */
