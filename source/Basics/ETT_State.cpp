#include "ETT_State.h"

ETT_State& ETT_State::operator=(const ETT_State& st) {
	// Insert the code that copies all data structures of the
	// state object from the ones of the parameter out
	// (excluding the reference to the input object, that is constant)
	
	periodAssignment = std::vector<int>(st.periodAssignment);
	roomAssignment = std::vector<int>(st.roomAssignment);
	hardViolatedAndFeasOnly = st.hardViolatedAndFeasOnly;

	updateRedundantStateData();

	return *this;
}

bool operator==(const ETT_State& st1, const ETT_State& st2) {
	// Insert the code that compares two states (this is used by the tester)
	int i;
	for(i=0; i < st1.periodAssignment.size(); ++i){
		if(st1.periodAssignment[i] != st2.periodAssignment[i] || 
				st1.roomAssignment[i] != st2.roomAssignment[i]){
			return false;
		}
	}
	return true;	
}


std::ostream& operator<<(std::ostream& os, const ETT_State& st) {
	// Insert the code that writes the state object (for debugging)
	int i,j,k;
	os << "ETT_State: " << std::endl;
	os << "RoomAssignment:" << std::endl; 
	for(i=0; i < st.periodAssignment.size(); ++i){
		os << st.roomAssignment[i] << " ";
	}
	os << std::endl;

	os << "PeriodAssignment:" << std::endl;
	for(i=0; i < st.periodAssignment.size(); ++i){
		os << st.periodAssignment[i] << " ";
	}
	os << std::endl;

	os << "Room occupation (redundant) " << std::endl;
	for(i=0; i < st.roomOccupation.size(); ++i){
		for(j=0; j < st.roomOccupation[i].size(); ++j){
			os << st.roomOccupation[i][j] << " ";
		}
		os << std::endl;
	}

	os << "Room exams (redundant) " << std::endl;
	for(i=0; i < st.roomExams.size(); ++i){
		for(j=0; j < st.roomExams[i].size(); ++j){
			os << st.roomExams[i][j] << " ";
		}
		os << std::endl;
	}

	os << "Room alone (redundant) " << std::endl;
	for(i=0; i < st.roomAlone.size(); ++i){
		for(j=0; j < st.roomAlone[i].size(); ++j){
			os << st.roomAlone[i][j] << " ";
		}
		os << std::endl;
	}

	os << "Room durations (redundant) " << std::endl;
	for(i=0; i < st.roomDurations.size(); i++){
		for (j = 0; j < st.roomDurations[i].size(); ++j) {
			os << "Room " << i << " period " << j << ": ";
			for (k = 0; k < st.roomDurations[i][j].size(); ++k) {
				os << st.roomDurations[i][j][k] << "\t";
			}
			os << std::endl;
		}
	}

	os << "Exam period conflicts (redundant) " << std::endl;
	for(i=0; i < st.examPeriodConflicts.size(); i++){
		os << "Exam " << i << ": ";
		for(j=0; j < st.examPeriodConflicts[i].size(); ++j){
			os << st.examPeriodConflicts[i][j] << "\t";
		}
		os << std::endl;
	}

	//os << "Weights (redundant) " << std::endl;
	//for(i=0; i < st.weights.size(); i++){
	//	os << "Exam " << i << ": " << st.weights[i] << std::endl;
	//}

	return os;
}

ETT_State::ETT_State(const ETT_Input &my_in) : in(my_in), roomAssignment(in.getNoExams(), 0), periodAssignment(in.getNoExams(), 0) {
	roomOccupation.resize(in.getNoRooms(), std::vector<int>(in.getNoPeriods(),0));
	roomExams.resize(in.getNoRooms(), std::vector<int>(in.getNoPeriods(),0));
	roomAlone.resize(in.getNoRooms(), std::vector<int>(in.getNoPeriods(),0));
	roomDurations.resize(in.getNoRooms(), std::vector<std::vector<int>>(in.getNoPeriods(), std::vector<int>(in.getNoUniqueExamDurations(), 0)));
	examPeriodConflicts.resize(in.getNoExams(), std::vector<int>(in.getNoPeriods(),0));
	//weights.resize(in.getNoExams(), 0);
}

