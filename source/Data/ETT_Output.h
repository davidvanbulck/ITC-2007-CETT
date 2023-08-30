#ifndef ETT_OUTPUT_H
#define ETT_OUTPUT_H

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>

#include "ETT_Input.h"

class ETT_Output {
	friend std::ostream& operator<<(std::ostream& os, const ETT_Output& out);
	friend std::istream& operator>>(std::istream& is, ETT_Output& out);

	public:
	ETT_Output(const ETT_Input& i);
	ETT_Output& operator=(const ETT_Output& out);

	int getPeriodAssignment(const int e) const { return periodAssignment[e]; }
	int getRoomAssignment(const int e) const { return roomAssignment[e]; }

	protected:
	const ETT_Input& in;

	public:
	std::vector<int> roomAssignment; 	// [e] room assigned to exam e
	std::vector<int> periodAssignment; // [e] period assigned to exam e

	void visualize();
};


#endif /* ETT_OUTPUT_H */
