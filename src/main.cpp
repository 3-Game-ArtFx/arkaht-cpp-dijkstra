#include <string>

#include "raylib.h"

#include "int2.h"
#include "dijkstra/pathfinder.h"

static const Int2 SCREEN_SIZE { 1280, 768 };
static const float TICK_TIME = 0.1f;
static const int GRID_SIZE = 7;
static const int TILE_SIZE = SCREEN_SIZE.y / GRID_SIZE;

bool grid[GRID_SIZE][GRID_SIZE] 
{
	{ 0, 1, 1, 1, 1, 1, 1 },
	{ 0, 1, 0, 1, 0, 0, 1 },
	{ 1, 1, 1, 1, 0, 1, 1 },
	{ 1, 1, 0, 1, 0, 1, 1 },
	{ 0, 1, 0, 1, 1, 1, 0 },
	{ 0, 1, 0, 0, 0, 1, 1 },
	{ 0, 1, 1, 1, 1, 1, 1 },
};
dijkstra::Pathfinder* pathfinder;
float current_tick_time = TICK_TIME;

int pos_to_grid_id( Int2 pos )
{
	return pos.x + GRID_SIZE * pos.y;
}

void init()
{
	pathfinder = new dijkstra::Pathfinder( GRID_SIZE );

	dijkstra::Node* start = pathfinder->get_node_at( 1, 0 );
	dijkstra::Node* end = pathfinder->get_node_at( GRID_SIZE - 1, GRID_SIZE - 1 );
	//pathfinder->begin( start, end );
}

void update( float dt )
{
	if ( ( current_tick_time -= dt ) <= 0.0f )
	{
		pathfinder->update();
		current_tick_time = TICK_TIME;
	}
}

void draw()
{
	//  draw grid
	for ( int y = 0; y < GRID_SIZE; y++ ) 
		for ( int x = 0; x < GRID_SIZE; x++ )
		{
			//if ( !grid[y][x] ) continue;
			Color color = RAYWHITE;

			dijkstra::Node* node = pathfinder->get_node_at( x, y );
			if ( node != nullptr )
			{
				switch ( node->state )
				{
				case dijkstra::NodeState::CLOSED:
					color = RED;
					break;
				case dijkstra::NodeState::OPENED:
					color = MAGENTA;
					break;
				}
			}

			DrawRectangle( x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, color );
		}

	DrawFPS( 10, 10 );
}

void dispose()
{
	delete pathfinder;
}



int main( void )
{
	InitWindow( SCREEN_SIZE.x, SCREEN_SIZE.y, "pathfinder" );
	SetTargetFPS( 60 );

	init();
	
	//  game loop
	while ( !WindowShouldClose() )
	{
		update( GetFrameTime() );

		BeginDrawing();
		ClearBackground( BLACK );

		draw();
		EndDrawing();
	}

	dispose();

	CloseWindow();
	return 0;
}