#pragma once
#include "MyList.h"
#include "Watch.h"
#include "MechanicalWatch.h"
#include "ElectronicWatch.h"
#include "SmartWatch.h"
#include "WristSmartWatch.h"
#include "TableClock.h"
#include "WristWatch.h"
#include <fstream>


class DataManager {
public:
    static void saveWatches(const MyList<Watch*>& watches, const std::string& filename);
    static MyList<Watch*> loadWatches(const std::string& filename);
    static std::string getWatchType(const Watch* watch);
};
