#include <libds/mm/compact_memory_manager.h>
#include "CSVReader.h"
#include "Stop.h"
#include <string>
#include <print>
#include <vector>


using namespace std;
using namespace ds::mm;

struct Person {
	int id_;


	Person() {
		id_ = 100;
		
	}
};

struct Student : Person {
	int student_number_;
	Student() {
		student_number_ = 200;
	}
};

int main() {
    try {
        CSVReader reader;
        std::vector<Stop> stops = reader.readCSV("./subory/GRT_Stops_Relevant.csv");

        // test for stops
        for (const Stop& stop : stops) {
			std::cout << "StopID: " << stop.stop_ID()
                << ", Latitude: " << stop.latitude()
                << ", Longitude: " << stop.longitude()
                << ", Street: " << stop.street()
                << ", Municipality: " << stop.municipality() << std::endl;
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}