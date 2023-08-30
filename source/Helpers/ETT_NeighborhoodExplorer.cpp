#include "ETT_NeighborhoodExplorer.h"

#include <chrono>
using namespace std::chrono;
#define MOVETHRESHOLD 100

//auto start = high_resolution_clock::now();
//auto stop = high_resolution_clock::now();
//auto durationOverall = duration_cast<microseconds>(stop - start);


/************************************
*  ETT_Move neighborhood explorer  *
************************************/
// initial move builder
#include <random>

bool sortbyfirstasc(const std::pair<int,int> &a, const std::pair<int,int> &b){
	return a.first<b.first;
}

bool sortbysecdesc(const std::pair<int,int> &a, const std::pair<int,int> &b){
	return a.second>b.second;
}

//struct node {
//	std::vector<int> residualCap;
//	std::vector<int> roomExams;
//	std::vector<int> roomAlone;
//	std::vector<std::vector<int>> durations;
//	std::vector<int> assignment;
//	int cost;
//	int level;
//
//	node(std::vector<int> residualCap, std::vector<int> roomExams, std::vector<int> roomAlone, 
//			std::vector<std::vector<int>> durations, std::vector<int> assignment, int cost, int level):
//		residualCap(residualCap), roomExams(roomExams), roomAlone(roomAlone),
//		durations(durations), assignment(assignment), cost(cost), level(level) {}
//	
//	bool operator<(const node& other) const {
//		// Lowest cost first, tie-break: lowest residual capacity
//		return (cost < other.cost || (cost == other.cost && residualCap[assignment[level]] < other.residualCap[other.assignment[other.level]]) || (cost == other.cost && residualCap[assignment[level]] == other.residualCap[other.assignment[other.level]]) && assignment[level] < other.assignment[other.level]);
//	}
//};

//int ETT_MoveNeighborhoodExplorer::beamSearch(const ETT_State& st, const ETT_MoveExam& mv, const int beamwidth) const {
//	// Step 1. Sort the exams in the chain in decreasing size of students
//	// O(|E|log|E|)
//	const int period = mv.to;
//	std::vector<std::pair<int,int>> examIds;
//	for(int e= 0; e < in.getNoExams(); ++e){
//		if(st.periodAssignment[e] == period){
//			examIds.push_back(std::make_pair(e, in.getNoExamStudents(e)));
//		}
//	}
//	std::sort(examIds.begin(), examIds.end(), sortbysecdesc);	
//
//	// Step 2. Initialize residual cap, number of exams in each rooms, and number of exams in each room that have an exclusive constraint
//	std::vector<int> residualCap(in.getNoRooms(),0);
//	for(int r = 0; r < in.getNoRooms(); ++r){
//		std::cout << "Room " << r << " has cap " << in.getRoomCapacity(r) << std::endl;
//		residualCap[r] = in.getRoomCapacity(r);
//	}
//	std::vector<int> roomExams(in.getNoRooms(), 0);
//	std::vector<int> roomAlone(in.getNoRooms(), 0);
//	std::vector<std::vector<int>> durations(in.getNoRooms(), std::vector<int>(in.getNoUniqueExamDurations(), 0));
//	std::vector<int> assignment(examIds.size(), -1);
//
//	// Step 3. Construct the tree
//	std::set<node> current = {};
//	std::set<node> prev = {node(residualCap, roomExams, roomAlone, durations, assignment, 0, 0)};
//	std::cout << "Beam: " << beamwidth << std::endl;
//	std::cin.get();
//
//	for(int level=0; level < examIds.size(); ++ level){
//		std::cout << "Level " << level << " with exam " << examIds[level].first << " - " << examIds[level].second << std::endl;
//		std::cout << "prev:" << std::endl;
//		for(auto &c : prev){
//			for(auto &p : c.assignment){
//				std::cout << p << " ";
//			}
//			std::cout << std::endl;
//		}
//		current = {};
//		int e = examIds[level].first;
//		for(auto &n : prev){
//			for(int r= 0; r < in.getNoRooms(); ++r){
//				std::vector<int> residualCapCopy(n.residualCap);
//				std::vector<int> roomExamsCopy(n.roomExams);
//				std::vector<int> roomAloneCopy(n.roomAlone);
//				std::vector<std::vector<int>> durationsCopy(in.getNoRooms(), std::vector<int>(in.getNoUniqueExamDurations(), 0));
//				for(int r= 0; r < in.getNoRooms(); ++r){
//					for(int i = 0; i < in.getNoUniqueExamDurations(); ++i){
//						durationsCopy[r][i] = durations[r][i];
//					}
//				}
//				std::vector<int> assignmentCopy(n.assignment);
//
//				// Exam e to room r
//				assignmentCopy[level] = r;
//				int cost = n.cost;
//				if(roomExamsCopy[r] > 0 && durationsCopy[r][in.getExamDurationIndex(e)] == 0){
//					cost += in.getNonMixedDurationsCost();
//				}
//				durationsCopy[r][in.getExamDurationIndex(e)]++;
//				cost += std::max(examIds[level].second - residualCapCopy[r], 0);
//				residualCapCopy[r] -= examIds[level].second;
//				cost += roomAloneCopy[r];
//				if(in.getExamAlone(e)){
//					cost += roomExamsCopy[r];
//					roomAloneCopy[r]++;
//				}
//				roomExamsCopy[r]++;
//				cost += in.getRoomPenalty(r);	
//
//				current.insert(node(residualCapCopy, roomExamsCopy, roomAloneCopy, durationsCopy, assignmentCopy, cost, level));
//			}
//		}
//		prev = std::set<node>(current.begin(), std::next(current.begin(), beamwidth));
//	}
//
//		std::cout << "Cost: " << prev.begin()->cost << std::endl;
//		std::cout << "Assignment: "  << std::endl;
//		for(int i=0; i < examIds.size(); ++i){
//			std::cout << "Exam " << examIds[i].first << "(" << examIds[i].second << ", " << in.getExamDuration(examIds[i].first) << ") to " << prev.begin()->assignment[i] << std::endl;
//		}
//		return prev.begin()->cost;
//
//}

void ETT_MoveNeighborhoodExplorer::RandomMove(const ETT_State& st, ETT_MoveExam& mv) const {
	// True if we perform a "Room Move", see Burke, E. K. & Bykov, Y. An Adaptive Flex-Deluge Approach to University Exam Timetabling INFORMS Journal on Computing, 2016, 28, 781-794
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	bool roomOnly = EasyLocal::Core::Random::Uniform<double>(0.001, 1.0) < roomRate;
	int cntr=0;
	bool continued;
	do {
		continued = false; 

		if (cntr++ > MOVETHRESHOLD){
#ifndef NDEBUG
			std::cout << "Empty move" << std::endl;
#endif
		       	throw EasyLocal::Core::EmptyNeighborhood(); 
		}

		// A small bias towards exams that have more flexibility to be moved around
		mv.exam = const_cast<ETT_MoveNeighborhoodExplorer*>(this)->distribution(EasyLocal::Core::Random::GetGenerator());

		// Too slow...
		// If not enough rooms: perform a period move
		//if(roomOnly){
		//       if(in.getNoValidRooms(mv.exam) <= 1) { continued = true; continue; }
		//} else {
		//	if(in.getNoValidPeriods(mv.exam) <= 1) { continued = true; continue;}
		//}
		//mv.exam = EasyLocal::Core::Random::Uniform<int>(0, in.getNoExams()-1);	
		
		// Determine the old room and period
		mv.from = st.periodAssignment[mv.exam];
		mv.oldRoom = st.roomAssignment[mv.exam];

		if(roomOnly && in.getNoValidRooms(mv.exam) > 1){
			// Choose a new room, the period stays the same
			int newRoomIndex = EasyLocal::Core::Random::Uniform<int>(0, in.getNoValidRooms(mv.exam)-2);
			if(newRoomIndex >= in.getValidRoomIndex(mv.exam, mv.oldRoom)){
				newRoomIndex++;
			}
			mv.newRoom = in.getValidRoom(mv.exam, newRoomIndex);			

			mv.to=mv.from;
			mv.deltaConflicts = 0;
		} else if (in.getNoValidPeriods(mv.exam) > 1) {
			// Determine a new time slot
			int toIndex = EasyLocal::Core::Random::Uniform<int>(0, in.getNoValidPeriods(mv.exam)-2);
			if(toIndex >= in.getValidPeriodIndex(mv.exam, mv.from)){
				toIndex++;
			}
			mv.to = in.getValidPeriod(mv.exam, toIndex);

			// And possibly a new room
			int newRoomIndex = EasyLocal::Core::Random::Uniform<int>(0, in.getNoValidRooms(mv.exam)-1);
			mv.newRoom = in.getValidRoom(mv.exam, newRoomIndex);			

			mv.deltaConflicts = st.getExamPeriodConflicts(mv.exam, mv.to) - st.getExamPeriodConflicts(mv.exam, mv.from);
		} else {
			continued = true;
			continue;
		}

	} while (continued || !FeasibleMove(st, mv));
}


//void ETT_MoveNeighborhoodExplorer::RandomMove(const ETT_State& st, ETT_MoveExam& mv) const {
//
//	// Biased version: each exam has an equal probability
//	//do {
//	//	mv.exam = EasyLocal::Core::Random::Uniform<int>(0, in.getNoExams()-1);	
//	//} while (in.getNoValidRooms(mv.exam) <= 1 && in.getNoValidPeriods(mv.exam) <= 1); // Move to a different period and/or room
//	// Unbiased version: each move has an equal probability
//	//mv.exam = const_cast<ETT_MoveNeighborhoodExplorer*>(this)->distribution(EasyLocal::Core::Random::GetGenerator());
//	mv.exam = const_cast<ETT_State&>(st).distribution(EasyLocal::Core::Random::GetGenerator());
//
//	mv.from = st.periodAssignment[mv.exam];
//	mv.fromIndex = in.getValidPeriodIndex(mv.exam, mv.from);
//	mv.oldRoom = st.roomAssignment[mv.exam];
//	mv.oldRoomIndex = in.getValidRoomIndex(mv.exam, mv.oldRoom);
//
//	// Choose a new period
//	int oldConflicts = st.getExamPeriodConflicts(mv.exam, mv.from);
//	if(in.getNoValidRooms(mv.exam) == 1){
//		// If there is only one valid room, then the periods must be different
//		assert(in.getNoValidPeriods(mv.exam) > 1); // Compiled away with NDEBUG flag
//		int cntr=0;
//		do {
//			if(mv.toIndex >= mv.fromIndex){
//				mv.toIndex++;
//			}
//			mv.to = in.getValidPeriod(mv.exam, mv.toIndex);		
//			if (++cntr % 100 == 0){
//				std::cerr << "Move empty neighborhood: " << cntr << std::endl;
//			       	throw EasyLocal::Core::EmptyNeighborhood(); 
//			}
//		} while (st.getExamPeriodConflicts(mv.exam, mv.to) > oldConflicts); // Not worsening
//	} else {
//		int cntr=0;
//		do {
//			// Can be anything
//			mv.toIndex = EasyLocal::Core::Random::Uniform<int>(0, in.getNoValidPeriods(mv.exam)-1);
//			mv.to = in.getValidPeriod(mv.exam, mv.toIndex);
//			if (++cntr % 100 == 0){
//				std::cerr << "Move empty neighborhood: " << cntr << std::endl;
//			       	throw EasyLocal::Core::EmptyNeighborhood(); 
//			}
//		} while (st.getExamPeriodConflicts(mv.exam, mv.to) > oldConflicts); // Not worsening
//	}
//
//	// Choose a new room
//	if(mv.to == mv.from){
//		// Avoid a null move: room must be different
//		assert(in.getNoValidRooms(mv.exam) > 1); // Compiled away with NDEBUG flag
//		mv.newRoomIndex = EasyLocal::Core::Random::Uniform<int>(0, in.getNoValidRooms(mv.exam)-2);
//		if(mv.newRoomIndex >= mv.oldRoomIndex){
//			mv.newRoomIndex++;
//		}
//	} else {
//                // We can select any room (including the old one, meaning we only change the period)
//		mv.newRoomIndex = EasyLocal::Core::Random::Uniform<int>(0, in.getNoValidRooms(mv.exam)-1);
//	}
//	mv.newRoom = in.getValidRoom(mv.exam, mv.newRoomIndex);
//}

