#pragma once
#include "node.fwd.h"
#include "node-connection.h"
#include "int2.h"

#include <unordered_set>

namespace dijkstra
{
	enum class NodeState
	{
		NONE,
		OPENED,
		CLOSED,
	};

	struct Node
	{
		NodeState state { NodeState::NONE };

		Int2 pos;
		const char* name;
		std::vector<Node*> connections;

		Node( Int2 pos, const char* name ) : pos( pos ), name( name ) {}

		void connect_to( Node* node, bool is_bidirectional = false )
		{
			if ( std::find( connections.begin(), connections.end(), node ) == connections.end() )
				connections.push_back( node );

			if ( is_bidirectional )
				node->connect_to( this );
		}

		void print_connections()
		{
			printf( "%s' connections:\n", name );
			for ( auto itr = connections.begin(); itr != connections.end(); itr++ )
			{
				printf( "- %s -> %s for %d\n", name, (*itr)->name);
			}
		}
	};
}

