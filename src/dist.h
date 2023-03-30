#pragma once
#include <math.h>

#include "int2.h"

namespace dist
{
	int manhattan( Int2 a, Int2 b )
	{
		return abs( b.x - a.x ) + abs( b.y - a.y );
	}

	int euclidean_sqr( Int2 a, Int2 b )
	{
		int x_diff = b.x - a.x;
		int y_diff = b.y - a.y;
		return x_diff * x_diff + y_diff + y_diff;
	}
}