// Check move feasibility
bool ETT_MoveNeighborhoodExplorer::FeasibleMove(const ETT_State& st, const ETT_MoveExam& mv) const {
	// Insert the code that checks whether move mv is legal in state st 
	// (return true if legal, false otherwise)
	
	// Period dur >= exam dur, room cap >= exam size	
	assert(in.isValidPeriod(mv.exam, mv.to));
        assert(in.isValidRoom(mv.exam, mv.newRoom));
	assert(in.isValidPeriod(mv.exam, mv.from));
	assert(in.isValidRoom(mv.exam, mv.oldRoom));
	
	// Rooms not the same, unless other period
	assert(mv.newRoom != mv.oldRoom || mv.to != mv.from);
	
	return  mv.deltaConflicts <= 0;
} 

// update the state according to the move 
void ETT_MoveNeighborhoodExplorer::MakeMove(ETT_State& st, const ETT_MoveExam& mv) const {
	// Insert the code that modifies the state st based on the application of move mv

	if(st.hardViolatedAndFeasOnly){
		return;
	}
	
	// Update the state matrix
	st.periodAssignment[mv.exam] = mv.to;
	st.roomAssignment[mv.exam] = mv.newRoom;

	// Update all redundant data
	st.decRoomOccupation(mv.oldRoom,mv.from,in.getNoExamStudents(mv.exam));
	st.incRoomOccupation(mv.newRoom,mv.to,in.getNoExamStudents(mv.exam));
	st.decRoomExams(mv.oldRoom,mv.from);
	st.incRoomExams(mv.newRoom,mv.to);
	if(in.getExamAlone(mv.exam)){
		st.decRoomAlone(mv.oldRoom, mv.from);
		st.incRoomAlone(mv.newRoom, mv.to);
	}
	st.decRoomDurations(mv.oldRoom,mv.from,in.getExamDurationIndex(mv.exam));
	st.incRoomDurations(mv.newRoom,mv.to,in.getExamDurationIndex(mv.exam));

	int a, e2;
	for(a=0; a < in.noConflicts(mv.exam); a++){
		e2 = in.getConflictList(mv.exam, a);
		st.decExamPeriodConflicts(e2, mv.from, in.getConflict(mv.exam,e2));
		st.incExamPeriodConflicts(e2, mv.to, in.getConflict(mv.exam,e2));
	}	
#ifdef PRINTOBJECTIVE
	std::cout << "Move: " << sm.CostFunctionComponents(st) << std::endl;
#endif
}  

//void ETT_MoveNeighborhoodExplorer::FirstMove(const ETT_State& st, ETT_MoveExam& mv) const {
//	// Start with exam 0, and determine the period and room currently assigned
//	mv.exam = 0;
//	while(st.weights[mv.exam]==0){
//		// Exam has >1 feasible rooms and/or >1 feasible periods
//		mv.exam++;
//	}
//
//	mv.from = st.periodAssignment[mv.exam];
//	mv.fromIndex = in.getValidPeriodIndex(mv.exam, mv.from);
//	mv.oldRoom = st.roomAssignment[mv.exam];	
//	mv.oldRoomIndex = in.getValidRoomIndex(mv.exam, mv.oldRoom);
//
//	// The first new period is period 0, unless there is only one valid room and exam e is currently assigned to period 0
//	firstNewPeriod(st, mv);
//
//	// The first newRoom is room 0, unless the current room is room 0 in which case we pick room 1
//	firstNewRoom(st, mv); 		
//}
//
void ETT_MoveNeighborhoodExplorer::FirstMove(const ETT_State& st, ETT_MoveExam& mv) const {
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	// Start with exam 0, and determine the period and room currently assigned
	mv.exam = 0;

	mv.from = st.periodAssignment[mv.exam];
	mv.oldRoom = st.roomAssignment[mv.exam];	

	mv.to = in.getValidPeriod(mv.exam, 0);
	mv.newRoom = in.getValidRoom(mv.exam, 0);
	mv.deltaConflicts = st.getExamPeriodConflicts(mv.exam, mv.to) - st.getExamPeriodConflicts(mv.exam, mv.from);

	while((mv.from == mv.to && mv.newRoom == mv.oldRoom) || !FeasibleMove(st,mv)){
		AnyNextMove(st,mv);
	}
}

bool ETT_MoveNeighborhoodExplorer::AnyNextMove(const ETT_State& st, ETT_MoveExam& mv) const {
	// First we try to increase newRoom
	int newRoomIndex = in.getValidRoomIndex(mv.exam, mv.newRoom);
	int toIndex = in.getValidPeriodIndex(mv.exam, mv.to);

	if(newRoomIndex < in.getNoValidRooms(mv.exam) - 1){
		mv.newRoom = in.getValidRoom(mv.exam, newRoomIndex+1);
	} else if(toIndex < in.getNoValidPeriods(mv.exam) - 1){// Then to
		mv.to = in.getValidPeriod(mv.exam, toIndex+1);
		mv.newRoom = in.getValidRoom(mv.exam, 0);
	} else if(mv.exam < in.getNoExams() - 1){ // Then exam
		mv.exam++;
		mv.from = st.periodAssignment[mv.exam];
		mv.oldRoom = st.roomAssignment[mv.exam];	
		mv.to = in.getValidPeriod(mv.exam, 0);
		mv.newRoom = in.getValidRoom(mv.exam, 0);
	} else {
		// No next move exists
		return false;
	}

	mv.deltaConflicts = st.getExamPeriodConflicts(mv.exam, mv.to) - st.getExamPeriodConflicts(mv.exam, mv.from);
	return true;
}


bool ETT_MoveNeighborhoodExplorer::NextMove(const ETT_State& st, ETT_MoveExam& mv) const {
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	do {
		if(!AnyNextMove(st, mv)){
			return false;
		}
	} while((mv.from == mv.to && mv.newRoom == mv.oldRoom) || !FeasibleMove(st,mv));
	return true;
}

//bool ETT_MoveNeighborhoodExplorer::NextMove(const ETT_State& st, ETT_MoveExam& mv) const {
//	// First we try to increase the room, only then the period, and as a last resort the exam
//	// Return true if a next move exists, false otherwise
//
//        if(nextNewRoom(st,mv)){ // First try to change the room
//                return true;
//        } else if (nextNewPeriod(st, mv)){
//                firstNewRoom(st, mv);
//                return true;
//        } else if(mv.exam < in.getNoExams() - 1){ 
//		while(st.weights[++mv.exam]==0){
//			// Exam has >1 feasible rooms and/or >1 feasible periods
//		}
//		mv.from = st.periodAssignment[mv.exam];
//		mv.fromIndex = in.getValidPeriodIndex(mv.exam, mv.from);
//		mv.oldRoom = st.roomAssignment[mv.exam];
//		mv.oldRoomIndex = in.getValidRoomIndex(mv.exam, mv.oldRoom);
//
//		// Determine the first new period and room
//		firstNewPeriod(st, mv);
//		firstNewRoom(st, mv);
//
//                return true;
//        } else {
//                return false;
//        }
//}

//void ETT_MoveNeighborhoodExplorer::firstNewRoom(const ETT_State& st, ETT_MoveExam& mv) const{
//	if(mv.from == mv.to && mv.oldRoom == in.getValidRoom(mv.exam, 0) && in.getNoValidRooms(mv.exam) > 1){
//		// Room 0 would result in a null move
//		mv.newRoomIndex= 1;
//		mv.newRoom = in.getValidRoom(mv.exam, 1);
//	} else {
//		mv.newRoomIndex = 0;
//		mv.newRoom = in.getValidRoom(mv.exam, 0);
//	}
//}
//
//
//bool ETT_MoveNeighborhoodExplorer::nextNewRoom(const ETT_State& st, ETT_MoveExam& mv) const{
//        mv.newRoomIndex++;
//	if(mv.newRoomIndex == mv.oldRoomIndex && mv.fromIndex == mv.toIndex){ // skip null move
//		mv.newRoomIndex++;
//	}
//
//	if(mv.newRoomIndex >= in.getNoValidRooms(mv.exam)){
//		return false;
//	} else {
//		mv.newRoom = in.getValidRoom(mv.exam, mv.newRoomIndex);
//		return true;
//	}
//}
//
//void ETT_MoveNeighborhoodExplorer::firstNewPeriod(const ETT_State& st, ETT_MoveExam& mv) const{
//	mv.toIndex = 0;
//	mv.to = in.getValidPeriod(mv.exam, mv.toIndex);	
//	int conflicts = st.getExamPeriodConflicts(mv.exam, mv.from);
//	while ((mv.toIndex == mv.fromIndex && in.getNoValidRooms(mv.exam) == 1) || st.getExamPeriodConflicts(mv.exam, mv.to) > conflicts){
//		mv.toIndex++;
//		mv.to = in.getValidPeriod(mv.exam, mv.toIndex);	
//	}
//}
//
//bool ETT_MoveNeighborhoodExplorer::nextNewPeriod(const ETT_State& st, ETT_MoveExam& mv) const{
//	int conflicts = st.getExamPeriodConflicts(mv.exam, mv.from);
//	do {
//        	mv.toIndex++;
//		if(mv.toIndex >= in.getNoValidPeriods(mv.exam)){
//			return false;
//		}
//		mv.to = in.getValidPeriod(mv.exam, mv.toIndex);
//	} while ((mv.toIndex == mv.fromIndex && in.getNoValidRooms(mv.exam) == 1) || st.getExamPeriodConflicts(mv.exam, mv.to) > conflicts);
//
//	return true;
//}

/************************************
*  ETT_Swap neighborhood explorer  *
************************************/
// initial swap builder
void ETT_SwapNeighborhoodExplorer::RandomMove(const ETT_State& st, ETT_SwapExam& swp) const {
	// Bias-free version: each move has an equal prob. to be selected
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	int cntr=0;
	do {
		// Choose a random pair of exams and determine all other data
		swp.exam1 = EasyLocal::Core::Random::Uniform<int>(0, in.getNoExams()-1);	

		// Randomly pick a second exam
		swp.exam2 = EasyLocal::Core::Random::Uniform<int>(0, in.getNoExams()-2);
		if(swp.exam2 >= swp.exam1){
			swp.exam2++;
		} else {
			std::swap(swp.exam1,swp.exam2);
		}

		// Retrieve all other data
		swp.period1 = st.periodAssignment[swp.exam1];
		swp.period2 = st.periodAssignment[swp.exam2];
		swp.room1 = st.roomAssignment[swp.exam1];
		swp.room2 = st.roomAssignment[swp.exam2];

		if(swp.period1 == swp.period2){
			swp.deltaConflicts = 0;
		} else {
			swp.deltaConflicts = st.getExamPeriodConflicts(swp.exam1, swp.period2)+
				st.getExamPeriodConflicts(swp.exam2, swp.period1)
				-2*in.getConflict(swp.exam1,swp.exam2)
				-st.getExamPeriodConflicts(swp.exam1, swp.period1)
				-st.getExamPeriodConflicts(swp.exam2, swp.period2);
		}
		if(cntr++>MOVETHRESHOLD){
#ifndef NDEBUG
			std::cout << "Empty swap" << std::endl;
#endif
        		throw EasyLocal::Core::EmptyNeighborhood();
		}
	} while(!FeasibleMove(st,swp));
}

