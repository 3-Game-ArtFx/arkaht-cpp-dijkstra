#pragma once

#include <vector>
#include <queue>
#include <unordered_set>

#define _ALLOC_NODE_CONNECTION( node, cost, previous )  alloc.push_back( new NodeConnection { node, cost, previous } ); \
                                                        open_list.push_back( alloc.back() );

namespace dijkstra
{
	struct Node;  //  forward declaration

	struct NodeConnection
	{
		Node* node { nullptr };
		int cost { 0 };

		NodeConnection* previous { nullptr };
	};

	struct NodeConnectionComparator
	{
		bool operator()( NodeConnection* a, NodeConnection* b ) const
		{
			return a->cost > b->cost;
		}
	};

	struct Node
	{
		const char* name;
		std::vector<NodeConnection> connections;

		Node( const char* name ) : name( name ) {}

		void connect_to( const NodeConnection& connection, bool is_bidirectional = false )
		{
			connections.push_back( connection );

			if ( is_bidirectional )
				connection.node->connect_to( { this, connection.cost } );
		}

		bool find_path_to( const Node& target, std::vector<Node*>* out_path )
		{
			NodeConnectionComparator cmp;

			std::vector<NodeConnection*> alloc;
			std::vector<NodeConnection*> open_list;
			std::unordered_set<Node*> close_list;
			
			_ALLOC_NODE_CONNECTION( this, 0, nullptr );

			while ( !open_list.empty() )
			{
				//  find prioritized node
				std::pop_heap( open_list.begin(), open_list.end(), cmp );
				NodeConnection* current_connection = open_list.back();
				open_list.pop_back();

				printf( "- %s\n", current_connection->node->name );

				//  check target
				if ( current_connection->node == &target )
				{
					printf( "\n: finding path\n" );

					//  find path
					NodeConnection* connection = current_connection;
					while ( connection != nullptr )
					{
						out_path->emplace( out_path->begin(), connection->node );
						printf( ": %s\n", connection->node->name );
						connection = connection->previous;
					}

					break;
				}

				//  mark as visited
				close_list.insert( current_connection->node );

				//  add connections to open list
				for ( auto itr = current_connection->node->connections.begin(); itr != current_connection->node->connections.end(); itr++ )
				{
					NodeConnection* connection = itr._Ptr;

					//  ignore visited nodes
					if ( close_list.find( connection->node ) != close_list.end() ) continue;

					//  add to open list
					_ALLOC_NODE_CONNECTION( connection->node, current_connection->cost + connection->cost, current_connection );
					std::push_heap( open_list.begin(), open_list.end(), cmp );

					printf( "%s: %d\n", connection->node->name, current_connection->cost + connection->cost );
				}
			}

			//  clear allocations
			for ( int i = 0; i < alloc.size(); i++ )
				delete alloc[i];

			return !out_path->empty();
		}

		void print_connections()
		{
			printf( "%s' connections:\n", name );
			for ( auto itr = connections.begin(); itr != connections.end(); itr++ )
			{
				printf( "- %s -> %s for %d\n", name, itr->node->name, itr->cost );
			}
		}
	};
}

#undef _ALLOC_NODE_CONNECTION