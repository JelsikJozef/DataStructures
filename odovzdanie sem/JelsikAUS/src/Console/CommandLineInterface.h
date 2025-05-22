#pragma once
#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "ConsoleIterator.h"
#include "StopTable.h"


template<typename T>
class CommandLineInterface
{
public:
	CommandLineInterface(ConsoleIterator<T>& iterator, StopTable& stopTable)
		: iterator_(iterator), stopTable_(stopTable) {
	}


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
		else std::cout << "Unknown command. Type 'help' for a list of commands.\n";

	}

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

	void handleSearch(std::istringstream& iss)
	{
		std::string query;
		std::getline(iss, query);

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
	}

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
};