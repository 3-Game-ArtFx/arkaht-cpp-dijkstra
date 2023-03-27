#include <iostream>

#include "dijkstra.h"

int main()
{
	//  init
	dijkstra::Node a( "A" ), b( "B" ), c( "C" ), d( "D" ), e( "E" ), f( "F" ), g( "G" );

	//  linking
	a.connect_to( { &b, 10 } );
	a.connect_to( { &e, 30 }, true );
	a.connect_to( { &c, 15 }, true );

	b.connect_to( { &f, 57 } );

	c.connect_to( { &g, 52 } );
	c.connect_to( { &d, 16 } );

	d.connect_to( { &c, 13 } );

	f.connect_to( { &b, 49 } );
	f.connect_to( { &e, 12 } );

	e.connect_to( { &f, 11 } );
	e.connect_to( { &g, 34 } );

	g.connect_to( { &e, 35 } );
	g.connect_to( { &c, 63 } );

	//  show A' connections
	a.print_connections();
	printf( "\n" );

	//  find path
	std::vector<dijkstra::Node*> path;
	bool is_success = a.find_path_to( g, &path );
	printf( "path: A -> G: %d\n", is_success );
	for ( auto itr = path.begin(); itr != path.end(); itr++ )
	{
		printf( "- %s\n", (*itr)->name );
	}
}