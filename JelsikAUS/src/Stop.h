#pragma once
#include <string>
#include <cctype>

/* * Stop class represents a bus stop with its ID, latitude, longitude,*/

class Stop {
	private:
		std::string stop_id_;
		double stop_lat_;
		double stop_lon_;
		std::string street_;
		std::string municipality_;
	public:
		Stop() :stop_id_(""), stop_lat_(0.0),
			stop_lon_(0.0), street_(""), municipality_("") {
		}
		Stop(const std::string& stop_id, double stop_lat,
			double stop_lon, const std::string& street, 
			const std::string& municipality) :
			stop_id_(stop_id), stop_lat_(stop_lat),
			stop_lon_(stop_lon), street_(street),
			municipality_(municipality) {
		}
		std::string stop_ID() const {
			return stop_id_;
		}
		double latitude() const {
			return stop_lat_;
		}
		double longitude() const {
			return stop_lon_;
		}
		std::string street() const {
			return street_;
		}
		std::string municipality() const {
			return municipality_;
		}
		std::string toString() const {
			return "Stop ID: " + stop_id_ + ", Latitude: " +
				std::to_string(stop_lat_) + ", Longitude: " +
				std::to_string(stop_lon_) + ", Street: " +
				street_ + ", Municipality: " + municipality_;
		}
};