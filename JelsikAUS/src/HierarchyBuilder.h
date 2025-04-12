#pragma once

#include "Stop.h"
#include "CSVReader.h"
#include <libds/amt/explicit_hierarchy.h>
#include <unordered_map>
#include <string>

/**
 * @brief The HierarchyBuilder class provides a method to construct a multi-level hierarchy of
 * stops
 * The structure of the hierarchy is as follows:
 * - Root node represents the transport company (e.g., GRT)
 * - First level nodes represent municipalities (e.g., Kitchener, Waterloo)
 * - Second level nodes represent streets (e.g., King St, Regina St)
 * - Third level nodes represent stops (e.g., Stop1, Stop2) stops have IDs
 */
class HierarchyBuilder
{
	/**
  * @brief NodeData holds the name of the node (municipality, street, or stop)
  */
	struct NodeData
	{
		std::string name; //< name of the node
		Stop stopData; //< Stop data only used at the leaf (stop) level
		bool isStop = false; //< true if the node is a stop, false otherwise

		bool operator==(const NodeData& other ) const
		{
			return name == other.name &&
				stopData.stop_ID() == other.stopData.stop_ID()&&
				isStop == other.isStop;
		}
	};
public:
	using HierarchyType = ds::amt::MultiWayExplicitHierarchy<NodeData>; //< Alias for the type of the hierarchy
	using Block = HierarchyType::BlockType;			 //< Alias for the type of the block in the hierarchy

	/**
  *@brief Constructs hierarchy from the provided vector of stops.
  *
  *Each stop is inserted under its respective municipality and street. with the stop itself as the leaf node.
  *
  *@param stops vector of Stop objects loaded from the CSV file
  *@return A populated MultiWayExplicitHierarchy tree.
  */
	static HierarchyType buildHierarchy(const std::vector<Stop>& stops)
	{
		HierarchyType hierarchy;

		//Create the root node "GRT"
		Block& root = hierarchy.emplaceRoot();
		root.data_.name = "GRT";

		//Maps for fast access to existing municipality and street nodes
		std::unordered_map<std::string, Block*> municipalityMap;
		std::unordered_map<std::string, std::unordered_map<std::string, Block*>> streetMap;

		//Process each stop and insert it into the appropriate place in the hierarchy
		for (const Stop& stop : stops)
		{
			const std::string& municipality = stop.municipality();
			const std::string& street = stop.street();

			Block* municipalityNode = nullptr;
            if (municipalityMap.find(municipality) != municipalityMap.end())
			{
				municipalityNode = municipalityMap[municipality];
			} else
			{
				Block& newMunicipality = hierarchy.emplaceSon(root, municipalityMap.size());
				newMunicipality.data_.name = municipality;
				municipalityMap[municipality] = &newMunicipality;
				municipalityNode = &newMunicipality;
			}
			Block* streetNode = nullptr;
			auto& municipalityStreets = streetMap[municipality];
			if (municipalityStreets.find(street) != municipalityStreets.end()) //check if street already exists
			{
				streetNode = municipalityStreets[street];
			} else {
				Block& newStreet = hierarchy.emplaceSon(*municipalityNode, municipalityStreets.size());
				newStreet.data_.name = street;
				municipalityStreets[street] = &newStreet;
				streetNode = &newStreet;
			}
			//Insert stop as a leaf under the street
			Block& stopNode = hierarchy.emplaceSon(*streetNode, hierarchy.degree(*streetNode));
			stopNode.data_.name = stop.stop_ID();
			stopNode.data_.stopData = stop; // Store the stop data
			stopNode.data_.isStop = true; // Mark this node as a stop
		}
		return hierarchy;
	}
};