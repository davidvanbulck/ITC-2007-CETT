#ifndef ETT_STATE_H
#define ETT_STATE_H

#include "../Data/ETT_Input.h"
#include <random>
#include "../Basics/ETT_Move.h"


class ETT_State {  
	friend std::ostream& operator>>(std::istream& is, const ETT_State& st);
	friend std::ostream& operator<<(std::ostream& os, const ETT_State& st);
	friend bool operator==(const ETT_State& st1, const ETT_State& st2);

	protected:
	const ETT_Input &in;  

	// Constructors
	public:
	ETT_State(const ETT_Input &in);
	ETT_State& operator=(const ETT_State& s);

	// Solution
	public:
	std::vector<int> roomAssignment; 	// [e] room assigned to exam e
	std::vector<int> periodAssignment; // [e] period assigned to exam e
	
	public:
	bool hardViolatedAndFeasOnly = false; 	// True if at least one of the hard cons is violated, and
					// the search space consists of feasible solutions only

	// Redundant data
	public:
	void updateRedundantStateData();
	void resetState();
	bool redundantDataIsConsistent();
	int getRoomOccupation(const int r, const int p) const { return roomOccupation[r][p]; }
	void incRoomOccupation(const int r, const int p, const int v) { roomOccupation[r][p] += v;}
	void decRoomOccupation(const int r, const int p, const int v) { roomOccupation[r][p] -= v;}
	void setRoomOccupation(const int r, const int p, const int v) { roomOccupation[r][p] = v;}
	int getRoomExams(const int r, const int p) const { return roomExams[r][p]; }
	void incRoomExams(const int r, const int p) { roomExams[r][p]++; }
	void decRoomExams(const int r, const int p) { roomExams[r][p]--; }
	void setRoomExams(const int r, const int p, const int v) { roomExams[r][p] = v; }
	int getRoomAlone(const int r, const int p) const { return roomAlone[r][p]; }
	void incRoomAlone(const int r, const int p) { roomAlone[r][p]++; }
	void decRoomAlone(const int r, const int p) { roomAlone[r][p]--; }
	void setRoomAlone(const int r, const int p, const int v) { roomAlone[r][p] = v; }
	int getRoomDurations(const int r, const int p, const int k) const { return roomDurations[r][p][k]; }
	void incRoomDurations(const int r, const int p, const int k) { roomDurations[r][p][k]++; }
	void decRoomDurations(const int r, const int p, const int k) { roomDurations[r][p][k]--; }
	void setRoomDurations(const int r, const int p, const int k, const int v) { roomDurations[r][p][k] = v; }
	int getExamPeriodConflicts(const int e, const int p) const { return examPeriodConflicts[e][p]; }
	void incExamPeriodConflicts(const int e, const int p, const int val) { examPeriodConflicts[e][p] += val; }
	void decExamPeriodConflicts(const int e, const int p, const int val) { examPeriodConflicts[e][p] -= val; }
	void setExamPeriodConflicts(const int e, const int p, const int val) { examPeriodConflicts[e][p] = val; }


	protected:
	std::vector<std::vector<int>> roomOccupation; 		// [r][p] number of students in room r on period p
	std::vector<std::vector<int>> roomExams; 		// [r][p] number of exams in room r on period p
	std::vector<std::vector<int>> roomAlone; 		// [r][p] number of exams in room r on period p that should be scheduled alone
	std::vector<std::vector<std::vector<int>>> roomDurations; // [r][p][k] gives number of exams with duration in.durationIndex[k] in room r and period k
	std::vector<std::vector<int>> examPeriodConflicts; // [e][p] number of conflicts with exam e in period p

	int totalConflicts = 0;

	//int computeSwapDeltaExamSequence(const int period1, const int period2, const int exam1, const int exam2) const;
	//int computeSwapDeltaExamDistance(const ETT_SwapExam &swp);
	//int computeSwapDeltaExamFrontLoad(const ETT_SwapExam &swp);

	//public:
	//// Weight = noValidPeriods*noValidRooms, where validPeriods take into account period length and conflicts
	//std::vector<int> weights;
	//std::discrete_distribution<int> distribution;
};


#endif /* ETT_STATE_H */
