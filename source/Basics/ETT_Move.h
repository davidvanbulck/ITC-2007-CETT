#ifndef ETT_MOVE_H
#define ETT_MOVE_H

#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <set>
#include <map>
#include <list>
#include <assert.h>

struct node {
	std::vector<int> residualCap;
	std::vector<int> roomExams;
	std::vector<int> roomAlone;
	std::vector<std::vector<int>> durations;
	std::vector<int> assignment;
	int capCost, durCost, exclCost, roomCost, totCost;
	int nodeId;   // Make sure that each node is unique (without the need to check assignment vector)
	float randCost; // Tie break at random
	int level;
	bool isOldAssignment;
	node* parent;
	int lastRoom;

	node(){}
	node(node* other, int r) : capCost(other->capCost), durCost(other->durCost), exclCost(other->exclCost), roomCost(other->roomCost), lastRoom(r), parent(other) {}
	node(std::vector<int> *residualCap, std::vector<int> *roomExams, std::vector<int> *roomAlone, std::vector<std::vector<int>> *durations, std::vector<int> *assignments, int capCost, int durCost, int exclCost, int roomCost, int totCost, int nodeId, float randCost, int level, bool isOldAssignment, node* parent, int lastRoom) : 
		residualCap(*residualCap), roomExams(*roomExams), roomAlone(*roomAlone), durations(*durations), assignment(*assignments), capCost(capCost), durCost(durCost), exclCost(exclCost), totCost(totCost), nodeId(nodeId), randCost(randCost), level(level), isOldAssignment(isOldAssignment), parent(parent), lastRoom(lastRoom) {}
	
#ifndef NDEBUG
	bool operator<(const node* other) const {
		// Lowest cost first, tie-break: lowest residual capacity
		std::cout << "This comparison between nodes was not expected" << std::endl;
		std::cout << "Aborting" << std::endl;
		std::abort();
	}
#endif
};

bool nodeComparison(node *lhs, node *rhs);

struct nodeComp {
	  bool operator()(const node* lhs, const node* rhs) const { 
		return (
				// First order on cost
				lhs->totCost < rhs->totCost 
				// Tie break on residual capacity
				|| ((lhs->totCost == rhs->totCost) && (lhs->parent->residualCap[lhs->lastRoom] < rhs->parent->residualCap[rhs->lastRoom])) 
				// Then tie break on rand cost 
				// Intuition: if you only break on node id, then if zero-sols exist in the beginning, they will look very similar
				|| ((lhs->totCost == rhs->totCost) && (lhs->parent->residualCap[lhs->lastRoom] == rhs->parent->residualCap[rhs->lastRoom]) && (lhs->randCost < rhs->randCost))
				// Then tie break on unique node id
				// You will perhaps never reach this...
				|| ((lhs->totCost == rhs->totCost) && (lhs->parent->residualCap[lhs->lastRoom] == rhs->parent->residualCap[rhs->lastRoom]) && (lhs->randCost == rhs->randCost) && (lhs->nodeId < rhs->nodeId))
			);
				
	  }
};

class ETT_MoveExam {
	friend bool operator==(const ETT_MoveExam& m1, const ETT_MoveExam& m2);
	friend bool operator!=(const ETT_MoveExam& m1, const ETT_MoveExam& m2);
	friend bool operator<(const ETT_MoveExam& m1, const ETT_MoveExam& m2);
	friend std::ostream& operator<<(std::ostream& os, const ETT_MoveExam& c);
	friend std::istream& operator>>(std::istream& is, ETT_MoveExam& c);

	public:
	ETT_MoveExam();

	// To be enumerated: exam, toIndex, newRoomIndex
	// All other values can be computed from the above three
	int exam, from, to, oldRoom, newRoom;
	int deltaConflicts; 
};

class ETT_SwapExam {
	friend bool operator==(const ETT_SwapExam& m1, const ETT_SwapExam& m2);
	friend bool operator!=(const ETT_SwapExam& m1, const ETT_SwapExam& m2);
	friend bool operator<(const ETT_SwapExam& m1, const ETT_SwapExam& m2);
	friend std::ostream& operator<<(std::ostream& os, const ETT_SwapExam& c);
	friend std::istream& operator>>(std::istream& is, ETT_SwapExam& c);

	public:
	ETT_SwapExam();

	// To be enumerated: exam1, exam2
	// All other values can be computed from the above two
	int exam1, exam2, period1, period2, room1, room2;
	int deltaConflicts;
};

class ETT_KickExam {
	friend bool operator==(const ETT_KickExam& m1, const ETT_KickExam& m2);
	friend bool operator!=(const ETT_KickExam& m1, const ETT_KickExam& m2);
	friend bool operator<(const ETT_KickExam& m1, const ETT_KickExam& m2);
	friend std::ostream& operator<<(std::ostream& os, const ETT_KickExam& c);
	friend std::istream& operator>>(std::istream& is, ETT_KickExam& c);

