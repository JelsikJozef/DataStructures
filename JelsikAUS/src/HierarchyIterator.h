#pragma once
#include "HierarchyBuilder.h"
#include <iostream>

#include "Filters.h"
#include "Console/ConsoleIterator.h"

class HierarchyIterator : public ConsoleIterator<HierarchyBuilder::Node>
{

public:
	using HierarchyType = HierarchyBuilder::HierarchyType;
	using Block = HierarchyType::BlockType;
	using Node = HierarchyBuilder::Node;




	HierarchyIterator(HierarchyType& hierarchy)
		: hierarchy_(hierarchy), current_(hierarchy.accessRoot()) {
	}

	void goToStart() override
	{
		current_ = hierarchy_.accessRoot();
	}

	bool goPrevious() override
	{
		if (!hierarchy_.isRoot(*current_))
		{
			current_ = hierarchy_.accessParent(*current_);
			return true;
		}
		else
		{
			std::cout << "Already at root.\n";
			return false;
		}
	}

	bool goNext(size_t index) override
	{
		if (index < hierarchy_.degree(*current_))
		{
			current_ = hierarchy_.accessSon(*current_, index);
			return true;
		} else
		{
			std::cout << "Invalid index. Please try again.\n";
			return false;
		}
	}

	Node& getCurrent() const override{
		return current_->data_;
	}


	size_t childCount() const override
	{
		return hierarchy_.degree(*current_);
	}

	std::string childName(size_t index) const override
	{
		auto* child = hierarchy_.accessSon(*current_, index);
		if (!child) return "Invalid index";
		if (child->data_.isStop)
		{
			return child->data_.stopData.value().stop_ID();
		}
		else
		{
			return child->data_.name;
		}
	}

	std::vector<size_t> searchChildren(std::string_view text)
	const override
	{
		std::vector<size_t> results;
		std::string target = std::string(text);
		std::transform(target.begin(), target.end(),
			target.begin(), ::tolower);

		for (size_t i = 0; i < hierarchy_.degree(*current_); i++)
		{
			auto* child = hierarchy_.accessSon(*current_, i);
			std::string childText;
			if (child->data_.isStop)
			{
				childText = child->data_.stopData.value().stop_ID();
			}
			else
			{
				childText = child->data_.name;
			}

			// Convert to lowercase for case-insensitive search
			std::transform(childText.begin(), childText.end(),
				childText.begin(), ::tolower);

			if (childText.find(target) != std::string::npos)
			{
				results.push_back(i);
			}
		}
		return results;
	}

	std::vector<Stop> filterSubtree(std::function<bool(const Stop&)> predicate) const override {
		std::vector<Stop> result;
		hierarchy_.processPreOrder(current_, [&](const Block* block) {
			if (block->data_.isStop && block-> data_.stopData.has_value() && predicate(block->data_.stopData.value())) {
				result.push_back(block->data_.stopData.value());
			}
			});
		return result;
	}

	int promptFilerChoice() override {
		std::cout << "Select filter type:\n"
			<< "1. Municipality\n"
			<< "2. Street\n"
			<< "3. Region\n"
			<< "> ";
		int choice;
		std::cin >> choice;
		if (std::cin.fail()) throw std::invalid_argument("Invalid input.");
		std::cin.ignore();
		return choice;
	}

	std::function<bool(const Stop&)> buildPredicate(int filterType) override {
		switch (filterType) {
		case 1: {
			std::string name;
			std::cout << "Enter municipality name: ";
			std::getline(std::cin, name);
			return predicateMunicipality(name);
		}
		case 2: {
			std::string street;
			std::cout << "Enter street substring: ";
			std::getline(std::cin, street);
			return predicateStreet(street);
		}
		case 3: {
			double minLat, maxLat, minLon, maxLon;
			std::cout << "Enter min latitude (sirka): "; std::cin >> minLat;
			std::cout << "Enter max latitude: "; std::cin >> maxLat;
			std::cout << "Enter min longitude (dlzka): "; std::cin >> minLon;
			std::cout << "Enter max longitude: "; std::cin >> maxLon;
			if (std::cin.fail()) throw std::invalid_argument("Coordinates must be numbers.");
			std::cin.ignore();
			return predicateRegion(minLat, maxLat, minLon, maxLon);
		}
		default:
			throw std::invalid_argument("Unknown filter type.");
		}
	}

	std::function<bool(const Stop&)> predicateMunicipality(
		const std::string& name) const override {
		if (hierarchy_.isRoot(*current_)) {
			throw std::logic_error("You must be inside a municipality.");
		}
		return isInMunicipality(name);
	}

	std::function<bool(const Stop&)> predicateStreet(const std::string& street)
	const override {
		return isOnStreet(street);
	}

	std::function<bool(const Stop&)> predicateRegion(
		double minLat, double maxLat, double minLon, double maxLon)
	const override {
		return isInRegion(minLat, maxLat, minLon, maxLon);
	}

private:
	HierarchyType& hierarchy_;
	Block* current_;

};