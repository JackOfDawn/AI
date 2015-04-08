#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "GridPathfinder.h"
#include "GridGraph.h"
#include "GridVisualizer.h"
#include "Path.h"
#include "Game.h"
#include "Grid.h"
#include "GraphicsBuffer.h"

int GridPathfinder::CurrentColor = 0;
bool GridPathfinder::DrawVisited = false;

GridPathfinder::GridPathfinder( GridGraph* pGraph )
:Pathfinder(pGraph)
,mTimeElapsed(0.0)
{
#ifdef VISUALIZE_PATH
	mpVisualizer = NULL;
	mpPath = NULL;
	mPathColor = al_map_rgb(255, 255, 255);
#endif
}

GridPathfinder::~GridPathfinder()
{
#ifdef VISUALIZE_PATH
	delete mpVisualizer;
#endif
}

#ifdef VISUALIZE_PATH
int GridPathfinder::getPathSize()
{
	return mpPath->getNumNodes();
}

void GridPathfinder::drawVisualization( Grid* pGrid, GraphicsBuffer* pDest )
{
	delete mpVisualizer;
	mpVisualizer = new GridVisualizer( pGrid );


	mPathColor = al_map_rgb(255, 0, 0);
	
	
	static ALLEGRO_COLOR visitedColor = al_map_rgb( 0, 128, 0 );
	static ALLEGRO_COLOR startColor = al_map_rgb( 0, 0, 255 );
	static ALLEGRO_COLOR stopColor = al_map_rgb(0, 255, 0);
	static ALLEGRO_COLOR lineColor = al_map_rgb(1, 1, 1);
	static ALLEGRO_COLOR forwardColor = al_map_rgb(0, 0, 125);
	static ALLEGRO_COLOR reverseColor = al_map_rgb(0, 125, 0);

	if( mpPath != NULL && mpPath->getNumNodes() != 0)
	{
		if (DrawVisited)
		{
			for each (auto i in mVisitedNodes)
			{
				mpVisualizer->addColor(i->getId(), visitedColor);
			}
			mpVisualizer->draw(*pDest);
			delete mpVisualizer;
			mpVisualizer = new GridVisualizer(pGrid);
		}

		mpVisualizer->addColor(mpPath->peekNode(0)->getId(), startColor);
		mpVisualizer->addColor(mpPath->peekNode(mpPath->getNumNodes() - 1)->getId(), stopColor);

		for (int i = 1; i < mpPath->getNumNodes() - 1; i++)
		{
			if(!mReverse)
			{
				mpVisualizer->addColor(mpPath->peekNode(i)->getId(), forwardColor);
			}
			else
			{
				mpVisualizer->addColor(mpPath->peekNode(i)->getId(), reverseColor);
			}
		}


		


	}

	mpVisualizer->draw(*pDest);

	//put the drawing here
	if (mpPath != NULL)
	{
		for (int i = 0; i < mpPath->getNumNodes() - 1; i++)
		{
			Vector2D centerPos = pGrid->getULCornerOfSquare(mpPath->peekNode(i)->getId());
			centerPos.setXY(centerPos.getX() + pGrid->getSquareSize() / 2, centerPos.getY() + pGrid->getSquareSize() / 2);
			
			Vector2D centerPos2 = pGrid->getULCornerOfSquare(mpPath->peekNode(i + 1)->getId());
			centerPos2.setXY(centerPos2.getX() + pGrid->getSquareSize() / 2, centerPos2.getY() + pGrid->getSquareSize() / 2);

			al_draw_line(centerPos.getX(), centerPos.getY(), centerPos2.getX(), centerPos2.getY(), lineColor, 2);
		}
	}


}

void GridPathfinder::switchDirection(bool reverse)
{
	mReverse = reverse;
}
void GridPathfinder::clearPath()
{
	delete mpPath;
	mpPath = NULL;
}
#endif