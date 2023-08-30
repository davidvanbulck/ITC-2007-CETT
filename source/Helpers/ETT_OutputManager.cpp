#include "ETT_OutputManager.h"

void ETT_OutputManager::InputState(ETT_State& st, const ETT_Output& out) const {
	// Insert the code that "translates" an output object to a state object
	int e;
	for(e=0; e < in.getNoExams(); e++){
		st.roomAssignment[e] = out.roomAssignment[e];
		st.periodAssignment[e] = out.periodAssignment[e];
	}
	st.updateRedundantStateData();
}

void ETT_OutputManager::OutputState(const ETT_State& st, ETT_Output& out) const {
	// Insert the code that "translates" a state object to an output object
	int e;
	for(e=0; e < in.getNoExams(); e++){
		out.roomAssignment[e] = st.roomAssignment[e];
		out.periodAssignment[e] = st.periodAssignment[e];
	}
}


