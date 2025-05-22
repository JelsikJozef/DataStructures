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


//FIRST TASK VECTOR STORED DATA
void firstTask(CSVReader reader, FilterAlgorithm algo,
               std::vector<Stop> stops)
{
	//FIRST TASK VECTOR STORED DATA


	//Declaration of FilterAlgorithm
	

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
}

//running the programm
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

	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}

	return 0;
}
