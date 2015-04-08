#include "Grid.h"
#include "GraphicsSystem.h"
#include "Vector2D.h"
#include <memory.h>
#include "Defines.h"
#include "../editor/MapDefines.h"

Grid::Grid( int pixelWidth, int pixelHeight, int squareSize )
:mPixelWidth(pixelWidth)
,mPixelHeight(pixelHeight)
,mSquareSize(squareSize)
{
	mNumCandy = mNumGrues = mNumPortals = 0;
	mHasPlayer = false;
	mGridWidth = pixelWidth / squareSize;
	mGridHeight = pixelHeight / squareSize;
	mpValues = new int[ mGridWidth * mGridHeight ];
	memset( mpValues, 0, sizeof(int) * mGridWidth * mGridHeight  );
}

Grid::~Grid()
{
	delete [] mpValues;
}

int Grid::getSquareIndexFromPixelXY( int x, int y ) const
{
	x /= mSquareSize;
	y /= mSquareSize;
	return y * mGridWidth + x;
}

int Grid::getValueAtIndex( int index ) const
{
	return *(mpValues + index);
}

int Grid::getValueAtPixelXY( int x, int y ) const
{
	int index = getSquareIndexFromPixelXY( x, y );
	return getValueAtIndex( index );
}
	
void Grid::setValueAtIndex( int index, int value )
{
	*(mpValues + index) = value;
}

void Grid::setValueAtPixelXY( int x, int y, int value )
{
	int index = getSquareIndexFromPixelXY( x, y );
	setValueAtIndex( index, value );
}

Vector2D Grid::getULCornerOfSquare( int index ) const
{
	int squareY = index / mGridWidth;
	int squareX = index % mGridWidth;
	Vector2D pos( (float)(squareX * mSquareSize), (float)(squareY * mSquareSize) );
	return pos;
}

//get adjacent grid square indices
std::vector<int> Grid::getAdjacentIndices( int theSquareIndex ) const
{
	std::vector<int> indices;

	static const int NUM_DIRS = 8;
	//						        N 	E  	S	 W
	static int xMods[NUM_DIRS] = {	0,	1,	0,	-1};
	static int yMods[NUM_DIRS] = { -1,	0,	1,	 0};

	//find the x,y of the passed in index
	int x = theSquareIndex % mGridWidth;
	int y = theSquareIndex / mGridWidth;

	for( int i=0; i<NUM_DIRS; i++ )
	{
		//calc adjacent x,y
		int adjX = x + xMods[i];
		int adjY = y + yMods[i];

		//convert back to an index if on the Grid
		if( adjX >= 0 && adjX < mGridWidth && adjY >=0 && adjY < mGridHeight )
		{
			int adjIndex = ( adjY * mGridWidth ) + adjX;

			//add to vector of indices
			indices.push_back( adjIndex );
		}
	}
	return indices;
}


void Grid::save( std::ofstream& file )
{
	file.write((char*)&mGridWidth, sizeof(int));
	file.write((char*)&mGridHeight, sizeof(int));
	file.write((char*)&mSquareSize, sizeof(int));
	int numSquares = mGridWidth * mGridHeight;
	for( int i=0; i<numSquares; i++ )
	{
		file.write((char*)&mpValues[i], sizeof(int));
	}
}

void Grid::load( std::ifstream& file )
{
	mNumPortals = 0;
	file.read((char*)&mGridWidth, sizeof(int));
	file.read((char*)&mGridHeight, sizeof(int));
	file.read((char*)&mSquareSize, sizeof(int));
	int numSquares = mGridWidth * mGridHeight;

	delete[] mpValues;
	mpValues = new int[numSquares];

	for( int i=0; i<numSquares; i++ )
	{
		file.read((char*)&mpValues[i], sizeof(int));

		if (mpValues[i] >= MIN_PORTAL_ID)//found a portal
		{
			mNumPortals++;
		}
		else if (mpValues[i] == PLAYER_SPAWN_ID)
		{
			mHasPlayer = true;
		}
		else if (mpValues[i] == GRUE_SPAWN)
		{
			mNumGrues++;
		}
		else if (mpValues[i] == MIGHTY_CANDY_ID)
		{
			mNumCandy++;
		}
	}
}

int Grid::getRandomEmpty()
{
	int index;
	int	value = WALL_BLOCK_ID;
	while (value >= WALL_BLOCK_ID)
	{
		int xValue = rand() % (getGridWidth() * getSquareSize());
		int yValue = rand() % (getGridHeight() * getSquareSize());
		value = getValueAtPixelXY(xValue, yValue);
		if (value != WALL_BLOCK_ID && value < MIN_PORTAL_ID)
		{
			index = getSquareIndexFromPixelXY(xValue, yValue);
			break;
		}
	}
	return index;
}