// check move feasibility
bool ETT_SwapNeighborhoodExplorer::FeasibleMove(const ETT_State& st, const ETT_SwapExam& swp) const {
	// Insert the code that check is swap swp is legal in state st 
	// (return true if legal, false otherwise)

	assert(swp.exam1 < swp.exam2);

	return in.isValidPeriod(swp.exam1, swp.period2) && in.isValidPeriod(swp.exam2, swp.period1)
		&& in.isValidRoom(swp.exam1, swp.room2) && in.isValidRoom(swp.exam2, swp.room1)
		&& (swp.room1 != swp.room2 || swp.period1 != swp.period2)
		&& swp.deltaConflicts <= 0;
} 

// update the state according to the move 
void ETT_SwapNeighborhoodExplorer::MakeMove(ETT_State& st, const ETT_SwapExam& swp) const {
	// Insert the code that modifies the state st based on the application of move swp

	if(st.hardViolatedAndFeasOnly){
		return;
	}
	
	// Update the state matrix
	st.periodAssignment[swp.exam1] = swp.period2;
	st.periodAssignment[swp.exam2] = swp.period1;
	st.roomAssignment[swp.exam1] = swp.room2;
	st.roomAssignment[swp.exam2] = swp.room1;

	// Update redundant data
	// Number of exams stays the same in each period
	// --> No change in roomExams structure
	int studentDiff = in.getNoExamStudents(swp.exam1) - in.getNoExamStudents(swp.exam2);
	if(studentDiff > 0){ // exam1 > exam2
		st.decRoomOccupation(swp.room1, swp.period1, studentDiff);
		st.incRoomOccupation(swp.room2, swp.period2, studentDiff);
	} else if(studentDiff < 0) { // exam1 < exam2
		st.incRoomOccupation(swp.room1, swp.period1, -studentDiff);
		st.decRoomOccupation(swp.room2, swp.period2, -studentDiff);
	}
	if(in.getExamAlone(swp.exam1)){
		st.decRoomAlone(swp.room1, swp.period1);
		st.incRoomAlone(swp.room2, swp.period2);
	}
	if(in.getExamAlone(swp.exam2)){
		st.decRoomAlone(swp.room2, swp.period2);
		st.incRoomAlone(swp.room1, swp.period1);
	}
	st.decRoomDurations(swp.room1, swp.period1, in.getExamDurationIndex(swp.exam1));
	st.decRoomDurations(swp.room2, swp.period2, in.getExamDurationIndex(swp.exam2));
	st.incRoomDurations(swp.room2, swp.period2, in.getExamDurationIndex(swp.exam1));
	st.incRoomDurations(swp.room1, swp.period1, in.getExamDurationIndex(swp.exam2));

	const_cast<ETT_State&>(st);
	int a, e3;


	for(a=0; a < in.noConflicts(swp.exam1); a++){
		e3 = in.getConflictList(swp.exam1, a);
		st.decExamPeriodConflicts(e3, swp.period1, in.getConflict(swp.exam1,e3));
		st.incExamPeriodConflicts(e3, swp.period2, in.getConflict(swp.exam1,e3));
	}	
	for(a=0; a < in.noConflicts(swp.exam2); a++){
		e3 = in.getConflictList(swp.exam2, a);
		st.decExamPeriodConflicts(e3, swp.period2, in.getConflict(swp.exam2,e3));
		st.incExamPeriodConflicts(e3, swp.period1, in.getConflict(swp.exam2,e3));
	}		
#ifdef PRINTOBJECTIVE
	std::cout << "Swap: " << sm.CostFunctionComponents(st) << std::endl;
#endif
}  

void ETT_SwapNeighborhoodExplorer::FirstMove(const ETT_State& st, ETT_SwapExam& swp) const {
	// Need to consider the case where no feasible swap exists for exam1=0
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	swp.exam2 = in.getNoExams();
	for(int i = 0; swp.exam2 == in.getNoExams() && i < in.getNoExams(); ++i){
		swp.exam1 = i;
		swp.period1 = st.periodAssignment[swp.exam1];
		swp.room1 = st.roomAssignment[swp.exam1];

		// Exam2 is the first following exam for which the period and room assignment is valid
		for(swp.exam2=i+1; swp.exam2 < in.getNoExams(); ++swp.exam2){
			swp.period2 = st.periodAssignment[swp.exam2];
			swp.room2 = st.roomAssignment[swp.exam2];		

			if(swp.period1 == swp.period2){
				swp.deltaConflicts = 0;
			} else {
				swp.deltaConflicts = st.getExamPeriodConflicts(swp.exam1, swp.period2)+
					st.getExamPeriodConflicts(swp.exam2, swp.period1)
					-2*in.getConflict(swp.exam1,swp.exam2)
					-st.getExamPeriodConflicts(swp.exam1, swp.period1)
					-st.getExamPeriodConflicts(swp.exam2, swp.period2);
			}

			if(FeasibleMove(st,swp)){
				break;
			}
		}
	}
}

bool ETT_SwapNeighborhoodExplorer::NextMove(const ETT_State& st, ETT_SwapExam& swp) const {
	// First try to increase exam2, only then exam1
	// Return true if a next swap exists, false otherwise
	
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	if(nextExam2(st,swp)){ // Increase exam2
		return true;
	} else if(nextExam1(st,swp)){ // Increase exam1 and set first exam2
		return true;
	} else { // No next swap
		return false;
	}
}

bool ETT_SwapNeighborhoodExplorer::nextExam2(const ETT_State& st, ETT_SwapExam& swp) const{
	// Exam2 is the first following exam for which the period and room assignment is valid
	for(++swp.exam2; swp.exam2 < in.getNoExams(); ++swp.exam2){
		swp.period2 = st.periodAssignment[swp.exam2];
		swp.room2 = st.roomAssignment[swp.exam2];		
		if(swp.period1 == swp.period2){
			swp.deltaConflicts = 0;
		} else {		
			swp.deltaConflicts = st.getExamPeriodConflicts(swp.exam1, swp.period2)+
				st.getExamPeriodConflicts(swp.exam2, swp.period1)
				-2*in.getConflict(swp.exam1,swp.exam2)
				-st.getExamPeriodConflicts(swp.exam1, swp.period1)
				-st.getExamPeriodConflicts(swp.exam2, swp.period2);
		}

		if(FeasibleMove(st,swp)){
			return true;
		}
	}
	return false;
}

bool ETT_SwapNeighborhoodExplorer::nextExam1(const ETT_State& st, ETT_SwapExam& swp) const{
	// Exam1 is the first following exam for which a valid swap exists
	for(; ++swp.exam1 < in.getNoExams()-1;){
		swp.period1 = st.periodAssignment[swp.exam1];
		swp.room1 = st.roomAssignment[swp.exam1];
		swp.exam2 = swp.exam1;
		if(nextExam2(st, swp)){
			return true;
		} 
	}
	return false;
}

/***********************************
*  ETT_Kick neighborhood explorer  *
***********************************/

void ETT_KickNeighborhoodExplorer::AnyRandomMove(const ETT_State& st, ETT_KickExam& kick) const {
	// Decide whether we perform a swap or a kick

	// Randomly pick two different exams
//	do {
		kick.exam1 = EasyLocal::Core::Random::Uniform<int>(0, in.getNoExams()-1);	
		if(kick.roomOnly){
	
	
			std::shuffle(const_cast<ETT_KickNeighborhoodExplorer*>(this)->randomExamIds.begin(), const_cast<ETT_KickNeighborhoodExplorer*>(this)->randomExamIds.end(), EasyLocal::Core::Random::GetGenerator());
			const int p = st.periodAssignment[kick.exam1];
			for (auto& i : randomExamIds) {
				if(st.periodAssignment[i] == p){
					// There is always such an exam, possible the exam itself...
					kick.exam2 = i;
					break;
				}
			}

			if(kick.swap && kick.exam1 > kick.exam2){
				// In the regular kick: the order matters!
				std::swap(kick.exam1,kick.exam2);
			}
		} else {
			// Small probability that the move is still a room only move
			kick.exam2 = EasyLocal::Core::Random::Uniform<int>(0, in.getNoExams()-2);
			if(kick.exam2 >= kick.exam1){
				kick.exam2++;
			} else if(kick.swap) {
				// In the regular kick: the order matters!
				std::swap(kick.exam1,kick.exam2);
			}
		}


		// Retrieve all other data
		kick.period1 = st.periodAssignment[kick.exam1];
		kick.period2 = st.periodAssignment[kick.exam2];
		kick.room1 = st.roomAssignment[kick.exam1];
		kick.room2 = st.roomAssignment[kick.exam2];		
//	} while (!in.isValidPeriod(kick.exam1, kick.period2) || !in.isValidRoom(kick.exam1, kick.room2) || (kick.swap && (!in.isValidPeriod(kick.exam2, kick.period1) || !in.isValidRoom(kick.exam2, kick.room1))));


	// Choose a new period and a new room for exam2
	if(kick.swap){
		kick.newPeriod = kick.period1;
		kick.newRoom = kick.room1;
	} else {
		// Note: small prob that it is in fact a swap --> check in FeasibleMove
		int newPeriodIndex = EasyLocal::Core::Random::Uniform<int>(0, in.getNoValidPeriods(kick.exam2)-1);
		kick.newPeriod = in.getValidPeriod(kick.exam2, newPeriodIndex);

		int newRoomIndex = EasyLocal::Core::Random::Uniform<int>(0, in.getNoValidRooms(kick.exam2)-1);
		kick.newRoom = in.getValidRoom(kick.exam2, newRoomIndex);
	}

	kick.deltaConflicts = 0;
	if(kick.period1 != kick.period2){
		kick.deltaConflicts += st.getExamPeriodConflicts(kick.exam1, kick.period2)
				- st.getExamPeriodConflicts(kick.exam1, kick.period1);

		if(kick.period2 != kick.newPeriod){
			kick.deltaConflicts -= in.getConflict(kick.exam1,kick.exam2);
		}
	}
	if(kick.period2 != kick.newPeriod){
		kick.deltaConflicts += st.getExamPeriodConflicts(kick.exam2, kick.newPeriod)
				- st.getExamPeriodConflicts(kick.exam2, kick.period2);
		if(kick.newPeriod == kick.period1 && kick.period1 != kick.period2){
			kick.deltaConflicts -= in.getConflict(kick.exam1,kick.exam2);
		}
	} 
}

void ETT_KickNeighborhoodExplorer::RandomMove(const ETT_State& st, ETT_KickExam& kick) const {
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;

	kick.swap = EasyLocal::Core::Random::Uniform<double>(0.0, 1.0) < swapRate;
	kick.roomOnly = EasyLocal::Core::Random::Uniform<double>(0.001, 1.0) < roomRate;
	int cntr=0;
	do {
		if(cntr++>MOVETHRESHOLD){
#ifndef NDEBUG
			std::cout << "Empty kick" << std::endl;
#endif
        		throw EasyLocal::Core::EmptyNeighborhood();
		}
		AnyRandomMove(st,kick);
	} while (!FeasibleMove(st,kick));
}

// check move feasibility
bool ETT_KickNeighborhoodExplorer::FeasibleMove(const ETT_State& st, const ETT_KickExam& kick) const {
	// Insert the code that check is move mv is legal in state st 
	// (return true if legal, false otherwise)
	
	assert(!kick.roomOnly || kick.period1 == kick.period2);
	
	return kick.deltaConflicts <= 0 
		&& kick.exam1 != kick.exam2 // Possible in roomOnly
		&& in.isValidPeriod(kick.exam1, kick.period2) && in.isValidPeriod(kick.exam2, kick.newPeriod)
		&& in.isValidRoom(kick.exam1, kick.room2) && in.isValidRoom(kick.exam2, kick.newRoom)
		&& (kick.room1 != kick.room2 || kick.period1 != kick.period2) // Null swap or kick=move
		&& (kick.room2 != kick.newRoom || kick.period2 != kick.newPeriod) // Null swap or kick = move
		&& (kick.swap == (kick.newRoom == kick.room1 && kick.newPeriod == kick.period1)) // swap correctly set
		&& (kick.exam1 < kick.exam2 || (!kick.swap && (kick.exam1 != kick.exam2))); // If swap: e1 < e2; else: e1 != e2	
} 

