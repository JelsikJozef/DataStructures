#pragma once
#include "Stop.h"
#include <string>
#include <algorithm>
#include <cctype>

/**
 * @brief  filters the stops based on the given predicate
 * @param municipalityToFind 
 * @return whether the stop is in the desired municipality
 */
inline auto isInMunicipality(const std::string& municipalityToFind) {
	return [municipalityToFind](const Stop& s) {
		std::string lowerStop = s.municipality();
		std::transform(lowerStop.begin(), lowerStop.end(), lowerStop.begin(), ::tolower);

		std::string lowerSearch = municipalityToFind;
		std::transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(), ::tolower);
		return lowerStop == lowerSearch;
		};
}
/**
 * @brief filters the stops based on the given predicate
 * @param streetSubstring
 * @return whether the stop is on the desired street
 */
inline auto isOnStreet(const std::string& streetSubstring) {
	return [streetSubstring](const Stop& s) {
		return s.street().find(streetSubstring) != std::string::npos;
		};
}

/**
 * @brief filters the stops based on the given predicate
 * @param minLat minimum latitude
 * @param maxLat maximum latitude
 * @param minLon minimum longitude
 * @param maxLon maximum longitude
 * @return whether the stop is in the desired region
 */
inline auto isInRegion(double minLat, double maxLat, double minLon, double maxLon) {
	return [minLat, maxLat, minLon, maxLon](const Stop& s) {
		return s.latitude() >= minLat && s.latitude() <= maxLat &&
			s.longitude() >= minLon && s.longitude() <= maxLon;
		};
}

