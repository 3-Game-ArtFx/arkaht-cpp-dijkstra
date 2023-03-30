#pragma once
#include "node.fwd.h"

namespace dijkstra
{
	struct NodeConnection
	{
		Node* node { nullptr };
		int cost { 0 };

		NodeConnection* previous{ nullptr };
	};

	struct NodeConnectionComparator
	{
		bool operator()( NodeConnection* a, NodeConnection* b ) const
		{
			return a->cost > b->cost;
		}
	};
}

namespace std
{
	template<> struct hash<dijkstra::NodeConnection>
	{
		size_t operator()( const dijkstra::NodeConnection& connection ) const noexcept
		{
			return (size_t) connection.node;
		}
	};
}