bool ETT_KickNeighborhoodExplorer::AnyNextMove(const ETT_State& st, ETT_KickExam& kick) const {
	// First we try to increase newRoom
	int newRoomIndex = in.getValidRoomIndex(kick.exam2, kick.newRoom);
	if(newRoomIndex < in.getNoValidRooms(kick.exam2) - 1){
		kick.newRoom = in.getValidRoom(kick.exam2, newRoomIndex+1);
		kick.swap = (kick.newPeriod == kick.period1 && kick.newRoom == kick.room1);


		return true;
	}

	// Then newPeriod
	int newPeriodIndex = in.getValidPeriodIndex(kick.exam2, kick.newPeriod);
	if(newPeriodIndex < in.getNoValidPeriods(kick.exam2) - 1){
		kick.newPeriod = in.getValidPeriod(kick.exam2, newPeriodIndex+1);
		kick.newRoom = in.getValidRoom(kick.exam2, 0);
		kick.swap = (kick.newPeriod == kick.period1 && kick.newRoom == kick.room1);
	} else if(kick.exam2 < in.getNoExams() - 1){ // Then exam2
		kick.exam2++;
		kick.period2 = st.periodAssignment[kick.exam2];
		kick.room2 = st.roomAssignment[kick.exam2];		
		kick.newPeriod = in.getValidPeriod(kick.exam2, 0);
		kick.newRoom = in.getValidRoom(kick.exam2, 0);
		kick.swap = (kick.newPeriod == kick.period1 && kick.newRoom == kick.room1);
	} else if(kick.exam1 < in.getNoExams() - 1){ // Finally exam1
		kick.exam1++;
		kick.period1 = st.periodAssignment[kick.exam1];
		kick.room1 = st.roomAssignment[kick.exam1]; 
		kick.exam2 = 0;
		kick.period2 = st.periodAssignment[0];
		kick.room2 = st.roomAssignment[0];		
		kick.newPeriod = in.getValidPeriod(kick.exam2, 0);
		kick.newRoom = in.getValidRoom(kick.exam2, 0);
		kick.swap = (kick.newPeriod == kick.period1 && kick.newRoom == kick.room1);
	} else {
		// No next move exists
		return false;
	}

	kick.deltaConflicts = 0;
	if(kick.period1 != kick.period2){
		kick.deltaConflicts += st.getExamPeriodConflicts(kick.exam1, kick.period2)
				- st.getExamPeriodConflicts(kick.exam1, kick.period1);

		if(kick.period2 != kick.newPeriod){
			kick.deltaConflicts -= in.getConflict(kick.exam1,kick.exam2);
		}
	}
	if(kick.period2 != kick.newPeriod){
		kick.deltaConflicts += st.getExamPeriodConflicts(kick.exam2, kick.newPeriod)
				- st.getExamPeriodConflicts(kick.exam2, kick.period2);
		if(kick.newPeriod == kick.period1 && kick.period1 != kick.period2){
			kick.deltaConflicts -= in.getConflict(kick.exam1,kick.exam2);
		}
	} 

	return true;
}

// update the state according to the move 
void ETT_KickNeighborhoodExplorer::MakeMove(ETT_State& st, const ETT_KickExam& kick) const {
	// Insert the code that modifies the state st based on the application of move mv

	if(st.hardViolatedAndFeasOnly){
		return;
	}
	
	// Update the state matrix
	st.periodAssignment[kick.exam1] = kick.period2;
	st.roomAssignment[kick.exam1] = kick.room2;
	st.periodAssignment[kick.exam2] = kick.newPeriod;
	st.roomAssignment[kick.exam2] = kick.newRoom;


	// Update all redundant data
	st.decRoomOccupation(kick.room1, kick.period1, in.getNoExamStudents(kick.exam1));
	st.decRoomOccupation(kick.room2, kick.period2, in.getNoExamStudents(kick.exam2));
	st.incRoomOccupation(kick.room2, kick.period2, in.getNoExamStudents(kick.exam1));
	st.incRoomOccupation(kick.newRoom, kick.newPeriod, in.getNoExamStudents(kick.exam2));

	if(!kick.swap){
		st.decRoomExams(kick.room1, kick.period1);
		st.incRoomExams(kick.newRoom, kick.newPeriod);
	}

	if(in.getExamAlone(kick.exam1)){
		st.decRoomAlone(kick.room1, kick.period1);
		st.incRoomAlone(kick.room2, kick.period2);
	}
	if(in.getExamAlone(kick.exam2)){
		st.decRoomAlone(kick.room2, kick.period2);
		st.incRoomAlone(kick.newRoom, kick.newPeriod);
	}

	st.decRoomDurations(kick.room1,kick.period1,in.getExamDurationIndex(kick.exam1));
	st.decRoomDurations(kick.room2,kick.period2,in.getExamDurationIndex(kick.exam2));
	st.incRoomDurations(kick.room2,kick.period2,in.getExamDurationIndex(kick.exam1));
	st.incRoomDurations(kick.newRoom,kick.newPeriod,in.getExamDurationIndex(kick.exam2));

	const_cast<ETT_State&>(st);
	int a, e3;
	for(a=0; a < in.noConflicts(kick.exam1); a++){
		e3 = in.getConflictList(kick.exam1, a);
		st.decExamPeriodConflicts(e3, kick.period1, in.getConflict(kick.exam1,e3));
		//if(in.isValidPeriod(e3,kick.period1) && st.getExamPeriodConflicts(e3,kick.period1) == 0){
		//	st.weights[e3] += in.getNoValidRooms(e3);
		//}
		//if(in.isValidPeriod(e3,kick.period2) && st.getExamPeriodConflicts(e3,kick.period2) == 0){
		//	st.weights[e3] -= in.getNoValidRooms(e3);
		//}
		st.incExamPeriodConflicts(e3, kick.period2, in.getConflict(kick.exam1,e3));
	}	
	for(a=0; a < in.noConflicts(kick.exam2); a++){
		e3 = in.getConflictList(kick.exam2, a);
		st.decExamPeriodConflicts(e3, kick.period2, in.getConflict(kick.exam2,e3));
		//if(in.isValidPeriod(e3,kick.period2) && st.getExamPeriodConflicts(e3,kick.period2) == 0){
		//	st.weights[e3] += in.getNoValidRooms(e3);
		//}
		//if(in.isValidPeriod(e3,kick.newPeriod) && st.getExamPeriodConflicts(e3,kick.newPeriod) == 0){
		//	st.weights[e3] -= in.getNoValidRooms(e3);
		//}
		st.incExamPeriodConflicts(e3, kick.newPeriod, in.getConflict(kick.exam2,e3));
	}		
	//st.distribution = std::discrete_distribution<int>(st.weights.begin(), st.weights.end());
#ifdef PRINTOBJECTIVE
	std::cout << "Kick: " << sm.CostFunctionComponents(st) << std::endl;
#endif

}  

void ETT_KickNeighborhoodExplorer::FirstMove(const ETT_State& st, ETT_KickExam& kick) const {
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	kick.exam1 = 0;
	kick.period1 = st.periodAssignment[0];
	kick.room1 = st.roomAssignment[0];

	kick.exam2 = 1;
	kick.period2 = st.periodAssignment[1];
	kick.room2 = st.roomAssignment[1];

	kick.newRoom = in.getValidRoom(kick.exam2, 0);
	kick.newPeriod = in.getValidPeriod(kick.exam2, 0);

	kick.swap = (kick.newPeriod == kick.period1 && kick.newRoom == kick.room1);

	kick.deltaConflicts = 0;
	if(kick.period1 != kick.period2){
		kick.deltaConflicts += st.getExamPeriodConflicts(kick.exam1, kick.period2)
				- st.getExamPeriodConflicts(kick.exam1, kick.period1);

		if(kick.period2 != kick.newPeriod){
			kick.deltaConflicts -= in.getConflict(kick.exam1,kick.exam2);
		}
	}
	if(kick.period2 != kick.newPeriod){
		kick.deltaConflicts += st.getExamPeriodConflicts(kick.exam2, kick.newPeriod)
				- st.getExamPeriodConflicts(kick.exam2, kick.period2);
		if(kick.newPeriod == kick.period1 && kick.period1 != kick.period2){
			kick.deltaConflicts -= in.getConflict(kick.exam1,kick.exam2);
		}
	} 

	while (!FeasibleMove(st, kick)) {
		AnyNextMove(st, kick);	
	}
}

// Remark: there are two options to program this function
// Option 1: enumerate only feasible moves in NextMove
// --> Slightly more efficient, but lots of if conditions that need to be checked
// Option 2: enumerate all possible moves, only then check whether it is feasible
// --> Somewhat less efficient, but code is much easier to verify for correctness...
// I opted for option 2, see commit befor 16/03/2022 for option 1
bool ETT_KickNeighborhoodExplorer::NextMove(const ETT_State& st, ETT_KickExam& kick) const {
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	do {
		if(!AnyNextMove(st, kick)){
			return false;
		}
	} while (!FeasibleMove(st,kick));
	return true;
}

/************************************
*  ETT_Kempe neighborhood explorer  *
************************************/

void ETT_KempeNeighborhoodExplorer::AnyRandomMove(const ETT_State& st, ETT_KempeExam& kc) const {
	// Try to avoid as much as possible to construct an expensive chain or perform an expensive room assignment
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	int cntr = 0;
	do {
		// A small bias towards exams that have more flexibility to be moved around
		kc.exam = const_cast<ETT_KempeNeighborhoodExplorer*>(this)->distribution(EasyLocal::Core::Random::GetGenerator());
		//mv.exam = EasyLocal::Core::Random::Uniform<int>(0, in.getNoExams()-1);	
		
		// Determine the old room and period
		kc.from = st.periodAssignment[kc.exam];

		// Determine a new time slot during which the exam has at least one conflict
		int toIndex = EasyLocal::Core::Random::Uniform<int>(0, in.getNoValidPeriods(kc.exam)-1);
		kc.to = in.getValidPeriod(kc.exam, toIndex);

		// Do not construct the chain if we reject it anyway
		if (kc.to != kc.from && st.getExamPeriodConflicts(kc.exam, kc.to) > 0) {
			CreateChain(st, kc);
		}
	} while (
		(kc.to == kc.from) 					// Null move
		|| (st.getExamPeriodConflicts(kc.exam, kc.to) == 0) 	// Chain of length zero
		|| (kc.s2.size() == 0) 					// No conflict or conflict which we cannot move because of valid period: simple move
		// || (kc.s1.size() == 1 && kc.s2.size() == 1) 		// Simple swap: allow, reason see FeasibleMove
	);

	// The move is feasible!
	// Determine room assignment
	kc.oldRoom = st.roomAssignment[kc.exam];
	if(assignNewRoom){
		int newRoomIndex = EasyLocal::Core::Random::Uniform<int>(0, in.getNoValidRooms(kc.exam)-1);
		kc.newRoom = in.getValidRoom(kc.exam, newRoomIndex);		
	} else {
		kc.newRoom = -1;
	}
	if(roomMode){
		BeamFit(st, kc);
	} else {
		TightestFit(st, kc);
	}
}

