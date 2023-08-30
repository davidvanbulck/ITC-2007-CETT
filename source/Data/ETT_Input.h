#ifndef ETT_INPUT_H
#define ETT_INPUT_H

#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <string>
#include <stdexcept>
#include <fstream>
#include <set>
#include <algorithm> // std::sort, std::set_intersection

class ETT_Input {
	friend std::ostream& operator<<(std::ostream& os, const ETT_Input& in);

	// Constructor and basic getters
	public:
	ETT_Input(std::string fileName, const std::vector<int>& hardConWeights);
	std::string getInstanceName() const { return instanceName; }
	int getNoUniqueExamDurations() const { return uniqueExamDurations.size(); }
	protected:
	std::string instanceName;
	std::vector<int> uniqueExamDurations; 	// Contains all different exam durations (needed for mixed duration calculations)

	// Soft constraint weights
	protected:
	int twoInARowCost; 			// Penalty when a student has to sit two exams in a row on the same day
	int twoInADayCost; 			// Penalty when a student has to sit two exams but not in a row on the same day
	int periodSpread; 				// Penalty of 1 when a student has to sit > 1 exam withi periodSpread slots
	int maxPeriodsDay; 			// Max number of periods in the same day
	int nonMixedDurationsCost; 		// Penalty when two exams scheduled in the same room have a different time duration
	int firstFrontLoadPeriod; 	 		// First period in which no frontLoad should be scheduled
	int frontLoadCost; 			// Penalty when a frontLoad exam is scheduled in on of the last periods
	std::vector<int> frontLoadIndices; 	// Indices of frontLoad exams
	std::vector<bool> frontLoad; 			// [e] true if exam e is a frontload exam
							//
	const bool fncomp (int l, int r) const {return getNoExamStudents(l) > getNoExamStudents(r);}

	public:
	int getTwoInARowCost() const {return twoInARowCost;}
	int getTwoInADayCost() const {return twoInADayCost;}
	int getPeriodSpread() const {return periodSpread;}
	int getMaxPeriodsDay() const {return maxPeriodsDay;}
	int getNonMixedDurationsCost() const {return nonMixedDurationsCost;}
	int getFrontLoadIndex(const int i) const { return frontLoadIndices[i]; }
	int getFrontLoadSize() const { return frontLoadIndices.size(); }
	int getFirstFrontLoadPeriod() const { return firstFrontLoadPeriod; }
	int getFrontLoadCost() const { return frontLoadCost; }
	bool isFrontLoad(const int i) const { return frontLoad[i]; }

	// Hard constraint weights
	// Conflict cost is considered as 1, all other cost components are a multiple of this
	public:
	const int ROOM_OCCUPANCY_COST;
	const int PERIOD_UTILIZATION_COST;
	const int PERIOD_RELATED_COST;
	const int ROOM_RELATED_COST;


	// Exam info
	protected:
	int noExams;
	std::vector<int> examDuration; 		// Length of the exam in minutes	
	std::vector<std::vector<int>> examStudents;// [e] gives all student ids taking exam e (sorted)
	std::vector<bool> examAlone; 			// [e] true if exam e should receive its own room
	std::vector<std::vector<int>> conflict; 	// [e1][e2] gives the number of students exams e1 and e2 have in common
	std::vector<std::vector<int>> conflictList; 	// [e1] gives all exams e1 has a conflict with
	std::vector<std::vector<std::vector<int>>> pairConflictList;// [e1][e2] gives all exams e1 or e2 has a conflict with
	std::vector<std::vector<int>> examAfter; 	// [e] examIds with which e has an after relation
	std::vector<std::vector<int>> examBefore; 	// [e] examIds with which e has a before relation
	std::vector<std::vector<int>> examExcl; 	// [e] examIds with which e has an exclusive relation
	std::vector<std::vector<int>> examCoin; 	// [e] examIds with which e has a conincidence relation

	public:
	int getNoExams() const { return noExams; }
	int getExamDuration(const int e) const { return examDuration[e]; }
	int getNoExamStudents(const int e) const { return examStudents[e].size(); }
	int getExamStudents(const int e, const int a) const { return examStudents[e][a]; }
	int getExamAlone(const int e) const { return examAlone[e]; }
	int getConflict(const int e1, const int e2) const { return conflict[e1][e2]; }
	int getConflictList(const int e, const int a) const { return conflictList[e][a]; }
	int getConflictList(const int e1, const int e2, int a) const { return pairConflictList[e1][e2][a]; }
	int noConflicts(const int e) const { return conflictList[e].size(); }
	int noConflicts(const int e1, const int e2) const { return pairConflictList[e1][e2].size(); }
	int getAfter(const int e, const int a) const { return examAfter[e][a]; }
	int getNoAfter(const int e) const { return examAfter[e].size(); }
	int getBefore(const int e, const int a) const { return examBefore[e][a]; }
	int getNoBefore(const int e) const { return examBefore[e].size(); }
	int getExcl(const int e, const int a) const { return examExcl[e][a]; }
	int getNoExcl(const int e) const { return examExcl[e].size(); }
	int getCoin(const int e, const int a) const { return examCoin[e][a]; }
	int getNoCoin(const int e) const { return examCoin[e].size(); }

