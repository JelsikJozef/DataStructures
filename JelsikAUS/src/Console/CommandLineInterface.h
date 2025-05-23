#pragma once
#pragma once
#include "../UniversalSorter.h"
#include "../StopComparators.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "ConsoleIterator.h"
#include "StopTable.h"

/**
 * @brief Command Line interface mediates between user and all assignments 
 * @tparam T HierarchyBuilder::Node
 */
template<typename T>
class CommandLineInterface
{
public:
	/**
	 * @brief Constructs a CommandLineInterface object.
	 * @param iterator The ConsoleIterator object for navigating the hierarchy.
	 * @param stopTable The StopTable object for looking up stops.
	 */
	CommandLineInterface(ConsoleIterator<T>& iterator, StopTable& stopTable)
		: iterator_(iterator), stopTable_(stopTable) {
	}

	/**
	 * @brief Starts the command line interface.
	 * This function runs a loop that waits for user input and executes commands.
	 */
	void run()
	{
		printHelp();
		std::string line;
		while (std::cout << "\n>", std::getline(std::cin, line))
		{
			if (line == "exit") break;
			execute(line);
		}
	}
private:
	ConsoleIterator<T>& iterator_;
	StopTable& stopTable_;

	/**
	 * @brief Executes the command line input.
	 * @param cmdLine The command line input from the user.
	 */
	void execute(std::string& cmdLine)
	{
		std::istringstream iss(cmdLine);
		std::string cmd;
		iss >> cmd;

		if (cmd == "help")			printHelp();
		else if (cmd == "root")		iterator_.goToStart();
		else if (cmd == "up")		iterator_.goPrevious();
		else if (cmd == "go")		handleGo(iss);
		else if (cmd == "ls")		handleList();
		else if (cmd == "info")		handleInfo();
		else if (cmd == "search")   handleSearch(iss);
		else if (cmd == "filter")	handleFilter();
		else if (cmd == "lookup")   handleLookup(iss);
		else if (cmd == "sort")		handleSort(iss);
		else std::cout << "Unknown command. Type 'help' for a list of commands.\n";

	}
	/**
	 * @brief Handles the "go" command.
	 * @param iss The input stream containing the command arguments.
	 */
	void handleGo(std::istringstream& iss)
	{
		size_t index;
		if (iss >> index)
		{
			if (!iterator_.goNext(index))
			{
				std::cout << "Invalid index. Please try again.\n";
			}
		}
	}
	/**
	 * @brief Handles the "ls" command.
	 * Lists the children of the current node.
	 */
	void handleList()
	{
		auto count = iterator_.childCount();
		std::cout << "Children (" << count << "):\n";
		for (size_t i = 0; i < count; ++i)
		{
			std::cout << std::setw(3) << i << ": "
				<< iterator_.childName(i) << "\n";
		}
	}
	/**
	 * @brief Handles the "info" command.
	 * Displays information about the current node.
	 */
	void handleInfo()
	{
		auto e = iterator_.getCurrent();
		try {
			std::cout << e.toString() << "\n";
		}
		catch (...)
		{
			std::cout << "Object is not printable\n";
		}
	}
	/**
	 * @brief Handles the "search" command.
	 * Searches for children of the current node by name.
	 * @param iss The input stream containing the search query.
	 */

	void handleSearch(std::istringstream& iss)
	{
		std::string query;
		std::getline(iss, query);
		//Remove potential spaces
		query.erase(0, query.find_first_not_of(' '));

		auto matches = iterator_.searchChildren(query);
		for (auto i : matches)
		{
			std::cout << "[" << i << "]" << iterator_.childName(i) << "\n";
		}
		if (matches.empty())
		{
			std::cout << "No matches found.\n";
		}
	}

