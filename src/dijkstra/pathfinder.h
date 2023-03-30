#pragma once

#include "node.h"
#include "dist.h"

#include "raylib.h"

#include <vector>
#include <queue>
#include <unordered_map>
#include <string>

namespace dijkstra
{
	class Pathfinder
	{
	private:
		NodeConnectionComparator cmp;
		std::vector<NodeConnection*> alloc;
		std::vector<NodeConnection*> open_list;
		std::unordered_map<int, Node*> nodes;
		std::unordered_set<Node*> close_list;

		Node* start{ nullptr };
		Node* end{ nullptr };

		std::vector<Node*> out_path;

		int grid_size;

		void _open_connection( NodeConnection* connection, bool should_sort = false )
		{
			alloc.push_back( connection );
			open_list.push_back( alloc.back() );

			if ( should_sort )
				std::push_heap( open_list.begin(), open_list.end(), cmp );
		}

	public:
		Pathfinder( int grid_size ) 
			: grid_size( grid_size )
		{
			//  create nodes
			for ( int y = 0; y < grid_size; y++ )
				for ( int x = 0; x < grid_size; x++ )
				{
					std::string name = std::to_string( nodes.size() );
					nodes[pos_to_grid_id( x, y )] = new Node( Int2{ x, y }, name.c_str() );
				}

			//  connect nodes
			for ( int i = 0; i < nodes.size(); i++ )
			{
				Node* node = nodes[i];

				for ( int off_x = -1; off_x < 1; off_x++ )
					for ( int off_y = -1; off_y < 1; off_y++ )
					{
						if ( off_x == 0 && off_y == 0 ) continue;

						Node* neighboor = get_node_at( node->pos.x + off_x, node->pos.y + off_y );
						if ( neighboor == nullptr ) continue;

						node->connect_to( neighboor, true );
					}
			}
		}
		~Pathfinder()
		{
			release();
		}

		void release()
		{
			for ( int i = 0; i < alloc.size(); i++ )
				delete alloc[i];

			for ( int i = 0; i < nodes.size(); i++ )
				delete nodes[i];

			alloc.clear();
			nodes.clear();
			open_list.clear();
			close_list.clear();
			out_path.clear();
		}

		int pos_to_grid_id( int x, int y )
		{
			return x + grid_size * y;
		}

		void begin( Node* start, Node* end )
		{
			this->start = start, this->end = end;

			release();
			_open_connection( new NodeConnection{ start, 0, nullptr } );
		}

		bool update()
		{
			if ( open_list.empty() ) return false;

			//  find prioritized node
			std::pop_heap( open_list.begin(), open_list.end(), cmp );
			NodeConnection* current_connection = open_list.back();
			open_list.pop_back();

			//printf( "- %s\n", current_connection->node->name );

			//  check target
			if ( current_connection->node == end )
			{
				printf( "\n: finding path\n" );

				//  find path
				/*NodeConnection* connection = current_connection;
				while ( connection != nullptr )
				{
					out_path.emplace( out_path.begin(), connection->node );
					printf( ": %s\n", connection->node->name );
					connection = connection->previous;
				}*/

				return true;
			}

			//  mark as visited
			close_list.insert( current_connection->node );
			current_connection->node->state = NodeState::CLOSED;

			//  add connections to open list
			for ( auto itr = current_connection->node->connections.begin(); itr != current_connection->node->connections.end(); itr++ )
			{
				Node* node = *itr;

				//  ignore visited nodes
				if ( close_list.find( node ) != close_list.end() ) continue;

				//  add to open list
				_open_connection( new NodeConnection{ node, current_connection->cost + get_node_dist( current_connection->node, node ), current_connection}, true);
				node->state = NodeState::OPENED;

				//printf( "%s: %d\n", connection->node->name, current_connection->cost + connection->cost );
			}
		}

		int get_node_dist( Node* a, Node* b )
		{
			return dist::euclidean_sqr( a->pos, b->pos );
		}

		Node* get_node_at( int x, int y )
		{
			int id = pos_to_grid_id( x, y );
			if ( nodes.find( id ) == nodes.end() ) 
				return nullptr;

			return nodes[id];
		}
	};
}