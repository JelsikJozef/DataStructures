#include <libds/mm/compact_memory_manager.h>
#include "CSVReader.h"
#include "Stop.h"
#include <string>
#include <print>
#include <vector>
#include "Filters.h"
#include "FilterAlgorithm.h"



int main() {


    CSVReader reader;
    std::vector<Stop> stops = reader.readCSV("./subory/GRT_Stops_Relevant.csv");

	//Declaration of FilterAlgorithm
    FilterAlgorithm algo;

	//1. Filter - Municipality
    auto municipalityFilter = isInMunicipality("Kitchener");
    auto filteredStops = algo.filter(stops.begin(), stops.end(), municipalityFilter);

	/*std::cout << "Stops in Kitchener: \n";
    for (const auto& s : filteredStops) {
		std::cout << "StopID: " << s.stop_ID() << ", Municipality: " << s.municipality()<< "\n";
    }*/
	//2. Filter - Street
	auto streetFilter = isOnStreet("Regina St");
	filteredStops = algo.filter(stops.begin(), stops.end(), streetFilter);

	std::cout << "Stops on King Street: \n";
	for (const auto& s : filteredStops) {
		std::cout << "StopID: " << s.stop_ID() << ", Street: " << s.street() << "\n";
	}
	//3. Filter - Region
	/*auto regionFilter = isInRegion(43.4, 43.5, -80.6, -80.5);
	auto regionStops = algo.filter(stops.begin(), stops.end(), regionFilter);

	std::cout << "Stops in Region: \n";
	for (const auto& s : regionStops) {
		std::cout << "StopID: " << s.stop_ID() << ", Latitude: " << s.latitude() << ", Longitude: " << s.longitude() << "\n";
	}*/



    return 0;
}