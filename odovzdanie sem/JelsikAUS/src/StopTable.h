#pragma once  
#include <libds/adt/table.h>  
#include <optional>  
#include "Stop.h"  


class StopTable
{
private:
    std::unordered_map<std::string, Stop*> stopTable_;
public:
    void insert(const Stop& stop)
    {
        stopTable_[stop.stop_ID()] = new Stop(stop);
    }

    std::optional<Stop*> find(const std::string& stopID)
    {
        auto it = stopTable_.find(stopID);
        if (it != stopTable_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    ~StopTable()
    {
        for (auto& pair : stopTable_)
        {
            delete pair.second;
        }
    }
};