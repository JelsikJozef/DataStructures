#pragma once
#include "Stop.h"
#include <functional>

namespace StopComparator
{
	inline std::function<bool(const Stop*, const Stop*)> compareStreetMunicipality =
		[](const Stop* a, const Stop* b)
		{
			if (a->municipality() != b->municipality())
			{
				return a->municipality() < b->municipality();
			}
			else
			{
				return a->street() < b->street();
			}
		};
	inline std::function<bool(const Stop*, const Stop*)> compareID =
		[](const Stop* a, const Stop* b) -> bool
		{
			return a->stop_ID() < b->stop_ID();
		};
}