	public:
	ETT_KickExam();

	// To be enumerated: exam1, exam2, newPeriod, newRoom
	// All other values are redundant
	int exam1, exam2, room1, room2, period1, period2, newPeriod, newRoom;
	int deltaConflicts;
	// True if we perform a simple swap (redundant)
	bool swap;
	// True if only the rooms are changed
	bool roomOnly;
};

class ETT_KempeExam {
	friend bool operator==(const ETT_KempeExam& m1, const ETT_KempeExam& m2);
	friend bool operator!=(const ETT_KempeExam& m1, const ETT_KempeExam& m2);
	friend bool operator<(const ETT_KempeExam& m1, const ETT_KempeExam& m2);
	friend std::ostream& operator<<(std::ostream& os, const ETT_KempeExam& c);
	friend std::istream& operator>>(std::istream& is, ETT_KempeExam& c);

	public:
	ETT_KempeExam();

	// To be enumerated: exam, newPeriod, newRoom
	// All other values are redundant
	int exam, oldRoom, oldRoomIndex, newRoom, newRoomIndex, from, fromIndex, to, toIndex, notMovedConflicts;

	std::vector<int> newRoomAssignment; // New room assignment, only use positions of exams in the chain
	int newCapViol; 		 // The cost of this new room assignment, determined by bestFit

	std::unordered_set<int> s1; // All exams that move from kc.from to kc.to in the repair chain
	std::unordered_set<int> s2; // All exams that move from kc.to to kc.from in the repair chain
};

class ETT_ShakeExam {
	friend bool operator==(const ETT_ShakeExam& m1, const ETT_ShakeExam& m2);
	friend bool operator!=(const ETT_ShakeExam& m1, const ETT_ShakeExam& m2);
	friend bool operator<(const ETT_ShakeExam& m1, const ETT_ShakeExam& m2);
	friend std::ostream& operator<<(std::ostream& os, const ETT_ShakeExam& c);
	friend std::istream& operator>>(std::istream& is, ETT_ShakeExam& c);

	public:
	ETT_ShakeExam();

	// To be enumerated: period1 and period2
	// All other values can be computed from the above two
	int period1, period2;
};

class ETT_ComponentExam {
	friend bool operator==(const ETT_ComponentExam& c1, const ETT_ComponentExam& c2);
	friend bool operator!=(const ETT_ComponentExam& c1, const ETT_ComponentExam& c2);
	friend bool operator<(const ETT_ComponentExam& c1, const ETT_ComponentExam& c2);
	friend std::ostream& operator<<(std::ostream& os, const ETT_ComponentExam& c);
	friend std::istream& operator>>(std::istream& is, ETT_ComponentExam& c);

	public:
	ETT_ComponentExam();

	// To be enumerated: period1 and period2
	// All other values can be computed from the above two
	int component, slot, k;
	int firstSlot; 		// First slot during which an exam in the component is assigned
	int lastSlot; 		// Last slot during which an exam in the component is assigned
				//
	std::set<int> slotsUsed; // All slots used
	std::map<int, std::list<int>> examsInSlotsUsed; // [p] gives all the exams from the component scheduled in period p
	std::set<int> slotsMovedTo;   // All slots from which exams have been moved to
	std::set<int> slotsMovedFrom; // All slots from which exams have been removed
	
	std::vector<int> newRoomAssignment; // New room assignment, only use positions of exams in the chain
	int newCapViol; 		 // The cost of this new room assignment, determined by bestFit
	int newMixedRoom; 		 // The cost of the new mixed room assignment
};


class ETT_BeamExam {
	friend bool operator==(const ETT_BeamExam& m1, const ETT_BeamExam& m2);
	friend bool operator!=(const ETT_BeamExam& m1, const ETT_BeamExam& m2);
	friend bool operator<(const ETT_BeamExam& m1, const ETT_BeamExam& m2);
	friend std::ostream& operator<<(std::ostream& os, const ETT_BeamExam& c);
	friend std::istream& operator>>(std::istream& is, ETT_BeamExam& c);

	public:
	ETT_BeamExam();

	// To be enumerated: examIds
	// Reoptimize all rooms assigned to examIds
	// Note: it does not make sense to consider exams assigned to a different 
	// period (separate optimization problems)
	std::vector<std::pair<int, int>> examIds; 	// All exams to be reoptimized

	// All other values are redundant
	int period, cost; 				// Period can be derived from any exam in examIds
	node bestFound; 				// Best found assignment
	int oldRoomOccupCost; 					
	int oldRoomExclCost;
	int oldMixedDurCost;
	int oldRoomPenalty;

};

#endif /* ETT_MOVE_H */
