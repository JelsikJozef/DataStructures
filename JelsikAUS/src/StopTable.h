#pragma once  
#include <libds/adt/table.h>
#include <optional>  
#include "Stop.h"  


class StopTable
{
private:
    ds::adt::HashTable<std::string, Stop*> stopTable_;
public:
    void insert(const Stop& stop)
    {
        stopTable_.insert(stop.stop_ID(), new Stop(stop));
    }

    std::optional<Stop*> find(const std::string& stopID)
    {
        Stop** resultPtr = nullptr;
        if (stopTable_.tryFind(stopID, resultPtr) && resultPtr != nullptr)
        {
            return *resultPtr;
        }
        return std::nullopt;
    }

    ~StopTable()
    {
        for (auto it = stopTable_.begin();
            it != stopTable_.end(); ++it)
        {
			delete (*it).data_;
        }
		stopTable_.clear();
    }
};