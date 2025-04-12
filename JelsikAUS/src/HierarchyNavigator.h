#pragma once
#include "HierarchyBuilder.h"
#include <iostream>

#include "Filters.h"

/**
 * @brief Navigator class provides manual traversal capabilities over the municipality-street -stop hierarchy.
 */
class HierarchyNavigator {
public:
	using HierarchyType = HierarchyBuilder::HierarchyType;
	using Block = HierarchyType::BlockType;

	/**
	 *@brief Constructs a navigator starting at the root of the given hierarchy.
	 */
	HierarchyNavigator(HierarchyType& hierarchy);

	/**
	 *@brief Move to the parent of the current node.
	 */
	void GoToParent();

	/**
	 *@brief Move to the i-th child of the current node.
	 *@param index Index of the child move to.
	 */
	void GoToChild(size_t index);

	/**
	 *@brief Move to the root of the hierarchy.
	 */
	void GoToRoot() { current_ = hierarchy_.accessRoot() ; }

	/**
	 *@brief Prints the current node's name and, if applicable, its Stop ID.
	 */
	void PrintCurrentNode() const;

	/**
	 *@brief  Lists all children of the current node with their indices.
	 */
	void ListChildren() const;

	/**
	 *@brief Returns a pointer to the current node.
	 *@return Pointer to the current node.
	 */
	Block* current() const { return current_; };

	/**
	 *@brief set the current node manually.
	 */
	void setCurrent(Block* node) { current_ = node; }

	/**
	 *@brief Filter all stops under the current node using a predicate.
	 *@param predicate Lambda predicate to apply.
	 *@return Vector of matching stops.
	 */
	std::vector<Stop> FilterStops(std::function<bool(const Stop&)> predicate) const;

	/**
	 *@brief Starts interactive command line interface for navigating and filtering.
	 */
	void RunConsole();

private:
	HierarchyType& hierarchy_; //< Reference to the underlying hierarchy
	Block* current_; //< Pointer to the current node

	// Command line interface
	void ExecuteCommand(const std::string& input); //< Execute a command based on user input
	void PrintHelp(); //< Print help information
	void HandleGo(const std::string& arg);
	void HandleFilter();
};
//------------------Implementation------------------//

HierarchyNavigator::HierarchyNavigator(HierarchyType& hierarchy) :
	hierarchy_(hierarchy),current_(hierarchy.accessRoot()){}

void HierarchyNavigator::GoToParent() {
	if (hierarchy_.isRoot(*current_)) {
		std::cout << "Already at the root node.\n";
	}
	else {
		current_ = hierarchy_.accessParent(*current_);
	}
}

void HierarchyNavigator::GoToChild(size_t index)
{
	if (index >= hierarchy_.degree(*current_))
	{
		std::cout << "Invalid child index.\n";
		return;
	}
	current_ = hierarchy_.accessSon(*current_, index);
}

void HierarchyNavigator::PrintCurrentNode() const
{
	const auto& data = current_->data_;
	std::cout << "Current node: " << data.name;
	if (data.isStop)
	{
		std::cout << " (Stop ID: " << data.stopData.stop_ID() << ")";
	} else {
		std::cout << "\n";
	}
}

void HierarchyNavigator::ListChildren() const
{
	size_t deg = hierarchy_.degree(*current_);
	std::cout << "Children (" << deg << "):\n";
	for (size_t i = 0; i < deg; ++i)
	{
		Block* child = hierarchy_.accessSon(*current_, i);
		if (child -> data_.isStop)
		{
			std::cout << "(Stop ID: " << child->data_.stopData.stop_ID() << ") ";
		}
		else
		{
			std::cout << "("<<i<< ")"<<child->data_.name << ", ";
		}
	}
	std::cout << "\n";
}

std::vector<Stop> HierarchyNavigator::FilterStops(std::function<bool(const Stop&)> predicate) const
{
	std::vector<Stop> result;
	hierarchy_.processPreOrder(current_,[&](const Block* block)
	{
		if (block -> data_.isStop && predicate(block-> data_.stopData))
		{
			result.push_back(block->data_.stopData);
		}
		});
	return result;
}

//Comand Line Interface
void HierarchyNavigator::RunConsole()
{
	std::string line;
	std::cout << "\nType 'help' for a list of commands.\n";
	while (true)
	{
		std::cout << "\n> ";
		std::getline(std::cin, line);
		if (line == "exit") break;
		ExecuteCommand(line);
	}
}

void HierarchyNavigator::ExecuteCommand(const std::string& input)
{
	std::istringstream iss(input);
	std::string cmd;
	iss >> cmd;

	if (cmd == "help") PrintHelp();
	else if (cmd == "ls") ListChildren();
	else if (cmd == "info") PrintCurrentNode();
	else if (cmd == "root") GoToRoot();
	else if (cmd == "up") GoToParent();
	else if (cmd == "go")
	{
		std::string indexStr;
		iss >> indexStr;
		HandleGo(indexStr);
	}
	else if (cmd == "filter")
	{
		HandleFilter();
	}
	else {
		std::cout << "Unknown command. Type 'help' for a list of commands.\n";
	}
}

void HierarchyNavigator::PrintHelp()
{
	std::cout << "Available commands:\n"
	          << "  root			-Move to root\n"
			  << "  up				-Move to parent\n"
			  << "  go <index>		-Move to child at index (starts at 0) \n"
			  << "  ls				-List children\n"
			  << "  info			-Show current node info\n"
			  << "  filter			-Apply filter\n"
			  << "  help			-Show this help message\n"
			  << "  exit			-Exit the console\n";
}

void HierarchyNavigator::HandleGo(const std::string& arg)
{
	try
	{
		size_t index = std::stoul(arg);
		GoToChild(index);
		
	} catch (...)
	{
		std::cout << "Invalid index. Please enter a valid number.\n";
	}
}

void HierarchyNavigator::HandleFilter()
{
	std::cout << "Select filter type:\n"
		<< "1. Municipality\n"
		<< "2. Street\n"
		<< "3. Region\n"
		<< "> ";
	int choice;
	std::cin >> choice;
	std::cin.ignore(); // Ignore the newline character left in the input buffer

	std::function<bool(const Stop&)> predicate;
	switch (choice) {
		case 1:
			{
			std::string name;
			std::cout << "Enter municipality name: ";
			std::getline(std::cin, name);
			if (current_ != hierarchy_.accessRoot())
			{
				predicate = isInMunicipality(name);
			}
			else
			{
				std::cout << "You are at the root node. Please navigate to a municipality first.\n";
				return;
			}
			}
			break;
		case 2:
			{
			std::string substr;
			std::cout << "Enter substring to match in street name: ";
			std::getline(std::cin, substr);
			predicate = isOnStreet(substr);
			}
			break;
		case 3:
			{
			double minLat, maxLat, minLon, maxLon;
			std::cout << "Enter min latitude: "; std::cin >> minLat;
			std::cout << "Enter max latitude: "; std::cin >> maxLat;
			std::cout << "Enter min longitude: "; std::cin >> minLon;
			std::cout << "Enter max longitude: "; std::cin >> maxLon;
			std::cin.ignore();
			predicate = isInRegion(minLat, maxLat, minLon, maxLon);
			break;
			}
		default:
			std::cout << "Invalid choice. Please select a valid filter type.\n";
			return;
	}

	auto results = FilterStops(predicate);
	std::cout << "Filtered stops:\n";
	for (const auto& stop : results)
	{
		std::cout << "Stop ID: " << stop.stop_ID();
	}
	std::cout << "\n";
}