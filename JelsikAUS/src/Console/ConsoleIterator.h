#pragma once
#include <string>
#include <vector>
#include <functional>
#include <iomanip>
#include <libds/mm/memory_manager.h>>
#include "Stop.h"

template<typename T>
struct ConsoleIterator
{
	virtual ~ConsoleIterator() = default;

	//navigation
	virtual void goToStart() = 0;
	virtual bool goPrevious() = 0;
	virtual bool goNext(size_t index) = 0;

	//inspection of current node
	virtual T& getCurrent() const = 0;
	virtual size_t childCount() const = 0;
	virtual std::string childName(size_t index) const = 0;

	//search & filter
	virtual std::vector<size_t> searchChildren(std::string_view text) const = 0;
	virtual std::vector<Stop> filterSubtree(
		std::function<bool(const Stop&)> predicate) const = 0;

	//predicates construction
	virtual int promptFilerChoice() = 0;
	virtual std::function<bool(const Stop&)> buildPredicate(int filterType) = 0;
	virtual std::function<bool(const Stop&)> predicateMunicipality(
		const std::string& name) const = 0;
	virtual std::function<bool(const Stop&)> predicateStreet(
		const std::string& street) const = 0;
	virtual std::function<bool(const Stop&)> predicateRegion(
		double minLat, double maxLat, double minLon, double maxLon) const = 0;
};