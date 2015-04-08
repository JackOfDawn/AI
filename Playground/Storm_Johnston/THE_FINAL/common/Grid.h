#pragma once

#include <fstream>
#include <vector>
#include "Trackable.h"

class Vector2D;

const int CLEAR_VALUE = 0;
const int BLOCKING_VALUE = 1;
const int PORTAL_ONE_VALUE = 100;

class Grid:public Trackable
{
public:
	Grid( int pixelWidth, int pixelHeight, int squareSize );
	~Grid();

	int getSquareIndexFromPixelXY( int x, int y ) const;

	int getValueAtIndex( int index ) const;
	int getValueAtPixelXY( int x, int y ) const;
	void setValueAtIndex( int index, int value );
	void setValueAtPixelXY( int x, int y, int value );
	Vector2D getULCornerOfSquare( int index ) const;

	int getRandomEmpty();

	//accessors
	int getPixelWidth() const { return mPixelWidth; };
	int getPixelHeight() const { return mPixelHeight; };
	int getGridWidth() const { return mGridWidth; };
	int getGridHeight() const { return mGridHeight; };
	int getSquareSize() const { return mSquareSize; };
	int getNumPortals() const { return mNumPortals; };
	int getNumGrues() const { return mNumGrues; };
	int getNumCandy() const { return mNumCandy; };
	bool hasPlayer() const { return mHasPlayer; };

	//get adjacent grid square indices
	std::vector<int> getAdjacentIndices( int theSquareIndex ) const;

	void save( std::ofstream& file );
	void load( std::ifstream& file );

private:
	int* mpValues;
	int mPixelWidth;
	int mPixelHeight;
	int mSquareSize;
	int mGridWidth;
	int mGridHeight;
	int mNumPortals;
	int mNumGrues;
	int mNumCandy;
	bool mHasPlayer;
};