#include "UtilityFunctions.h"

int ExtractNumber(string& str, int defaultVal) {
	if (!str.empty()) {
		size_t pos = 0;
		while (pos < str.size() && !isdigit(str[pos])) {
			++pos;
		}
		str.erase(0, pos);

		if (!str.empty()) {
			return stoi(str);
		}
	}

	str = to_string(defaultVal);
	return defaultVal;
}