#include "Statistics.h"

StatsInfo LoadStats(const string& filePath, string& playerName) {
	INIFile file(filePath);
	INIStructure ini;
	StatsInfo stats{ playerName, 0, 0 };

	file.read(ini);

	string& w  = ini[playerName]["wins"];
	stats.wins = ExtractNumber(w, 0);

	string& l    = ini[playerName]["losses"];
	stats.losses = ExtractNumber(l, 0);

	file.write(ini, true);
	return stats;
}

void SaveStats(const string& filePath, StatsInfo& stats) {
	INIFile file(filePath);
	INIStructure ini;

	if (!file.read(ini)) {
		cerr << "Failed to open file: " << filePath << endl;
		return;
	}

	ini[stats.name]["wins"] = to_string(stats.wins);
	ini[stats.name]["losses"] = to_string(stats.losses);

	file.write(ini, true);

	cout << "Stats saved for player \"" << stats.name << "\"" << endl;
}
