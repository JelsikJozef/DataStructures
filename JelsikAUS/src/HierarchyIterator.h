#pragma once
#include "HierarchyBuilder.h"
#include <iostream>

#include "Filters.h"
#include "Console/ConsoleIterator.h"

/**
 * @brief The HierarchyIterator class provides an interface for
 * navigating and filtering a hierarchy of stops.
 * It allows users to traverse the hierarchy, search for specific stops,
 * and apply filters based on various criteria.
 * @override ConsoleIterator
 */
class HierarchyIterator : public ConsoleIterator<HierarchyBuilder::Node>
{

public:
	using HierarchyType = HierarchyBuilder::HierarchyType;
	using Block = HierarchyType::BlockType;
	using Node = HierarchyBuilder::Node;



	/*
	 *@brief Constructs a HierarchyIterator for the given hierarchy.
	 */
	HierarchyIterator(HierarchyType& hierarchy)
		: hierarchy_(hierarchy), current_(hierarchy.accessRoot()) {
	}
	/*
	 *@brief navigates to the root of the hierarchy.
	 */
	void goToStart() override
	{
		current_ = hierarchy_.accessRoot();
	}
	/**@brief Moves to the parent of the current node.
	 * @return true if moved successfully, false if already at root.
	 */
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
	/**
	 * @brief Moves to the child at the specified index.
	 * @param index The index of the child to move to.
	 * @return true if moved successfully, false if index is invalid.
	 */
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

	/**
	 * 
	 * @return reference to the current node.
	 */
	Node& getCurrent() const override{
		return current_->data_;
	}

	/**
	 * @brief Returns the number of children of the current node.
	 * @return The number of children.
	 */
	size_t childCount() const override
	{
		return hierarchy_.degree(*current_);
	}
	/**
	 * @brief Returns the name of the child at the specified index.
	 * @param index The index of the child.
	 * @return The name of the child.
	 */
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

	/**
	 * 
	 * @param text helping function to find index
	 * @return 
	 */
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
	/**
	 * @brief Filters the subtree of the current node based on the given predicate.
	 * @param predicate The predicate function to filter stops.
	 * @return A vector of filtered stops.
	 */
	std::vector<Stop> filterSubtree(std::function<bool(const Stop&)> predicate) const override {
		std::vector<Stop> result;
		hierarchy_.processPreOrder(current_, [&](const Block* block) {
			if (block->data_.isStop && block-> data_.stopData.has_value() && predicate(block->data_.stopData.value())) {
				result.push_back(block->data_.stopData.value());
			}
			});
		return result;
	}
	/**
	 * @brief Prompts the user to select a filter type.
	 * @return The selected filter type.
	 */
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
	/**
	 * @brief Constructs a predicate function based on the selected filter type.
	 * @param filterType The selected filter type.
	 * @return A predicate function for filtering stops.
	 */
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
	/**
	 * @brief Constructs a predicate function to filter stops by municipality.
	 * @param name The name of the municipality.
	 * @return A predicate function for filtering stops in the specified municipality.
	 */
	std::function<bool(const Stop&)> predicateMunicipality(
		const std::string& name) const override {
		if (hierarchy_.isRoot(*current_)) {
			throw std::logic_error("You must be inside a municipality.");
		}
		return isInMunicipality(name);
	}

	/**
	 * @brief Constructs a predicate function to filter stops by street.
	 * @param street The name of the street.
	 * @return A predicate function for filtering stops on the specified street.
	 */

	std::function<bool(const Stop&)> predicateStreet(const std::string& street)
	const override {
		return isOnStreet(street);
	}

	/**
	 * @brief Constructs a predicate function to filter stops by region.
	 * @param minLat Minimum latitude of the region.
	 * @param maxLat Maximum latitude of the region.
	 * @param minLon Minimum longitude of the region.
	 * @param maxLon Maximum longitude of the region.
	 * @return A predicate function for filtering stops in the specified region.
	 */

	std::function<bool(const Stop&)> predicateRegion(
		double minLat, double maxLat, double minLon, double maxLon)
	const override {
		return isInRegion(minLat, maxLat, minLon, maxLon);
	}

private:
	HierarchyType& hierarchy_;
	Block* current_;

};