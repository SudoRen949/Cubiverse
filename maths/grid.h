#pragma once

#ifndef _VOX2_GRID_H_
#define _VOX2_GRID_H_

#include <cstring>
#include <cstdlib>
#include <SFML/System.hpp>

#include "maths.h"
#include "../defines.h"

class Grid
{
	public:

		void createGrid(sf::Vector3i size, UInt bit = 1);
		void destroyGrid();
		void clear(UInt v = 0);
		void setValueAt(sf::Vector3i position, UInt value);
		UInt getValueAt(sf::Vector3i position);
		sf::Vector3i getSize();

	private:

		UInt byteSlice, nBits;
		sf::Vector3i gridSize;
		UInt *dataMap, dataSize;
};

#endif /* _VOX2_GRID_H_ */
