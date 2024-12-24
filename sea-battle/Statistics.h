#pragma once

#include <iostream>

#include "ini.h"
#include "UtilityFunctions.h"

using namespace std;
using namespace mINI;

struct StatsInfo {
	string name;
	int wins;
	int losses;
};

StatsInfo LoadStats(const string& filePath, string& playerName);
void      SaveStats(const string& filePath, StatsInfo& stats);