void ETT_KempeNeighborhoodExplorer::CreateChain(const ETT_State& st, ETT_KempeExam& kc) const {
	// creates the sets s1 and s2 
	std::unordered_set<int> new_insertions, previous_insertions;
	
	kc.s1.clear();
	kc.s2.clear();


	bool direction_s1_s2 = true;
	new_insertions.insert(kc.exam);
	while (new_insertions.size() > 0){
		if (direction_s1_s2){
			kc.s1.insert(new_insertions.begin(), new_insertions.end());
			previous_insertions = new_insertions;
			new_insertions.clear();
			ComputeNewInsertions(st,kc.s2,previous_insertions,new_insertions,kc.to,kc.from);
		} else {
			kc.s2.insert(new_insertions.begin(), new_insertions.end());	
			previous_insertions = new_insertions;
			new_insertions.clear();
			ComputeNewInsertions(st,kc.s1,previous_insertions,new_insertions,kc.from,kc.to);
		}
		direction_s1_s2 = !direction_s1_s2;	  
	}
}

void ETT_KempeNeighborhoodExplorer::ComputeNewInsertions(const ETT_State& st, const std::unordered_set<int>& current, const std::unordered_set<int>& to_be_checked, std::unordered_set<int>& new_elements, int p, int s) const {  
	// given the current state of a set (s1 or s2) and the new elements of the 
	// other set (to_be_checked), compute the new elements of the first one
 	int e, e2, j; 
	std::unordered_set<int>::const_iterator it;
	for (it = to_be_checked.begin(); it != to_be_checked.end(); ++it){
		e = *it;
		for (j = 0; j < in.noConflicts(e); j++){
			e2 = in.getConflictList(e,j);
			if(st.periodAssignment[e2] == p && !current.count(e2) && in.isValidPeriod(e2,s)){
				new_elements.insert(e2);        		
			}
		}		 
	}
}

void ETT_KempeNeighborhoodExplorer::BeamFit(const ETT_State& st, ETT_KempeExam& kc) const{
	int period = kc.to;
	std::unordered_set<int> other = kc.s2;
	kc.newCapViol = 0;
	kc.newRoomAssignment.resize(in.getNoExams());


	for(auto& chain : {kc.s1, kc.s2}){
		// Step 1. Initialize starting redundant data 
		// (cheaper than copying an entire state at a node)
		node* root = new node;
		root->isOldAssignment = true;
		root->totCost = 0;
		root->capCost = 0, root->durCost = 0, root->exclCost = 0, root->roomCost = 0, root->totCost = 0;
		root->level = -1;
		int nodeId = 0;
		root->nodeId = nodeId;
		nodeId++;
		root->randCost = EasyLocal::Core::Random::Uniform<float>(0, 1);
		root->residualCap.resize(in.getNoRooms());
		root->roomExams.resize(in.getNoRooms());
		root->roomAlone.resize(in.getNoRooms());
		root->durations.resize(in.getNoRooms());

		// Set state at root node
		int r,i;
		for (r = 0; r < in.getNoRooms(); ++r) {
			root->residualCap[r] = in.getRoomCapacity(r) - st.getRoomOccupation(r,period);	
			root->roomExams[r] = st.getRoomExams(r,period);	
			root->roomAlone[r] = st.getRoomAlone(r,period);
			root->durations[r].resize(in.getNoUniqueExamDurations());
			for (i = 0; i < in.getNoUniqueExamDurations(); ++i) {
				root->durations[r][i] = st.getRoomDurations(r,period,i);	
			}
		}

		// "Remove" all room assignments from the other chain (will move to period kc.to)
		for(auto& e : other){
			assert(st.periodAssignment[e] == period);
			r = st.roomAssignment[e];
			root->residualCap[r] += in.getNoExamStudents(e);
			root->roomExams[r]--;
			if(in.getExamAlone(e)) root->roomAlone[r]--;
			root->durations[r][in.getExamDurationIndex(e)]--;
		}

		for (r = 0; r < in.getNoRooms(); ++r) {
			// Cost of exams already there
			kc.newCapViol += std::max(-root->residualCap[r], 0);
		}

		// Room assgined in specific node
		root->assignment.resize(chain.size(), -1);

		// Step 2. Construct the tree
		std::vector<node*> current = {};
		current.reserve(beamwidth*in.getNoRooms());
		std::vector<node*> prev = {root};
		prev.reserve(beamwidth*in.getNoRooms());

		// One level for each exam
		int level = 0;
		for(auto& e : chain){
			current.clear();
			current.reserve(beamwidth*in.getNoRooms());
			for(const auto& n : prev){
				// Try to assign exam to each possible room
				for(int r= 0; r < in.getNoRooms(); ++r){
					if(!in.isValidRoom(e, r)) continue;

					// Construct a new node object by copying data from the previous node
					// Wait, however, to copy expensive state vectors
					node* newNode = new node(n, r);
					newNode->level = level;
					newNode->nodeId = nodeId;
					newNode->randCost = EasyLocal::Core::Random::Uniform<float>(0, 1);

					// Calcule costs but wait to update states
					if(n->roomExams[r] > 0 && n->durations[r][in.getExamDurationIndex(e)] == 0){
						newNode->durCost += in.getNonMixedDurationsCost();
					}
					newNode->capCost += std::max(in.getNoExamStudents(e) - std::max(n->residualCap[r], 0), 0);
					newNode->exclCost += n->roomAlone[r];
					if(in.getExamAlone(e)){
						newNode->exclCost += n->roomExams[r];
					}
					newNode->roomCost += in.getRoomPenalty(r);	
					newNode->totCost = newNode->capCost + newNode->durCost + newNode->exclCost + newNode->roomCost;

					// No node can be equal to a prev node (or something is wrong in nodeComp)
					//assert(!current.count(newNode));

					current.push_back(newNode);
					nodeId++;
				}
			}

			// Reduce the size of each level to the beamwidth!
			std::partial_sort(current.begin(), current.begin() + std::min(beamwidth, (int) current.size()), current.end(), nodeComparison);

			std::vector<node*> temp(current.begin(), std::next(current.begin(), std::min(beamwidth, (int) current.size())));
			for(int dummy = std::min(beamwidth, (int) current.size()); dummy < current.size(); ++dummy){
				delete *std::next(current.begin(), dummy);
			}

			// Only now we do the state update: for much fewer nodes we have to make the expensive copy!
			for(auto& n : temp){
				// Deep copy from parent
				n->assignment = n->parent->assignment;
				n->durations = n->parent->durations;
				n->residualCap = n->parent->residualCap;
				n->roomExams = n->parent->roomExams;
				n->roomAlone = n->parent->roomAlone;
				
				// Update state
				r = n->lastRoom;
				n->assignment[level] = r;
				n->durations[r][in.getExamDurationIndex(e)]++;
				n->residualCap[r] -= in.getNoExamStudents(e);
				n->roomExams[r]++;
				if(in.getExamAlone(e)){
					n->roomAlone[r]++;
				}
			}
			for(auto& n : prev){
				delete n;
			}
			prev = temp;
			level++;
		}
		// Deep copy for memory clearing
		node* bestFound = *prev.begin();
		kc.newCapViol += bestFound->capCost;
		level = 0;
		for(auto& e : chain){
			kc.newRoomAssignment[e] = bestFound->assignment[level++];
		}
		for(auto& n : prev){
			delete n;
		}


		period = kc.from;
		other = kc.s1;
	}	

	return;
}

void ETT_KempeNeighborhoodExplorer::TightestFit(const ETT_State& st, ETT_KempeExam& kc) const{
	int period = kc.to;
	std::unordered_set<int> other = kc.s2;
	kc.newCapViol = 0;
	kc.newRoomAssignment.resize(in.getNoExams());

	for(auto& chain : {kc.s1, kc.s2}){
		// Step 1. Sort the exams in the chain in decreasing size of students
		// O(|E|log|E|)
		std::vector<std::pair<int,int>> examIds;
		for (auto& e : chain) {
			examIds.push_back(std::make_pair(e,in.getNoExamStudents(e)));
		}
		std::sort(examIds.begin(), examIds.end(), sortbysecdesc);

		// Step 2. Compute residual capacities
		// O(|R|)
		std::vector<int> residualCap;
		residualCap.reserve(in.getNoRooms());
		for (int r = 0; r < in.getNoRooms(); ++r) {
			residualCap.push_back(in.getRoomCapacity(r) - st.getRoomOccupation(r, period));
		}

		// "Remove" all room assignments from the other chain (will move to period kc.to)
		// O(|E|)
		for(auto& e : other){
			assert(st.periodAssignment[e] == period);
			residualCap[st.roomAssignment[e]] += in.getNoExamStudents(e);
		}

		// Step 3. Assign to tightest room which has enough capacity, or to the largest room if no such room exists
		for(auto& e: examIds){
			int bestRoom = -1;
			if(assignNewRoom && e.first == kc.exam){
				bestRoom = kc.newRoom;
			} else {
				int minViol = INT_MAX;
				for (int r = 0; r < in.getNoRooms(); ++r) {
					if(!in.isValidRoom(e.first, r)) continue;
					int dummy = residualCap[r] - e.second;
					if(bestRoom == -1 || (dummy >= 0 && (minViol < 0 || dummy < minViol)) || (dummy < 0 && dummy > minViol)){
						bestRoom = r;
						minViol = dummy;
					}
				}
			}
			assert(bestRoom != -1);
			residualCap[bestRoom] -= e.second;
			kc.newRoomAssignment[e.first] = bestRoom;
		}


		// Save some time in the delta's
		for (int r = 0; r < in.getNoRooms(); ++r) {
			kc.newCapViol += std::max(-residualCap[r], 0);
		}

		period = kc.from;
		other = kc.s1;
	}	

	return;
}


void ETT_KempeNeighborhoodExplorer::RandomMove(const ETT_State& st, ETT_KempeExam& kc) const {
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	do {
		AnyRandomMove(st,kc);
	} while (!FeasibleMove(st,kc));
}

// Check move feasibility
bool ETT_KempeNeighborhoodExplorer::FeasibleMove(const ETT_State& st, const ETT_KempeExam& kc) const {
	// Insert the code that checks whether move kc is legal in state st 
	// (return true if legal, false otherwise)
	
	assert(assignNewRoom ? kc.newRoom != -1 : kc.newRoom == -1);
	assert(in.isValidPeriod(kc.exam, kc.to));
	assert(!assignNewRoom || in.isValidRoom(kc.exam, kc.newRoom));

	
	return in.isValidPeriod(kc.exam, kc.to) 
		&& (!assignNewRoom || in.isValidRoom(kc.exam, kc.newRoom)) // Period dur >= exam dur, room cap >= exam size	
		&& (weights[kc.exam] > 0) // More than one valid room and/or period
		&& (kc.s2.size() > 0) // No conflict: simple move
		//&& !(kc.s1.size() == 1 && kc.s2.size() == 1) // Simple swap: but allow it, it may be expensive to compute a conflict free swap! (in the sense that it takes a lot of random iterations)
		//&& (st.getExamPeriodConflicts(kc.exam, kc.to) > 0) // Repair chain is needed, otherwise simple move
		&& (kc.to != kc.from); // Periods not the same
} 

