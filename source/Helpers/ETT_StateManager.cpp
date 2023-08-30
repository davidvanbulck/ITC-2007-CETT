#include "ETT_StateManager.h"

// constructor
ETT_StateManager::ETT_StateManager(const ETT_Input & pin) : StateManager<ETT_Input,ETT_State>(pin, "ETTStateManager")  {} 

// initial state builder (random rooms)
void ETT_StateManager::RandomState(ETT_State& st) {
	// Insert the code that creates a random state in object st
	
	// TODO Uncomment the code below for a random state
	//// Reset all redundant data
	//st.resetState();

	//// Select a random period and random room for each exam
	//int i;
	//for (i = 0; i < in.getNoExams(); ++i) {
	//	st.periodAssignment[i] = in.getValidPeriod(i, EasyLocal::Core::Random::Uniform<int>(0, in.getNoValidPeriods(i)-1));
	//	st.roomAssignment[i] = in.getValidRoom(i, EasyLocal::Core::Random::Uniform<int>(0, in.getNoValidRooms(i)-1));
	//}

	//// Compute all redundant data
	//st.updateRedundantStateData();

	// TODO Uncommnent the code below for an initial state based on DSATUR
	HeuristicState(st);
} 

void ETT_StateManager::HeuristicState(ETT_State& st) {
	// Insert the code to construct a heuristic state
	// See also 
	//   Burke, E.; Bykov, Y. & Newall J.and Petrovic, S. A time-predefined local search approach to exam timetabling problems 
	//     IIE Trans., Taylor & Francis, 2004, 36, 509-528
	//   Bykov, Y. & Petrovic, S. A step counting hill climbing algorithm applied to university examination timetabling 
	//     J. Sched., Springer, 2016, 19, 479-492
	
	// TODO TODO See also Carter his backtracking strategy
	//     Carter, M. W.; Laporte, G. & Lee, S. Y. Examination timetabling: Algorithmic strategies and applications J. Oper. Res. Soc., Springer, 1996, 47, 373-383
	
	// DSATUR (saturation = no. time slots to which it cannot be assigned):
	//   1. Pick an exam with the largest saturation, and randomly assign it to one of the
	//   available time slots (if no available time slot exists, randomly assign it to a time slot)
	//   2. Repeat 1 until all exams are assigned
	//   3. Randomly assign rooms

	// All exams are yet to be assigned a period and room
	std::unordered_set<int> exams;
	int i;
	for (i = 0; i < in.getNoExams(); ++i) {
		exams.insert(i);
	}	

	// Keep track of the conflict free slots of exam e
	std::vector<std::unordered_set<int>> validPeriods(in.getNoExams());
	for (auto &e : exams){
		for(i = 0; i < in.getNoValidPeriods(e); ++i){
			validPeriods[e].insert(in.getValidPeriod(e,i));
		}
	}

	// Choose the exam with the least flexibility and assign it to a slot
	int available, minAvailable, minExam, minConflicts;
	while(!exams.empty()){
		minAvailable = INT_MAX;
		minConflicts = -1;
		minExam = -1;
		for(auto &e : exams){
			available = validPeriods[e].size();
			if(available == minAvailable && in.noConflicts(e) > minConflicts){
				// Break ties based on number of conflicts
				minExam = e;
				minConflicts = in.noConflicts(e);
			} else if(available < minAvailable){
				minAvailable = available;
				minExam = e;
				minConflicts = in.noConflicts(e);
			}
		}
		assert(minExam != -1);

		// Assign minExam to one of the randomly available time slots
		if (validPeriods[minExam].size() > 0){
			// There is at least one conflict free period
			st.periodAssignment[minExam] = *std::next(validPeriods[minExam].begin(), EasyLocal::Core::Random::Uniform<int>(0, validPeriods[minExam].size()-1));
		} else {
			// No conflict free period is available: pick a random one
			st.periodAssignment[minExam] = in.getValidPeriod(minExam, EasyLocal::Core::Random::Uniform<int>(0, in.getNoValidPeriods(minExam)-1));
		}
		int slot = st.periodAssignment[minExam];

		// The room assignment is random
		st.roomAssignment[minExam] = in.getValidRoom(minExam, EasyLocal::Core::Random::Uniform<int>(0, in.getNoValidRooms(minExam)-1));

		// Update the valid periods based on exam conflicts
		for(i=0; i < in.noConflicts(minExam); ++i){
			validPeriods[in.getConflictList(minExam, i)].erase(slot);
		}

		// Update the valid periods based on examBefore
		for(i=0; i < in.getNoBefore(minExam); ++i){
			int d = in.getBefore(minExam, i);
			for(int s=0; s <= slot; ++s){
				validPeriods[d].erase(slot);
			}
		}
		
		// Update the valid periods based on examAfter
		for(i=0; i < in.getNoAfter(minExam); ++i){
			int d = in.getAfter(minExam, i);
			for(int s=slot; s < in.getNoPeriods(); ++s){
				validPeriods[d].erase(slot);
			}
		}

		// Update the valid periods based on examCoin
		for(i=0; i < in.getNoCoin(minExam); ++i){
			if(validPeriods[in.getCoin(minExam,i)].count(slot)){
				validPeriods[in.getCoin(minExam,i)] = {slot};
			} else {
				// No valid slot will exist after the assignment...
				validPeriods[in.getCoin(minExam,i)].clear();
			}
		}

		exams.erase(minExam);
	}

	// Compute all redundant data
	st.updateRedundantStateData();
}

bool ETT_StateManager::CheckConsistency(const ETT_State& st) const {
	// Insert the code that checks if state in object st is consistent
	// (for debugging purposes)
	ETT_State& dummy = const_cast<ETT_State&>(st);
	return dummy.redundantDataIsConsistent();
}


