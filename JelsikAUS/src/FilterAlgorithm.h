#pragma once	
#include "Stop.h"
#include <vector>

/**
 * @brief main logic of the filtering algorithm
 */
class FilterAlgorithm {
	public:
		/**
	 * @brief Filters the vector of stops based on the predicate
	 * @param begin iterator to the start of the vector
	 * @param end iterator to the end of the vector
	 * @param pred predicate to filter the vector
	 * @return filtered vector of stops
	 */
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