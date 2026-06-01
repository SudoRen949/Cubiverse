#pragma once

#ifndef _VOX2_TYPES_H_
#define _VOX2_TYPES_H_

class Types
{
	public:

		enum Blocks
		{
			AIR = 0,
			GRASS,
			DIRT,
			STONE,
			COBBLE,
			LOG,
			GLASS,
			WATER,
			LAVA,
			BEDROCK,
			LEAF,
			TALLGRASS,
			ROSE,
			SAND,

			BlockCount
		};

		/* Block Type */

		static bool checkOpaque(int block)
		{
			if (checkTransparent(block) ||
				checkBlend(block)) return false;
			return true;
		}

		static bool checkTransparent(int block)
		{
			if (block == GLASS ||
				checkBush(block)) return true;
			return false;
		}

		static bool checkBlend(int block)
		{
			if (block == WATER ||
				block == LAVA) return true;
			return false;
		}

		static bool checkNotOpaque(int block)
		{
			if (checkTransparent(block) ||
				checkBlend(block)) return true;
			return false;
		}

		/* Classifications of Blocks (USED FOR COLLISIONS) */

		static bool checkSolids(int block)
		{
			if (block == AIR ||
				checkLiquids(block) ||
				checkBush(block)) return false;
			return true;
		}

		static bool checkLiquids(int block)
		{
			if (block == WATER ||
				block == LAVA) return true;
			return false;
		}

		/* OTHER TYPES OF BLOCKS */

		static bool checkBush(int block)
		{
			if (block == TALLGRASS ||
				block == ROSE) return true;
			return false;
		}

		static bool checkSands(int block)
		{
			if (block == SAND) return true;
			return false;
		}
};

#endif /* _VOX2_TYPES_H_ */
