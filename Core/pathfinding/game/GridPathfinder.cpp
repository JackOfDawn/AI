#include <allegro5/allegro.h>
#include "GridPathfinder.h"
#include "GridGraph.h"
#include "GridVisualizer.h"
#include "Path.h"
#include "Game.h"
#include "GraphicsBuffer.h"

GridPathfinder::GridPathfinder( GridGraph* pGraph )
:Pathfinder(pGraph)
,mTimeElapsed(0.0)
{
#ifdef VISUALIZE_PATH
	mpVisualizer = NULL;
	mpPath = NULL;
#endif
}

GridPathfinder::~GridPathfinder()
{
#ifdef VISUALIZE_PATH
	delete mpVisualizer;
#endif
}

#ifdef VISUALIZE_PATH
void GridPathfinder::drawVisualization( Grid* pGrid, GraphicsBuffer* pDest )
{
	delete mpVisualizer;
	mpVisualizer = new GridVisualizer( pGrid );
	static ALLEGRO_COLOR pathColor = al_map_rgb( 0, 129, 128 );
	static ALLEGRO_COLOR visitedColor = al_map_rgb( 0, 128, 0 );
	static ALLEGRO_COLOR startStopColor = al_map_rgb( 1, 255, 128 );

	if( mpPath != NULL )
	{
		for( int i=0; i<mpPath->getNumNodes(); i++ )
		{
			mpVisualizer->addColor( mpPath->peekNode(i)->getId(), pathColor );
		}

		mpVisualizer->addColor( mpPath->peekNode(0)->getId(), startStopColor );
		mpVisualizer->addColor( mpPath->peekNode( mpPath->getNumNodes()-1 )->getId(), startStopColor );
	}

	mpVisualizer->draw(*pDest);
}
#endif