// update the state according to the move 
void ETT_KempeNeighborhoodExplorer::MakeMove(ETT_State& st, const ETT_KempeExam& kc) const {
	// Insert the code that modifies the state st based on the application of move kc

	if(st.hardViolatedAndFeasOnly){
		return;
	}
	
	// Update the state matrix for all exams in s1
	int period1 = kc.from;
	int period2 = kc.to;
	int oldRoom, newRoom;
	int cntr=0;
	for(auto& chain : {kc.s1, kc.s2}){
		for(auto& e : chain){
			oldRoom = st.roomAssignment[e];
			newRoom = kc.newRoomAssignment[e];

			st.periodAssignment[e] = period2;
			st.roomAssignment[e] = newRoom;

			st.decRoomOccupation(oldRoom,period1,in.getNoExamStudents(e));
			st.incRoomOccupation(newRoom,period2,in.getNoExamStudents(e));
			st.decRoomExams(oldRoom,period1);
			st.incRoomExams(newRoom,period2);
			if(in.getExamAlone(e)){
				st.decRoomAlone(oldRoom, period1);
				st.incRoomAlone(newRoom, period2);
			}
			st.decRoomDurations(oldRoom,period1,in.getExamDurationIndex(e));
			st.incRoomDurations(newRoom,period2,in.getExamDurationIndex(e));

			for(int a=0; a < in.noConflicts(e); ++a){
				auto e2 = in.getConflictList(e, a);
				st.decExamPeriodConflicts(e2, period1, in.getConflict(e,e2));
				st.incExamPeriodConflicts(e2, period2, in.getConflict(e,e2));
			}
		}
		period1 = kc.to;
		period2 = kc.from;
	}

#ifdef PRINTOBJECTIVE
	std::cout << "Kempe: " << sm.CostFunctionComponents(st) << std::endl;
#endif
}  

void ETT_KempeNeighborhoodExplorer::FirstMove(const ETT_State& st, ETT_KempeExam& kc) const {
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	// Start with exam 0, and determine the period and room currently assigned
	kc.exam = 0;

	kc.from = st.periodAssignment[kc.exam];
	kc.oldRoom = st.roomAssignment[kc.exam];	

	kc.to = in.getValidPeriod(kc.exam, 0);
	kc.newRoom = (assignNewRoom ? in.getValidRoom(kc.exam, 0) : -1);

	CreateChain(st, kc);
	if(roomMode){
		BeamFit(st, kc);
	} else {
		TightestFit(st, kc);
	}

	while(!FeasibleMove(st,kc)){
		if (!AnyNextMove(st,kc)){
			// No next move exists
			break;
		}
	}
}

bool ETT_KempeNeighborhoodExplorer::AnyNextMove(const ETT_State& st, ETT_KempeExam& kc) const {
	// First we try to increase newRoom
	int newRoomIndex = (assignNewRoom ? in.getValidRoomIndex(kc.exam, kc.newRoom) : -1);
	int toIndex = in.getValidPeriodIndex(kc.exam, kc.to);

	if(assignNewRoom && newRoomIndex < in.getNoValidRooms(kc.exam) - 1){
		kc.newRoom = in.getValidRoom(kc.exam, newRoomIndex+1);
	} else if(toIndex < in.getNoValidPeriods(kc.exam) - 1){// Then to
		kc.to = in.getValidPeriod(kc.exam, toIndex+1);
		if (assignNewRoom) {
			kc.newRoom = in.getValidRoom(kc.exam, 0);
		}
	} else if(kc.exam < in.getNoExams() - 1){ // Then exam
		kc.exam++;
		kc.from = st.periodAssignment[kc.exam];
		kc.oldRoom = st.roomAssignment[kc.exam];	
		kc.to = in.getValidPeriod(kc.exam, 0);
		if (assignNewRoom) {
			kc.newRoom = in.getValidRoom(kc.exam, 0);
		}
	} else {
		// No next move exists
		return false;
	}

	CreateChain(st, kc);
	if(roomMode){
		BeamFit(st, kc);
	} else {
		TightestFit(st, kc);
	}

	return true;
}

bool ETT_KempeNeighborhoodExplorer::NextMove(const ETT_State& st, ETT_KempeExam& kc) const {
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	do {
		if(!AnyNextMove(st, kc)){
			return false;
		}
	} while (!FeasibleMove(st,kc));
	return true;

}

/************************************
*  ETT_Shake neighborhood explorer  *
************************************/
// initial swap builder
void ETT_ShakeNeighborhoodExplorer::RandomMove(const ETT_State& st, ETT_ShakeExam& sh) const {
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	// Bias-free version: each move has an equal prob. to be selected
	do {
		// Choose a random pair of period and determine all other data
		sh.period1 = EasyLocal::Core::Random::Uniform<int>(0, in.getNoPeriods()-1);	

		// Randomly pick a second period
		sh.period2 = EasyLocal::Core::Random::Uniform<int>(0, in.getNoPeriods()-2);
		if(sh.period2 >= sh.period1){
			sh.period2++;
		} else {
			std::swap(sh.period1,sh.period2);
		}
	} while(!FeasibleMove(st,sh));
}

// check move feasibility
bool ETT_ShakeNeighborhoodExplorer::FeasibleMove(const ETT_State& st, const ETT_ShakeExam& sh) const {
	// Symmetry
	assert(sh.period1 < sh.period2);

	// Check period length
	int d1 = in.getPeriodDuration(sh.period1), d2=in.getPeriodDuration(sh.period2);
	if(d1 < d2){
		// Check whether period 1 is long enough for all exams currently scheduled in period2
		for (int i = 0; i < in.getNoExams(); ++i) {
			if(st.periodAssignment[i] == sh.period2 && in.getExamDuration(i) > d1){
				return false;
			}
		}
	} else if (d2 < d1){
		// Check whether period 2 is long enough for all exams currently scheduled in period1
		for (int i = 0; i < in.getNoExams(); ++i) {
			if(st.periodAssignment[i] == sh.period1 && in.getExamDuration(i) > d2){
				return false;
			}
		}
	} 

	return true;
} 

// update the state according to the move 
void ETT_ShakeNeighborhoodExplorer::MakeMove(ETT_State& st, const ETT_ShakeExam& sh) const {
	// Insert the code that modifies the state st based on the application of move sh

	if(st.hardViolatedAndFeasOnly){
		return;
	}

	int i,j,s,k,a,dummy;
	for (i = 0; i < in.getNoExams(); ++i) {
		s = st.periodAssignment[i];

		// Update redundant conflict data
		dummy = st.getExamPeriodConflicts(i,sh.period1);
		st.setExamPeriodConflicts(i,sh.period1, st.getExamPeriodConflicts(i, sh.period2));
		st.setExamPeriodConflicts(i,sh.period2, dummy);

		// Update assignment
		if(s == sh.period1){
			st.periodAssignment[i] = sh.period2;
		} else if (s == sh.period2){
			st.periodAssignment[i] = sh.period1;
		}
	}


	// Update the redundant state matrix for rooms
	for(i=0; i < in.getNoRooms(); ++i){
		dummy = st.getRoomOccupation(i, sh.period1);
		st.setRoomOccupation(i, sh.period1, st.getRoomOccupation(i, sh.period2));
		st.setRoomOccupation(i, sh.period2, dummy);

		dummy = st.getRoomExams(i, sh.period1);
		st.setRoomExams(i, sh.period1, st.getRoomExams(i, sh.period2));
		st.setRoomExams(i, sh.period2, dummy);

		dummy = st.getRoomAlone(i, sh.period1);
		st.setRoomAlone(i, sh.period1, st.getRoomAlone(i, sh.period2));
		st.setRoomAlone(i, sh.period2, dummy);

		for(int k=0; k < in.getNoUniqueExamDurations(); ++k){
			dummy = st.getRoomDurations(i, sh.period1, k);
			st.setRoomDurations(i, sh.period1, k, st.getRoomDurations(i, sh.period2, k));
			st.setRoomDurations(i, sh.period2, k, dummy);
		}
	}
#ifdef PRINTOBJECTIVE
	std::cout << "Shake: " << sm.CostFunctionComponents(st) << std::endl;
#endif
}  

void ETT_ShakeNeighborhoodExplorer::FirstMove(const ETT_State& st, ETT_ShakeExam& sh) const {
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	// Need to consider the case where no feasible shake exists for period1=0
	sh.period2 = in.getNoPeriods();
	for(int i = 0; sh.period2 == in.getNoPeriods() && i < in.getNoPeriods(); ++i){
		sh.period1 = i;

		// period2 is the first following period for which the period length constraints are satisfied
		for(sh.period2=i+1; sh.period2 < in.getNoPeriods(); ++sh.period2){
			if(FeasibleMove(st,sh)){
				break;
			}
		}
	}
}

bool ETT_ShakeNeighborhoodExplorer::NextMove(const ETT_State& st, ETT_ShakeExam& sh) const {
	// First try to increase period2, only then period1
	// Return true if a next swap exists, false otherwise
	
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	if(nextPeriod2(st,sh)){ // Increase exam2
		return true;
	} else if(nextPeriod1(st,sh)){ // Increase exam1 and set first exam2
		return true;
	} else { // No next swap
		return false;
	}
}

bool ETT_ShakeNeighborhoodExplorer::nextPeriod2(const ETT_State& st, ETT_ShakeExam& sh) const{
	// Period2 is the first following exam for which the period length constraints are satisfied
	for(++sh.period2; sh.period2 < in.getNoPeriods(); ++sh.period2){
		if(FeasibleMove(st,sh)){
			return true;
		}
	}
	return false;
}

bool ETT_ShakeNeighborhoodExplorer::nextPeriod1(const ETT_State& st, ETT_ShakeExam& sh) const{
	// Exam1 is the first following exam for which a valid swap exists
	for(; ++sh.period1 < in.getNoPeriods()-1;){
		sh.period2 = sh.period1;
		if(nextPeriod2(st, sh)){
			return true;
		} 
	}
	return false;
}

/************************************
*  ETT_Component neighborhood explorer  *
************************************/
void ETT_ComponentNeighborhoodExplorer::TightestFit(const ETT_State& st, ETT_ComponentExam& c) const{
	c.newCapViol = 0;
	c.newMixedRoom = 0;
	c.newRoomAssignment.resize(in.getNoExams());

	c.examsInSlotsUsed.clear();
	for(auto &e : in.components[c.component]){
		c.examsInSlotsUsed[st.periodAssignment[e]].push_back(e);
	}	
	c.slotsMovedTo.clear();
	c.slotsMovedFrom.clear();

	std::vector<int> residualCap;
	residualCap.resize(in.getNoRooms());
	std::vector<std::vector<int>> roomDur; // [r][k] no. exams with duration index k in room r
	roomDur.resize(in.getNoRooms(), std::vector<int>(in.getNoUniqueExamDurations()));
	residualCap.reserve(in.getNoRooms());
	for(auto &p : c.slotsUsed){ // Reassign all exams from period p
		if((c.k < 0 && p > c.slot) || (c.k > 0 && p < c.slot)){
			// Exam will not be moved
			continue;
		} 

		int newPeriod = p + c.k;
		c.slotsMovedTo.insert(newPeriod);
		c.slotsMovedFrom.insert(p);

		// Step 1. Sort the exams in the component in decreasing size of students
		// This should already be done when computing the components of the graph
		assert(std::is_sorted(c.examsInSlotsUsed[p].begin(), c.examsInSlotsUsed[p].end(), [this](int l, int r) {return in.getNoExamStudents(l) > in.getNoExamStudents(r);}));

		// Step 2. Compute the residual capacities in the newPeriod
		// O(|R|)
		residualCap.clear();
		for (int r = 0; r < in.getNoRooms(); ++r) {
			residualCap.push_back(in.getRoomCapacity(r) - st.getRoomOccupation(r, newPeriod));

			for(int k=0; k < in.getNoUniqueExamDurations(); k++){
				roomDur[r][k] = st.getRoomDurations(r,newPeriod,k);
			}
		}


		if(c.slotsUsed.count(newPeriod) && 
				((newPeriod >= c.slot && c.k > 0) || (newPeriod <= c.slot && c.k < 0)) // The slot will be moved
		){
			// "Remove" all room assignments from exams currently assigned
			// O(|E|)
			for(auto &e : c.examsInSlotsUsed[newPeriod]){
				assert(st.periodAssignment[e] == newPeriod);
				residualCap[st.roomAssignment[e]] += in.getNoExamStudents(e);
				roomDur[st.roomAssignment[e]][in.getExamDurationIndex(e)]--;
			}
		}

		// Step 3. Assign to tightest room which has enough capacity, or to the largest room if no such room exists
		for(auto &e: c.examsInSlotsUsed[p]){
			int bestRoom = -1;

			int minViol = INT_MAX;
			for (int r = 0; r < in.getNoRooms(); ++r) {
				if(!in.isValidRoom(e, r)) continue;
				int dummy = residualCap[r] - in.getNoExamStudents(e);
				if(bestRoom == -1 || (dummy >= 0 && (minViol < 0 || dummy < minViol)) || (dummy < 0 && dummy > minViol)){
					bestRoom = r;
					minViol = dummy;
				}
			}
			
			assert(bestRoom != -1);
			residualCap[bestRoom] -= in.getNoExamStudents(e);
			c.newRoomAssignment[e] = bestRoom;
			roomDur[bestRoom][in.getExamDurationIndex(e)]++;
		}

		// Save some time in the delta's
		// This gives the capacity violations in all the movedToSlots
		bool dummy;
		for (int r = 0; r < in.getNoRooms(); ++r) {
			c.newCapViol += std::max(-residualCap[r], 0);

			dummy = false;
			for(int k=0; k < in.getNoUniqueExamDurations(); ++k){
				if(roomDur[r][k] > 0){
					c.newMixedRoom++;
					dummy = true;
				}
			}
			if(dummy){
				// One duration comes at no cost
				c.newMixedRoom--;
			}	
		}
	}

	// Compute the capacity gains in all the move from slots
	// such that c.newCapViol correctly gives the new cost in slots in from and to
	for(auto &s : c.slotsMovedFrom){
		// New cost already taken into account
		if(c.slotsMovedTo.count(s)) continue;
		residualCap.clear();
		for (int r = 0; r < in.getNoRooms(); ++r) {
			residualCap.push_back(in.getRoomCapacity(r) - st.getRoomOccupation(r, s));
		}
		for(auto& e : c.examsInSlotsUsed[s]){
			residualCap[st.roomAssignment[e]] += in.getNoExamStudents(e);
		}
		for (int r = 0; r < in.getNoRooms(); ++r) {
			c.newCapViol += std::max(-residualCap[r], 0);
		}
	}
	
	return;

}

