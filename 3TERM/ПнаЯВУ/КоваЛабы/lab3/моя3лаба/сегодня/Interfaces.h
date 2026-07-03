#pragma once
#include "MyList.h"
#include "Watch.h"
#include "MechanicalWatch.h"
#include "ElectronicWatch.h"
#include "SmartWatch.h"
#include "WristSmartWatch.h"
#include "TableClock.h"
#include "WristWatch.h"

class Interfaces {
public:
    static void displayHeader();
    static void displayWatches();
    static void modifyWatchById(int id);
    static void deleteWatchById(int id);
    static void addWatch();
};