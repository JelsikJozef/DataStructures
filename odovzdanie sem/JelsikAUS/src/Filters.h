#pragma once
#include "Stop.h"
#include <string>
#include <algorithm>
#include <cctype>


inline auto isInMunicipality(const std::string& municipalityToFind) {
	return [municipalityToFind](const Stop& s) {
		std::string lowerStop = s.municipality();
		std::transform(lowerStop.begin(), lowerStop.end(), lowerStop.begin(), ::tolower);

		std::string lowerSearch = municipalityToFind;
		std::transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(), ::tolower);
		return lowerStop == lowerSearch;
		};
}
inline auto isOnStreet(const std::string& streetSubstring) {
	return [streetSubstring](const Stop& s) {
		return s.street().find(streetSubstring) != std::string::npos;
		};
}

inline auto isInRegion(double minLat, double maxLat, double minLon, double maxLon) {
	return [minLat, maxLat, minLon, maxLon](const Stop& s) {
		return s.latitude() >= minLat && s.latitude() <= maxLat &&
			s.longitude() >= minLon && s.longitude() <= maxLon;
		};
}