// initial swap builder
void ETT_ComponentNeighborhoodExplorer::RandomMove(const ETT_State& st, ETT_ComponentExam& c) const {
	// Randomly select a component
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	int cntr=0;

	do {
		// Select a component with sufficient periods
		do {
			assert(in.getNoComponents()>0);
			c.component = EasyLocal::Core::Random::Uniform<int>(0, in.getNoComponents()-1);
			if(cntr++>MOVETHRESHOLD){
#ifndef	NDEBUG
				std::cout << "Empty component" << std::endl;
#endif
				throw EasyLocal::Core::EmptyNeighborhood();
			}
		} while (!computePeriods(st, c));

		// Randomly select k
		c.k = EasyLocal::Core::Random::Uniform<int>(-c.firstSlot, in.getNoPeriods() - c.lastSlot -2);
		if(c.k >= 0){
			c.k++;
		}

		// Randomly select the period
		c.slot = *std::next(c.slotsUsed.begin(), EasyLocal::Core::Random::Uniform<int>(0, c.slotsUsed.size()-1));	
	} while (!FeasibleMove(st,c));


	// Assign the rooms
	TightestFit(st, c);
}

// check move feasibility
bool ETT_ComponentNeighborhoodExplorer::FeasibleMove(const ETT_State& st, const ETT_ComponentExam& c) const {
	// Are all period length constraints satisfied?
	

	// Update the periods and rooms for all exams in the component
	int oldPeriod, newPeriod;
	for(auto &e : in.components[c.component]){
		// Compute new assignments
		oldPeriod = st.periodAssignment[e];

		if((c.k < 0 && oldPeriod > c.slot) || (c.k > 0 && oldPeriod < c.slot)){
			// Exam will not be moved
			continue;
		}

		newPeriod = oldPeriod + c.k;
		if(!in.isValidPeriod(e, newPeriod)){
			return false;
		}
	}

	return true;
} 

// update the state according to the move 
void ETT_ComponentNeighborhoodExplorer::MakeMove(ETT_State& st, const ETT_ComponentExam& c) const {
	// Insert the code that modifies the state st based on the application of move sh

	if(st.hardViolatedAndFeasOnly){
		return;
	}
	
	// Update the periods and rooms for all exams in the component
	int oldRoom, newRoom, oldPeriod, newPeriod;
	for(auto &e : in.components[c.component]){
		// Compute new assignments
		oldPeriod = st.periodAssignment[e];

		if((c.k < 0 && oldPeriod > c.slot) || (c.k > 0 && oldPeriod < c.slot)){
			// Exam will not be moved
			continue;
		}

		newPeriod = oldPeriod + c.k;
		oldRoom = st.roomAssignment[e];
		newRoom = c.newRoomAssignment[e];

		// Perform new assignments
		st.periodAssignment[e] = newPeriod;
		st.roomAssignment[e] = newRoom;
		assert(newPeriod >= 0 && newPeriod < in.getNoPeriods());
		assert(newRoom >= 0 && newRoom < in.getNoRooms());

		// Update state
		st.decRoomOccupation(oldRoom,oldPeriod,in.getNoExamStudents(e));
		st.incRoomOccupation(newRoom,newPeriod,in.getNoExamStudents(e));
		st.decRoomExams(oldRoom,oldPeriod);
		st.incRoomExams(newRoom,newPeriod);
		if(in.getExamAlone(e)){
			st.decRoomAlone(oldRoom, oldPeriod);
			st.incRoomAlone(newRoom, newPeriod);
		}
		st.decRoomDurations(oldRoom,oldPeriod,in.getExamDurationIndex(e));
		st.incRoomDurations(newRoom,newPeriod,in.getExamDurationIndex(e));

		// TODO If we move truly connected components only, this can be made faster?
		for(int a=0; a < in.noConflicts(e); ++a){
			auto e2 = in.getConflictList(e, a);
			st.decExamPeriodConflicts(e2, oldPeriod, in.getConflict(e,e2));
			st.incExamPeriodConflicts(e2, newPeriod, in.getConflict(e,e2));
		}
	}
#ifdef PRINTOBJECTIVE
	std::cout << "Component: " << sm.CostFunctionComponents(st) << std::endl;
#endif
}  

bool ETT_ComponentNeighborhoodExplorer::computePeriods(const ETT_State& st, ETT_ComponentExam& c) const{
	// Need to store all the slots used by the component
	c.slotsUsed.clear();
	for(auto &e : in.components[c.component]){
		c.slotsUsed.insert(st.periodAssignment[e]);
	}
	c.firstSlot = *std::min_element(c.slotsUsed.begin(), c.slotsUsed.end());
	c.lastSlot = *std::max_element(c.slotsUsed.begin(), c.slotsUsed.end());
	return (c.firstSlot > 0 || c.lastSlot!=in.getNoPeriods()-1);
}

void ETT_ComponentNeighborhoodExplorer::FirstMove(const ETT_State& st, ETT_ComponentExam& c) const {
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	// Start with component 0
	c.component = 0;
	for(; c.component < in.getNoComponents(); ++c.component){
		if(!computePeriods(st, c)){
			// Need to take into account the case where a component has exams in first and last time slot
			continue;
		}

		// Set k to the first slot (counting from zero makes that we
		// do not have to deduct 1)
		c.k = (-c.firstSlot == 0) ? 1 : -c.firstSlot ;

		// Set s to the first slot
		c.slot = c.firstSlot;

		break;
	}
	// We assume at least one component is feasible
	if(c.component == in.getNoComponents()){
		// No feasible move exists
#ifndef NDEBUG
			std::cout << "Empty component" << std::endl;
#endif
	       	throw EasyLocal::Core::EmptyNeighborhood(); 
	}

	// Assign the rooms
	TightestFit(st, c);
}

bool ETT_ComponentNeighborhoodExplorer::NextMove(const ETT_State& st, ETT_ComponentExam& c) const {
	// First try to increase k, then the slot, and finally the component
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	if(nextK(st, c)) { // Increase k
		return true;
	} else if (nextSlot(st, c)){ // Increase the slot
		return true;
	} else if (nextComponent(st, c)){ // Increase the component
		return true;
	} else {
		return false;
	}
}

bool ETT_ComponentNeighborhoodExplorer::nextK(const ETT_State& st, ETT_ComponentExam& c) const{
	if(c.k < in.getNoPeriods() - c.lastSlot -1){
		if(c.k != -1){
			c.k++;
			// Assign the rooms
			TightestFit(st, c);
			return true;
		} else if (in.getNoPeriods() - c.lastSlot > 1){
			// Skip zero: is 1 forward allowed?
			c.k += 2;
			// Assign the rooms
			TightestFit(st, c);
			return true;
		} else {
			// We cannot move forward
			return false;
		}
	} else {
		return false;
	}
}

bool ETT_ComponentNeighborhoodExplorer::nextSlot(const ETT_State& st, ETT_ComponentExam& c) const{
	auto index = c.slotsUsed.upper_bound(c.slot);
	if(index == c.slotsUsed.end()){
		return false;
	} else {
		c.slot = *index;
		c.k = (-c.firstSlot == 0) ? 1 : -c.firstSlot ;
		// Assign the rooms
		TightestFit(st, c);
		return true;
	}
}

bool ETT_ComponentNeighborhoodExplorer::nextComponent(const ETT_State& st, ETT_ComponentExam& c) const{
	c.component++;
	for(; c.component < in.getNoComponents(); ++c.component){
		if(!computePeriods(st, c)){
			// Need to take into account the case where a component has exams in first and last time slot
			continue;
		}

		// Set k to the first slot (counting from zero makes that we
		// do not have to deduct 1)
		c.k = (-c.firstSlot == 0) ? 1 : -c.firstSlot ;

		// Set s to the first slot
		c.slot = c.firstSlot;

		// Assign the rooms
		TightestFit(st, c);

		return true;
	}
	// No new component was found
	return false;
}


/************************************
*  ETT_Beam neighborhood explorer  *
************************************/
// initial move builder
#include <random>

// Definitions of node in ETT_Move.h

