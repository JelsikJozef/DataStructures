#pragma once
#include <fstream>
#include <iostream>
#include <array>
#include <vector>
#include <string>



struct Stop {
	int stop_id;
	double stop_lat;
	double stop_lon;
	std::string street;
	std::string municipality;
};

std::vector<Stop> readStops(const std::string& filename) {
	std::vector<Stop> stops;
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error: could not open file " << filename << std::endl;
		return stops;
	}
	std::string line;

}

     