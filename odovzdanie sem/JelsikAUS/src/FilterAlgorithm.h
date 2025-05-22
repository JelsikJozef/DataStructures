#pragma once	
#include "Stop.h"
#include <vector>

class FilterAlgorithm {
	public:
		template <typename Iterator, typename Predicate>
		std::vector<Stop> filter(Iterator begin, Iterator end, Predicate pred) {
			std::vector<Stop> result;
			for (auto it = begin; it != end; ++it) {
				if (pred(*it)) {
					result.push_back(*it);
				}

			}
			return result;
		}
};