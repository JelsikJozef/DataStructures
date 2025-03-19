#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "Stop.h"

/**
* @brief Reads csv file
* 
* @param filename adress of the file
* @return vector of loaded Stops
*/
class CSVReader {
public:
	std::vector<Stop> readCSV(const std::string& filename) {
		std::vector<Stop> stops;
		std::ifstream file(filename);

		if (!file.is_open()) {
			throw std::runtime_error("File not found");
		}

		std::string line;
		bool firstLine = true;

		while (std::getline(file, line)) {
			//skip header
			if (firstLine) {
				firstLine = false;
				continue;
			}

			//parse line
			std::stringstream ss(line);
			std::string token;
			std::vector<std::string> tokens;
			while (std::getline(ss, token, ';')) {
				tokens.push_back(token);
			}
			if (tokens.size() < 5) {
				std::cerr << "Error while parsing line: " << line << "\n";
				continue;
			}
			try {
				std::string stop_id = tokens[0];
				std::string street = tokens[1];
				double stop_lon = std::stod(tokens[2]);
				double stop_lat = std::stod(tokens[3]);
				
				std::string municipality = tokens[4];

				stops.emplace_back(stop_id, stop_lat, stop_lon, street, municipality);
			}
			catch (std::exception& e) {
				std::cerr << "Error while parsing line: " << line << "\n"
					<< "Error: " << e.what() << std::endl;
			}
			continue;

		}
		file.close();
		return stops;
	}
};

     