	// Period info
	protected:
	int noPeriods;
	std::vector<int> periodDay; 		// Index of day (index 0 is first day, e.g. 01:01:2000) 
	std::vector<int> periodSlot; 		// Index of slot (index 0 is the first block of the day, e.g. 09:00:00)
	std::vector<int> periodDuration; 		// Length of the period in minutes
	std::vector<int> periodPenalty; 		// Penalty for using this period
	std::vector<std::string> periodDayName; 	// Debugging, format 22:05:1900
	std::vector<std::string> periodSlotName; 	// Debugging, format 09:00:00
	std::vector<std::vector<bool>> validPeriod; 	// [e][p] true if duration exam e <= duration period p 
	std::vector<std::vector<int>> validPeriodList; // [e] gives all valid periods for exam e
	std::vector<std::vector<int>> validPeriodIndex; // [e][p] gives the index of period p in validPeriodList (-1 if not valid)
	std::vector<int> examDurationIndex; 	// [e] index in durationIndex for exam e

	// Features
	public:
	int PHC; 	// Number of constraints under [PeriodHardConstraints] in instance file
	int RHC; 	// Number of constraints under [RoomHardConstraints] in instance file
	int FLP; 	// Number of frontload periods
	float CD; 	// Conflict density
	float ExR; 	// Exams per room
	float SxE; 	// Number of enrollments per exam (!= noStudents/noExams !)
	float SCap; 	// Number of enrollments / (noRooms*noPeriods)
	float PC; 	// Period conflict


	public:
	int getNoPeriods() const { return noPeriods; }
	int getPeriodDay(const int p) const { return periodDay[p]; }
	int getPeriodSlot(const int p) const { return periodSlot[p]; }
	int getPeriodDuration(const int p) const { return periodDuration[p]; }
	int getPeriodPenalty(const int p) const { return periodPenalty[p]; }
	std::string getPeriodDayName(const int p) const { return periodDayName[p]; }
	std::string getPeriodSlotName(const int p) const { return periodSlotName[p]; }
	bool isValidPeriod(const int e, const int p) const { return validPeriod[e][p]; }
	int getNoValidPeriods(const int e) const { return validPeriodList[e].size(); }
	int getValidPeriod(const int e, const int a) const { return validPeriodList[e][a]; }
	int getValidPeriodIndex(const int e, const int p) const { return validPeriodIndex[e][p]; }
	int getExamDurationIndex(const int e) const { return examDurationIndex[e]; }

	// Helper functions and data to compute difference in days between two dates
	// Needed to compute day index of a period
	// See https://www.tutorialspoint.com/find-number-of-days-between-two-given-dates-in-cplusplus
	// A small error is on the website: 
	// 	monthDays gives the number of days, before the start of the month. Hence I added 0 for the first input
	// 	in the function countNoOfDays, you should acces monthDays with the month -1 (since c++ counts from 0, but
	// 	Januari is month 1).
	const int MONTH_DAYS[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243,273, 304, 334};
	int countLeapYearDays(const int d[]) const;
	int countNoOfDays(const int date1[], const int date2[]) const;


	// Room info
	protected:
	int noRooms;
	std::vector<int> roomCapacity; 		// Capacity in students of each room
	std::vector<int> roomPenalty; 		// Penalty for using this room
	std::vector<std::vector<bool>> validRoom; 	// [e][r] true if students of exam e <= capacity room r
	std::vector<std::vector<int>> validRoomList; // [e] gives all valid rooms for exam e
	std::vector<std::vector<int>> validRoomIndex; // [e][r] gives the index of room r in validRoomList (-1 if not valid)
	int noComponents = 0; 			  // Total number of components we consider

	public: // For speed issues
	std::vector<std::vector<int>> components; // [i][j] gives j-th exam in component i, in sorted order of exam size
	std::vector<std::set<int>> componentsSet; // [i] gives the set of exams in component i
						     
	public:
	int getNoRooms() const { return noRooms; }
	int getRoomCapacity(const int r) const { return roomCapacity[r]; }
	int getRoomPenalty(const int r) const { return roomPenalty[r]; }
	bool isValidRoom(const int e, const int r) const { return validRoom[e][r]; }
	int getNoValidRooms(const int e) const { return validRoomList[e].size(); }
	int getValidRoom(const int e, const int a) const { return validRoomList[e][a]; }
	int getValidRoomIndex(const int e, const int r) const { return validRoomIndex[e][r]; }
	int getNoComponents() const {return noComponents;}

	// Student info
	protected:
	int noStudents;
	std::vector<int> studentIds; 		// studentIds[s] s gives the id of student s
	std::vector<std::vector<int>> studentExams;// studentExams[s] gives all the exams of student s
	void visualize(); 			// Create dot file for visualization of conflict graph
	int findComponents(); 			// Enumerate all connected components
	int merge(int* parent, int x);	 	// Helper function to enumerate components


	public:
	int getNoStudents() const { return noStudents; }
	int getStudentId(const int s) const { return studentIds[s]; }
	std::vector<int> getStudentExams(const int s) const { return studentExams[s]; }

};

#endif /* ETT_INPUT_H */