	/**
	 * @brief combines together the filter algorithm and the predicate
	 */
	void handleFilter()
	{
		try
		{
			int choice = iterator_.promptFilerChoice();
			auto predicate = iterator_.buildPredicate(choice);
			auto results = iterator_.filterSubtree(predicate);
			std::cout << "Filtered stops:\n";
			for (const auto& stop : results)
			{
				std::cout << stop.toString() << std::endl;
			}
		}
		catch (const std::exception& e)
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
				'\n');
			std::cout << "Error: " << e.what() << "\n";
		}
	}
	/**
	 * @brief Displays the available commands.
	 */
	void static printHelp()
	{
		std::cout << "Available commands:\n";
		std::cout << std::setw(16) << "root" << " - Move to root\n";
		std::cout << std::setw(16) << "up" << " - Move to parent\n";
		std::cout << std::setw(16) << "go <i>" << " - Move to child at index\n";
		std::cout << std::setw(16) << "ls" << " - List children\n";
		std::cout << std::setw(16) << "info" << " - Show current node info\n";
		std::cout << std::setw(16) << "search <substring>" << " - Search children by name\n";
		std::cout << std::setw(16) << "filter" << " - Apply filter\n";
		std::cout << std::setw(16) << "lookup <name>" << " - Lookup Stop in a table\n";
		std::cout << std::setw(16) << "help" << " - Show this help message\n";
		std::cout << std::setw(16) << "exit" << " - Exit the console\n";
		std::cout << std::setw(16) << "sort <type>" << " - Sort stops (id/location)\n";
	}
	/**
	 * @brief Handles the "lookup" command.
	 * Looks up a stop by its ID in the StopTable.
	 * @param iss The input stream containing the stop ID.
	 */
	void handleLookup(std::istringstream& iss)
	{
		std::string stopId;
		if (!(iss >> stopId))
		{
			std::cout << "Please provide a stop ID.\n";
			return;
		}

		//Remove potential spaces
		stopId.erase(0, stopId.find_first_not_of(' '));
		stopId.erase(stopId.find_last_not_of(' ') + 1);

		auto stopResult = stopTable_.find(stopId);
		if (stopResult.has_value() && *stopResult != nullptr)
		{
			Stop* stop = *stopResult;
			std::cout << "Stop found:\n" << stop->toString() << "\n";
		}
		else
		{
			std::cout << "Stop not found with ID: " << stopId << "\n";
		}

	}
	/**
	 * @brief Handles the "sort" command.
	 * Sorts the stops by ID or location.
	 * @param iss The input stream containing the sort type.
	 */

	void handleSort(std::istringstream& iss)
	{
		std::string sortType;
		if (!(iss >> sortType))
		{
		 std::cout << "Please specify sort type 'id' or 'location'.\n";
		 return;
		}
		auto allStops = iterator_.filterSubtree([](const Stop&) { return true; });

		ds::amt::ImplicitSequence<Stop*> stopSequence;
		for (const auto& stop : allStops)
		{
			auto& node = stopSequence.insertLast();
			node.data_ = new Stop(stop);
		}

		UniversalSorter<Stop*> sorter;

		if (sortType == "id")
		{
			std::cout << "Sorting by ID...\n";
			sorter.sort(stopSequence, StopComparator::compareID);
		}
		else if (sortType == "location")
		{
			std::cout << "Sorting stops by Municipality and Street...\n";
			sorter.sort(stopSequence, StopComparator::compareStreetMunicipality);
		}
		else
		{
			std::cout << "Unknown sort type. Use 'id' or 'location'.\n";
			for (size_t i = 0; i < stopSequence.size(); ++i) {
				delete stopSequence.access(i)->data_;
			}
			return;
		}
		std::cout << "Sorted stops:\n";
		for (size_t i = 0; i < stopSequence.size(); ++i)
		{
			Stop* stop = stopSequence.access(i)->data_;
			std::cout << stop->toString();
		}

		for (size_t i = 0; i < stopSequence.size(); ++i) {
			delete stopSequence.access(i)->data_;
		}
	}
};