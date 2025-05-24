#include <libds/mm/compact_memory_manager.h>
#include "CSVReader.h"
#include "Stop.h"
#include <string>
#include <print>
#include <vector>
#include "Filters.h"
#include "FilterAlgorithm.h"
#include "libds/amt/explicit_hierarchy.h"
#include "StopTable.h"

#include "HierarchyBuilder.h"
#include "HierarchyIterator.h"
#include "Console/CommandLineInterface.h"



void municipality(std::vector<Stop> &stops, std::string predicate, FilterAlgorithm algo)
{
	auto municipalityFilter = isInMunicipality(predicate);
	auto filteredStops = algo.filter(stops.begin(), stops.end(), municipalityFilter);

	std::cout << "Stops in : " << predicate << std::endl;
	for (const auto& s : filteredStops) {
		std::cout << "StopID: " << s.stop_ID() << ", Municipality: " << s.municipality() << "\n";
	}
}

void street(std::vector<Stop> &stops, std::string predicate, FilterAlgorithm algo)
{
	auto streetFilter = isOnStreet(predicate);
	auto filteredStops = algo.filter(stops.begin(), stops.end(), streetFilter);
	std::cout << "Stops on " << predicate << ": \n";
	for (const auto& s : filteredStops) {
		std::cout << "StopID: " << s.stop_ID() << ", Street: " << s.street() << "\n";
	}
}

void location(std::vector<Stop> &stops, double lat1, double lat2, double lon1, double lon2, FilterAlgorithm algo)
{
	auto regionFilter = isInRegion(lat1, lat2, lon1, lon2);
	auto regionStops = algo.filter(stops.begin(), stops.end(), regionFilter);
	std::cout << "Stops in Region: \n";
	for (const auto& s : regionStops) {
		std::cout << "StopID: " << s.stop_ID() << ", Latitude: " << s.latitude() << ", Longitude: " << s.longitude() << "\n";
	}
}

//running the program
void RunConsole(std::vector<Stop> &stops) {
	auto hierarchy = HierarchyBuilder::buildHierarchy(stops);
	HierarchyIterator iterator(hierarchy);

	// Create and populate StopTable
	std::cout << "Inserting stops into StopTable..." << std::endl;
	StopTable stopTable;
	for (const auto& stop : stops) {
		stopTable.insert(stop);
	}
	std::cout << "All stops inserted successfully." << std::endl;

	try {
		std::cout << "Starting Command Line Interface..." << std::endl;
		CommandLineInterface<HierarchyBuilder::Node> console(iterator, stopTable);
		console.run();
		std::cout << "CLI terminated." << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "CLI Error: " << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Unknown error in CLI" << std::endl;
	}
}


int main() {
	initHeapMonitor();
	try {
		std::cout << "Reading CSV file..." << std::endl;
		CSVReader reader;
		std::vector<Stop> stops = reader.readCSV("./subory/GRT_Stops_Relevant.csv");
		std::cout << "Loaded " << stops.size() << " stops." << std::endl;

		RunConsole(stops);
		//FilterAlgorithm algo;
		//std::string municipalityName = "Kitchener"; // Example municipality
		//municipality(stops, municipalityName, algo);
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}

	return 0;
}
