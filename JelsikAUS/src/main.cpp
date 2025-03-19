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

    FilterAlgorithm algo;

    auto municipalityFilter = isInMunicipality("Kitchener");
    auto filteredStops = algo.filter(stops.begin(), stops.end(), municipalityFilter);

    for (const auto& s : filteredStops) {
		std::cout << "StopID: " << s.stop_ID() << ", Municipality: " << s.municipality()<< "\n";
    }





    return 0;
}