void ETT_BeamNeighborhoodExplorer::beamSearch(const ETT_State& st, ETT_BeamExam& b) const {

	//auto start = high_resolution_clock::now();

	// Step 1. Initialize starting redundant data 
	// (cheaper than copying an entire state at a node)
	node* root = new node;
	root->isOldAssignment = true;
	root->totCost = 0;
	root->capCost = 0, root->durCost = 0, root->exclCost = 0, root->roomCost = 0, root->totCost = 0;
	root->level = -1;
	int nodeId = 0;
	root->nodeId = nodeId;
	root->randCost = EasyLocal::Core::Random::Uniform<float>(0, 1);
	root->residualCap.resize(in.getNoRooms());
	root->roomExams.resize(in.getNoRooms());
	root->roomAlone.resize(in.getNoRooms());
	root->durations.resize(in.getNoRooms());

	int r,i;
	for (r = 0; r < in.getNoRooms(); ++r) {
		root->residualCap[r] = in.getRoomCapacity(r) - st.getRoomOccupation(r,b.period);	
		root->roomExams[r] = st.getRoomExams(r,b.period);	
		root->roomAlone[r] = st.getRoomAlone(r,b.period);
		root->durations[r].resize(in.getNoUniqueExamDurations());
		for (i = 0; i < in.getNoUniqueExamDurations(); ++i) {
			root->durations[r][i] = st.getRoomDurations(r,b.period,i);	
		}
	}
	for(auto& e : b.examIds){
		r = st.roomAssignment[e.first];
		root->residualCap[r] += e.second;
		root->roomExams[r]--;
		if(in.getExamAlone(e.first)) root->roomAlone[r]--;
		root->durations[r][in.getExamDurationIndex(e.first)]--;
	}

	// Room assgined in specific node
	root->assignment.resize(b.examIds.size(), -1);

	// Old room costs
	b.oldRoomOccupCost = 0; 					
	b.oldRoomExclCost = 0;
	b.oldMixedDurCost = 0;
	b.oldRoomPenalty = 0;


	// Step 2. Construct the tree
	std::vector<node*> current = {};
	current.reserve(beamwidth*in.getNoRooms());
	std::vector<node*> prev = {root};
	prev.reserve(beamwidth*in.getNoRooms());

	// One level for each exam
	for(int level=0; level < b.examIds.size(); ++level){
		current.clear();
		current.reserve(beamwidth*in.getNoRooms());
		int e = b.examIds[level].first;
		for(const auto& n : prev){
			// Try to assign exam to each possible room
			for(int r= 0; r < in.getNoRooms(); ++r){
				if(!in.isValidRoom(e, r)) continue;

				// Construct a new node object by copying data from the previous node
				// Wait, however, to copy expensive state vectors
				node* newNode = new node(n, r);
				newNode->level = level;
				newNode->nodeId = nodeId;
				newNode->randCost = EasyLocal::Core::Random::Uniform<float>(0, 1);

				// Calcule costs but wait to update states
				if(n->roomExams[r] > 0 && n->durations[r][in.getExamDurationIndex(e)] == 0){
					newNode->durCost += in.getNonMixedDurationsCost();
				}
				newNode->capCost += std::max(b.examIds[level].second - std::max(n->residualCap[r], 0), 0);
				newNode->exclCost += n->roomAlone[r];
				if(in.getExamAlone(e)){
					newNode->exclCost += n->roomExams[r];
				}
				newNode->roomCost += in.getRoomPenalty(r);	
				newNode->totCost = newNode->capCost + newNode->durCost + newNode->exclCost + newNode->roomCost;

				// Save the new node
				newNode->isOldAssignment = false;
				if(n->isOldAssignment && st.roomAssignment[e] == r){
					newNode->isOldAssignment = true;
					b.oldRoomOccupCost += (newNode->capCost - n->capCost); 					
					b.oldRoomExclCost += (newNode->exclCost - n->exclCost);
					b.oldMixedDurCost += (newNode->durCost - n->durCost);
					b.oldRoomPenalty += (newNode->roomCost - n->roomCost);

					// Make sure that the old node also survives
					// This allows to calculate the cost of the old assignment
					// In the last level, however, this option should not survive
					// Since otherwise, we have a null move...
					if(b.period == st.periodAssignment[e] ) { // Beam e.g. called from within kempe
						if (level < b.examIds.size() -1){
							newNode->totCost = -1;
						} else {
							// Null move
							newNode->totCost = INT_MAX;
						}
					}
				}

				// No node can be equal to a prev node (or something is wrong in nodeComp)
				//assert(!current.count(newNode));

				current.push_back(newNode);
				nodeId++;
			}
		}

		// Reduce the size of each level to the beamwidth!

		// We only need the first beamwidth elements to be sorted!
		// TODO TODO Improve sorting via a lambda function?
		// Sort on totCost + rand between 0 and 1?
		// vList.sort([](nod const * a, nod const * b) {return a->vCost < b->vCost;});
		std::partial_sort(current.begin(), current.begin() + std::min(beamwidth, (int) current.size()), current.end(), nodeComparison);

		std::vector<node*> temp(current.begin(), std::next(current.begin(), std::min(beamwidth, (int) current.size())));
		for(int dummy = std::min(beamwidth, (int) current.size()); dummy < current.size(); ++dummy){
			delete *std::next(current.begin(), dummy);
		}

		// Only now we do the state update: for much fewer nodes we have to make the expensive copy!
		for(auto& n : temp){
			// Deep copy from parent
			n->assignment = n->parent->assignment;
			n->durations = n->parent->durations;
			n->residualCap = n->parent->residualCap;
			n->roomExams = n->parent->roomExams;
			n->roomAlone = n->parent->roomAlone;
			
			// Update state
			r = n->lastRoom;
			n->assignment[level] = r;
			n->durations[r][in.getExamDurationIndex(e)]++;
			n->residualCap[r] -= b.examIds[level].second;
			n->roomExams[r]++;
			if(in.getExamAlone(e)){
				n->roomAlone[r]++;
			}
		}
		for(auto& n : prev){
			delete n;
		}
		prev = temp;
	}
	// Deep copy for memory clearing
	b.bestFound = **prev.begin();
	for(auto& n : prev){
		delete n;
	}

	//auto stop = high_resolution_clock::now();
	//auto durationOverall = duration_cast<microseconds>(stop - start);
}

// Vector of random examIds to be used whenever random exams need to be drawn without replacement
// Global var since I am not able to 
// change it in state, input,... 
// (constants and we require a shuffle)

ETT_BeamNeighborhoodExplorer::ETT_BeamNeighborhoodExplorer(const ETT_Input & pin, EasyLocal::Core::StateManager<ETT_Input,ETT_State>& psm, const int beamwidth, const int beamdepth) : NeighborhoodExplorer<ETT_Input,ETT_State,ETT_BeamExam>(pin, psm, "ETT_BeamExamNeighborhoodExplorer"), beamwidth(std::min(beamwidth, pin.getNoRooms())), beamDepth(std::min(beamdepth, in.getNoExams())) {

	// Initialize random exam ids
	randomExamIds.resize(in.getNoExams());
	std::iota(randomExamIds.begin(), randomExamIds.end(), 0);
} 

void ETT_BeamNeighborhoodExplorer::RandomMove(const ETT_State& st, ETT_BeamExam& b) const {
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	// TODO TODO
	// We disable the requirement to select exactly beamDepth exams: some period
	// may have less exams assigned thant the beamDepth
	// Re-enable if you want to check neighborhood enumerations
	// (next move requires examids.size() == beamDepth)
	int dummy = 0;
	do {
		// Choose a random period in which we optimize the room assignment
		b.period = EasyLocal::Core::Random::Uniform<int>(0, in.getNoPeriods()-1);

		// TODO TODO
		// Select at most beamdepth random exams scheduled in that period
		// This could be a bit more efficient if you would store a list of exams in each period...
		std::shuffle(const_cast<ETT_BeamNeighborhoodExplorer*>(this)->randomExamIds.begin(), const_cast<ETT_BeamNeighborhoodExplorer*>(this)->randomExamIds.end(), EasyLocal::Core::Random::GetGenerator());
		b.examIds.clear();
		for (auto& i : randomExamIds) {
			if(st.periodAssignment[i] == b.period){
				b.examIds.push_back({i, in.getNoExamStudents(i)});
				if (b.examIds.size() == beamDepth) {
					break;
				}
			}
		}
	} while (b.examIds.size() == 0 && dummy++ < MOVETHRESHOLD); 

	if(dummy >= MOVETHRESHOLD){
#ifndef NDEBUG
			std::cout << "Empty beam" << std::endl;
#endif
		throw EasyLocal::Core::EmptyNeighborhood(); 
	}
	
	// Assign largest exams first in the beam
	std::sort(b.examIds.begin(), b.examIds.end(), sortbysecdesc);	
	beamSearch(st, b);
}

// Check move feasibility
bool ETT_BeamNeighborhoodExplorer::FeasibleMove(const ETT_State& st, const ETT_BeamExam& b) const {
	// All moves are feasible
	assert(b.examIds.size() <= beamDepth); 
	return true;
} 

// update the state according to the move 
void ETT_BeamNeighborhoodExplorer::MakeMove(ETT_State& st, const ETT_BeamExam& b) const {

	if(st.hardViolatedAndFeasOnly){
		return;
	}

	// Insert the code that modifies the state st based on the application of move mv
	// Update 
	int i, oldRoom, newRoom, exam, size;
	for(i=0; i < b.examIds.size(); ++i){
		exam = b.examIds[i].first;
		oldRoom = st.roomAssignment[exam];
		newRoom = b.bestFound.assignment[i];
		size = b.examIds[i].second;

		if (oldRoom == newRoom) {
			continue;
		}

		// State
		st.roomAssignment[exam] = newRoom;

		//Redundant data
		st.decRoomOccupation(oldRoom, b.period, size); 
		st.incRoomOccupation(newRoom, b.period, size); 

		st.decRoomExams(oldRoom, b.period); 
		st.incRoomExams(newRoom, b.period);

		if (in.getExamAlone(exam)) {
			st.decRoomAlone(oldRoom, b.period);
			st.incRoomAlone(newRoom, b.period);
		}

		st.decRoomDurations(oldRoom, b.period, in.getExamDurationIndex(exam));
		st.incRoomDurations(newRoom, b.period, in.getExamDurationIndex(exam));
	}
#ifdef PRINTOBJECTIVE
	std::cout << "Beam: " << sm.CostFunctionComponents(st) << std::endl;
#endif
}  

bool ETT_BeamNeighborhoodExplorer::FirstBeamPeriod(const ETT_State& st, ETT_BeamExam& b, const int periodStart) const{
	int e;
	for (b.period = periodStart; b.period < in.getNoPeriods(); ++b.period) {
		b.examIds.clear();
		for (e = 0; e < in.getNoExams(); ++e) {
			if(st.periodAssignment[e] == b.period){
				b.examIds.push_back({e, in.getNoExamStudents(e)});
				if (b.examIds.size() >= beamDepth) {
					return true;	
				}
			} 
		}
	}
	return false;
}

void ETT_BeamNeighborhoodExplorer::FirstMove(const ETT_State& st, ETT_BeamExam& b) const {
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	// Only for debugging: can be inefficient
	
	if(FirstBeamPeriod(st, b, 0)){
	
		// Sort exams in decreasing order of students
		// Idea: execute beam search by assigning large exams first (bin packing)
		std::sort(b.examIds.begin(), b.examIds.end(), sortbysecdesc);	
		beamSearch(st, b);
	} else {
#ifndef NDEBUG
			std::cout << "Empty beam" << std::endl;
#endif
		throw EasyLocal::Core::EmptyNeighborhood(); 
	}
}

bool ETT_BeamNeighborhoodExplorer::NextMove(const ETT_State& st, ETT_BeamExam& b) const {
	const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = false;
	// Only debugging: can be ineficient
	// Need to enumerate exams in lexico ordering
	std::sort(b.examIds.begin(), b.examIds.end(), sortbyfirstasc);	
	
	// First try to increase the last exam in the list
	// Then the second to last,...
	// Finally increase the period
	while(b.period < in.getNoPeriods()){
		for(int positionOut = beamDepth-1; positionOut >= 0; --positionOut){
			for (int positionIn = positionOut; positionIn < beamDepth; ++positionIn) {
				int e;
				for (e = b.examIds[positionIn].first+1; e < in.getNoExams(); ++e) {
					if(st.periodAssignment[e] == b.period){
						b.examIds[positionIn] = std::make_pair(e, in.getNoExamStudents(e));
						if (positionIn == beamDepth - 1) {
							std::sort(b.examIds.begin(), b.examIds.end(), sortbysecdesc);	
							beamSearch(st, b);
							return true;
						} else {
							// Go to the next position
							b.examIds[positionIn+1] = {e, 0};
							break;
						}
					}
				}
				if (e == in.getNoExams()) {
					// We could not find a new exam
					for (int p = positionIn; p < beamDepth; ++p) {
						b.examIds[p] = {0, 0};
					}
					break;
				}
			}
		}
		FirstBeamPeriod(st, b, b.period + 1);
		b.examIds[beamDepth-1].first--; // Will be incremented again in for loop
	}



	return false;
}