bool ETT_State::redundantDataIsConsistent(){
	// Only for debugging
	bool consistent = true;

	// Copy the old redundant state
	std::vector<std::vector<int>> roomOccupationCopy(roomOccupation);
	std::vector<std::vector<int>> roomExamsCopy(roomExams);
	std::vector<std::vector<int>> roomAloneCopy(roomAlone);
	std::vector<std::vector<std::vector<int>>> roomDurationsCopy(roomDurations);
	std::vector<std::vector<int>> examPeriodConflictsCopy(examPeriodConflicts);
	//std::vector<int> weightsCopy(weights);

	// Recompute all values from scratch
	updateRedundantStateData();

	// Check wether old state is consistent with new state
	int i,j,k;
	for(i=0; i < roomOccupationCopy.size(); ++i){
		//if (weightsCopy[i] != weights[i]) {
		//	std::cerr << "Consistency error in weights[" << i << "]" << " (computed: " << weights[i] << ", stored " << weightsCopy[i] << ")" << std::endl;			
		//	consistent = false;
		//}
		for(j=0; j < roomOccupationCopy[i].size(); ++j){
			if(roomOccupation[i][j] != roomOccupationCopy[i][j]){
				std::cerr << "Consistency error in roomOccupation[" << i << "][" << j << "] (computed: " << roomOccupation[i][j] << ", stored: " << roomOccupationCopy[i][j] << ")" << std::endl;

				consistent = false;
			}
			if(roomExams[i][j] != roomExamsCopy[i][j]){
				std::cerr << "Consistency error in roomExams[" << i << "][" << j << "] (computed: " << roomExams[i][j] << ", stored: " << roomExamsCopy[i][j] << ")" << std::endl;

				consistent = false;
			}
			if(roomAlone[i][j] != roomAloneCopy[i][j]){
				std::cerr << "Consistency error in roomAlone[" << i << "][" << j << "] (computed: " << roomAlone[i][j] << ", stored: " << roomAloneCopy[i][j] << ")" << std::endl;

				consistent = false;
			}
			for (k = 0; k < in.getNoUniqueExamDurations(); ++k) {
				if(roomDurations[i][j][k] != roomDurationsCopy[i][j][k]){
					std::cerr << "Consistency error in roomDurations[" << i << "][" << j << "][" << k << "] (computed: " << roomDurations[i][j][k] << ", stored: " << roomDurationsCopy[i][j][k] << ")" << std::endl;
					consistent = false;
				}
			}
		}
	}	

	for(i=0; i < examPeriodConflictsCopy.size(); ++i){
		for(j=0; j < examPeriodConflictsCopy[i].size(); ++j){
			if(examPeriodConflicts[i][j] != examPeriodConflictsCopy[i][j]){
				std::cerr << "Consistency error in examPeriodConflicts[" << i << "][" << j << "] (computed: " << examPeriodConflicts[i][j] << ", stored: " << examPeriodConflictsCopy[i][j] << ")" << std::endl;

				consistent = false;
			} 
		}
	}

	return consistent;
}

void ETT_State::updateRedundantStateData(){
	// Reset all redundant data to 0
	resetState();	

	// Update all redundant data
	int e1,e2,r1,p1,p2,a;
	for(e1=0; e1 < in.getNoExams(); ++e1){
		r1 = roomAssignment[e1];
		p1 = periodAssignment[e1];

		// Increase roomOccupation
		roomOccupation[r1][p1] += in.getNoExamStudents(e1);
		++roomExams[r1][p1];
		if(in.getExamAlone(e1)) ++roomAlone[r1][p1];
		roomDurations[r1][p1][in.getExamDurationIndex(e1)]++;

		for(a=0; a < in.noConflicts(e1); a++){
			e2 = in.getConflictList(e1, a);

			if(e2 > e1) { // Each pair
				p2 = periodAssignment[e2];
				examPeriodConflicts[e1][p2] += in.getConflict(e1,e2);
				examPeriodConflicts[e2][p1] += in.getConflict(e1,e2);
			}
		}
		
		// Update weights
		//int validPeriodConflicts = 0;
		//for (a = 0; a < in.getNoPeriods(); ++a) {
		//	validPeriodConflicts += (in.isValidPeriod(e1, a) && (examPeriodConflicts[e1][a] == 0));	
		//}
		//weights[e1] = validPeriodConflicts*in.getNoValidRooms(e1) - 1;
	}
	//distribution = std::discrete_distribution<int>(weights.begin(), weights.end());
}

void ETT_State::resetState(){
	// Reset all redundant data
	int i,j,k;

	for(i=0; i < in.getNoRooms(); ++i){
		for(j=0; j < in.getNoPeriods(); ++j){
			roomOccupation[i][j] = 0;
			roomExams[i][j] = 0;
			roomAlone[i][j] = 0;
			for(k=0; k < in.getNoUniqueExamDurations(); ++k){
				roomDurations[i][j][k] = 0;
			}
		}
	}
	for(i=0; i < in.getNoExams(); ++i){
		//weights[i] = 0;
		for(j=0; j < in.getNoPeriods(); ++j){
			examPeriodConflicts[i][j] = 0;
		}
	}
} 
