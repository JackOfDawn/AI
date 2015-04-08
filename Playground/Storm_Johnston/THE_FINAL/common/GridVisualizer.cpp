#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include "GridVisualizer.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "Grid.h"
#include "Vector2D.h"
#include <sstream>

GridVisualizer::GridVisualizer( Grid* pGrid )
:mpGrid(pGrid)
,mDirty(true)
{
	mpFont = al_load_ttf_font("cour.ttf", 12, 0);
	if (mpFont == NULL)
	{
		printf("ttf font file not loaded properly!\n");
		assert(0);
	}
}

GridVisualizer::~GridVisualizer()
{
	al_destroy_font(mpFont);
}

void GridVisualizer::refresh()
{
	const ALLEGRO_COLOR& color = BLACK_COLOR; 
	const ALLEGRO_COLOR& portalColor = { 247, 0, 100, 255 };
	const ALLEGRO_COLOR& grueColor = { 0, 0, 240, 255 };
	const ALLEGRO_COLOR& playerColor = { 0, 247, 0, 255 }; 
	const ALLEGRO_COLOR& candyColor = { 247, 100, 0, 255 };


	if( mDirty )
	{
		//remove old entries first
		removeAllEntriesOfColor( color );
		removeAllEntriesOfColor(portalColor);
		removeAllEntriesOfColor(candyColor);
		removeAllEntriesOfColor(playerColor);
		removeAllEntriesOfColor(grueColor);

		int size = mpGrid->getGridWidth() * mpGrid->getGridHeight();
		//get any non-zero squares and send them to the visualizer
		for( int i=0; i<size; i++ )
		{
			if( mpGrid->getValueAtIndex(i) == 1 )
			{
				addColor( i, color );
			}
			else if (mpGrid->getValueAtIndex(i) >= 100)
			{
				addColor(i, portalColor);
			}
			else if (mpGrid->getValueAtIndex(i) == 2) //player spawn
			{
				addColor(i, playerColor);
			}
			else if (mpGrid->getValueAtIndex(i) == 3) //grue spawn
			{
				addColor(i, grueColor);
			}
			else if (mpGrid->getValueAtIndex(i) == 4) //might candy
			{
				addColor(i, candyColor);
			}
		}
	}
	mDirty = false;
}

void GridVisualizer::addColor( int index, const ALLEGRO_COLOR& color )
{
	//set dirty flag
	mDirty = true;

	//find the existing entry in the map (if it exists)
	std::map< ALLEGRO_COLOR, std::vector<int>, AllegroColorCompare >::iterator iter = mColormap.find( color );

	if( iter != mColormap.end() )//found the existing entry
	{
		iter->second.push_back( index );
	}
	else
	{
		//create a vector to put in the map
		std::vector<int> colorVector;
		colorVector.push_back( index );

		//put the vector into the map
		mColormap[color] = colorVector;
	}
}

void GridVisualizer::removeAllEntriesOfColor( const ALLEGRO_COLOR& color )
{
	//find the existing entry in the map (if it exists)
	std::map< ALLEGRO_COLOR, std::vector<int>, AllegroColorCompare >::iterator iter = mColormap.find( color );
	
	if( iter != mColormap.end() )
	{
		mColormap.erase( iter );
	}
}

void GridVisualizer::draw( GraphicsBuffer& dest )
{
	if( mDirty )
	{
		refresh();
	}

	ALLEGRO_BITMAP* pOldTarget = GraphicsSystem::switchTargetBitmap( dest.getBitmap() );

	int gridHeight = mpGrid->getGridHeight();
	int gridWidth = mpGrid->getGridWidth();
	int numSquares = gridWidth * gridHeight;
	int squareSize = mpGrid->getSquareSize();
	
	std::map< ALLEGRO_COLOR, std::vector<int>, AllegroColorCompare >::iterator iter;
	for( iter = mColormap.begin(); iter != mColormap.end(); ++iter )
	{
		std::vector<int> theIndices = iter->second;

		for( unsigned int i=0; i<theIndices.size(); i++ )
		{
			Vector2D ulPos = mpGrid->getULCornerOfSquare( theIndices[i] );
			al_draw_filled_rectangle( ulPos.getX(), ulPos.getY(), ulPos.getX() + squareSize, ulPos.getY() + squareSize, iter->first );
			if (mpGrid->getValueAtIndex(theIndices[i]) >= 100)
			{
				std::stringstream ss;
				ss <<  (mpGrid->getValueAtIndex(theIndices[i]) % 100 + 1);
				al_draw_text(mpFont, al_map_rgb(255, 255, 255), ulPos.getX(), ulPos.getY(), ALLEGRO_ALIGN_LEFT, ss.str().c_str());
			}
			//mpBuffer->fillRegion( ulPos, Vector2D( ulPos.getX() + squareSize, ulPos.getY() + squareSize ), iter->first );
		}

	}

	GraphicsSystem::switchTargetBitmap( pOldTarget );
}