#include "ETT_CostComponent.h"
// Avoid to accept the moves: can have an impact on the threshold acceptacy criteria
// TODO TODO If feasibleOnly != 2 --> can have an influence on delta cost consistency in debugger
#define LARGEPENALTY 100000

int ETT_ExamConflicts::ComputeCost(const ETT_State& st) const {
	int cost = 0;
	int e1, e2, a;

	// For each pair of exams with a conflict check whether the assigned period is the same
	for(e1=0; e1 < in.getNoExams(); e1++){
		for(a=0; a < in.noConflicts(e1); a++){
			e2 = in.getConflictList(e1, a);
			if(e2 > e1 // Each pair
				&& st.periodAssignment[e1] == st.periodAssignment[e2]){
				cost += in.getConflict(e1,e2);
			}
		}
	}

	return cost;

}

void ETT_ExamConflicts::PrintViolations(const ETT_State& st, std::ostream& os) const {
	int e1, e2, a;

	// For each pair of exams with a conflict check whether the assigned period is the same
	for(e1=0; e1 < in.getNoExams(); e1++){
		for(a=0; a < in.noConflicts(e1); a++){
			e2 = in.getConflictList(e1, a);
			if(e2 > e1 // Each pair
				&& st.periodAssignment[e1] == st.periodAssignment[e2]){
				os << "Conflict between exam " << e1 << " and " << e2 << " (period " << st.periodAssignment[e1] << ")" << std::endl;
			}
		}
	}
}

int ETT_MoveDeltaCostExamConflicts::ComputeDeltaCost(const ETT_State& st, const ETT_MoveExam& mv) const {
	//int cost = 0;
	//if(mv.to == mv.from) return cost;
	
	//// O(noExams)
	//int e, a;
	//for(a=0; a < in.noConflicts(mv.exam); a++){
	//	e = in.getConflictList(mv.exam, a);
	//	if(st.periodAssignment[e] == mv.from){
	//		cost -= in.getConflict(mv.exam, e);
	//	} else if(st.periodAssignment[e] == mv.to){
	//		cost += in.getConflict(mv.exam, e);
	//	}
	//}

	//return cost;
	
	//assert(st.getExamPeriodConflicts(mv.exam, mv.to) == 0);


	//return (st.getExamPeriodConflicts(mv.exam, mv.to)-st.getExamPeriodConflicts(mv.exam, mv.from));
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}		
	// Initially, all hard constrains are satisfied
	if(feasibleOnly && mv.deltaConflicts > 0){
		// If a hard constraint is violated, save so in the state
		// and return a very large cost so that the move is never
		// accepted
		// For all subsequent cost components: skip the evaluation
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return mv.deltaConflicts;
	}
}

int ETT_SwapDeltaCostExamConflicts::ComputeDeltaCost(const ETT_State& st, const ETT_SwapExam& swp) const {
//	int cost = 0;
//	if(swp.period1 == swp.period2) return cost;
//
//	// O(noExams)
//	// Cost exam1
//	int e, a;
//	int cdiff;
//	for(a=0; a < in.noConflicts(swp.exam1, swp.exam2); a++){
//		e = in.getConflictList(swp.exam1, swp.exam2, a);
//		cdiff = in.getConflict(e, swp.exam1) - in.getConflict(e, swp.exam2);
//		if(cdiff == 0 || e==swp.exam1 || e==swp.exam2) continue;
//		if(st.periodAssignment[e] == swp.period1){
//			cost -= cdiff;
//		} else if(st.periodAssignment[e] == swp.period2){
//			cost += cdiff;
//		}	
//	}
//	

	//if(swp.period1 == swp.period2) return 0;
	//assert(st.getExamPeriodConflicts(swp.exam1, swp.period2) <= in.getConflict(swp.exam1,swp.exam2));
	//assert(st.getExamPeriodConflicts(swp.exam2, swp.period1) <= in.getConflict(swp.exam1,swp.exam2));
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	if(feasibleOnly && swp.deltaConflicts > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return swp.deltaConflicts;
	}
}

int ETT_KickDeltaCostExamConflicts::ComputeDeltaCost(const ETT_State& st, const ETT_KickExam& kick) const {
	//int cost = 0;
	//int a;
	//int e;

	//if(kick.period1 != kick.period2){
	//	// Conflicts e1: p1->p2
	//	for(a=0; a < in.noConflicts(kick.exam1); a++){
	//		e = in.getConflictList(kick.exam1, a);
	//		if(st.periodAssignment[e] == kick.period1){
	//			cost -= in.getConflict(e, kick.exam1);
	//		} else if(st.periodAssignment[e] == kick.period2  && (e != kick.exam2 || kick.period2 == kick.newPeriod)){
	//			cost += in.getConflict(e, kick.exam1);
	//		}
	//	}
	//}

	//if(kick.period2 != kick.newPeriod){
	//	// Conflicts e2: p2->p_new
	//	for(a=0; a < in.noConflicts(kick.exam2); a++){
	//		e = in.getConflictList(kick.exam2, a);
	//		if(st.periodAssignment[e] == kick.period2){
	//			cost -= in.getConflict(e, kick.exam2);
	//		} else if(st.periodAssignment[e] == kick.newPeriod && (e != kick.exam1 || kick.period1 == kick.period2)){
	//			cost += in.getConflict(e, kick.exam2);
	//		}
	//	}
	//}
	//

	//return cost;
	
	//int deltaConflicts = 0;
	//if(kick.period1 != kick.period2){
	//	deltaConflicts += st.getExamPeriodConflicts(kick.exam1, kick.period2)
	//			- st.getExamPeriodConflicts(kick.exam1, kick.period1);

	//}
	//if(kick.period2 != kick.newPeriod){
	//	deltaConflicts += st.getExamPeriodConflicts(kick.exam2, kick.newPeriod)
	//			- in.getConflict(kick.exam1,kick.exam2)
	//			- st.getExamPeriodConflicts(kick.exam2, kick.period2);
	//} 
	//if(kick.newPeriod == kick.period1){
	//	deltaConflicts -= in.getConflict(kick.exam1,kick.exam2);
	//}
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	if(feasibleOnly && kick.deltaConflicts > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return kick.deltaConflicts;
	}
}

int ETT_KempeDeltaCostExamConflicts::ComputeDeltaCost(const ETT_State& st, const ETT_KempeExam& kc) const {
	int deltaConflicts = 0;
	int period1 = kc.from;
	int period2 = kc.to;
	auto other = &kc.s2;
	for(auto& chain : {&kc.s1, &kc.s2}){
		for(auto& e1: *chain){
			// Option 1: we kind of assume that chain is shorter than conflict list

			// New conflicts - old conflicts
			deltaConflicts += st.getExamPeriodConflicts(e1, period2);
			deltaConflicts -= st.getExamPeriodConflicts(e1, period1);

			// Correct for the conflicts which are solved by the Kempe
			if(period1 == kc.from){
				for(auto& e2 : *other){
					deltaConflicts -= 2*in.getConflict(e1,e2);
				}
			}

			// Correct for conflicts within chain that move along
			// (Can only happen when the timetable is not yet conflictfree before the Kempe)
			for(auto& e2 : *chain){
				if(e1 < e2){
					deltaConflicts += 2*in.getConflict(e1,e2);
				}
			}
			


			// Option 2: conflict list is shorter than chain
			//for(int a=0; a < in.noConflicts(e1); ++a){
			//	auto e2 = in.getConflictList(e1, a);
			//	if(st.periodAssignment[e2] == period1 && !chain->count(e2)){
			//		deltaConflicts -= in.getConflict(e1, e2);
			//	}
			//	// This can only happen in case we did not move a conflicting exam in period kc.from to kc.to
			//	// because kc.to is not a valid period for the conflicting exam
			//	if(st.periodAssignment[e2] == period2 && !other->count(e2)){
			//		deltaConflicts += in.getConflict(e1, e2);
			//	}
			//}
		}
		period1 = kc.to;
		period2 = kc.from;
		other = &kc.s1;
	}

	if(st.hardViolatedAndFeasOnly){
		return 0;
	}

	if(feasibleOnly && deltaConflicts > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return deltaConflicts;
	}
}

int ETT_RoomOccupancy::ComputeCost(const ETT_State& st) const {
	int cost = 0;
	int r,p;

	// For each room and each period, compute capacity violation
	for(r=0; r < in.getNoRooms(); r++){
		for(p=0; p < in.getNoPeriods(); p++){
			if(st.getRoomOccupation(r,p) > in.getRoomCapacity(r)){
				cost += st.getRoomOccupation(r,p) - in.getRoomCapacity(r);
			}
		}
	}
	return cost;
}

void ETT_RoomOccupancy::PrintViolations(const ETT_State& st, std::ostream& os) const {
	int r,p;

	// For each room and each period, compute capacity violation
	for(r=0; r < in.getNoRooms(); r++){
		for(p=0; p < in.getNoPeriods(); p++){
			if(st.getRoomOccupation(r,p) > in.getRoomCapacity(r)){
				os << st.getRoomOccupation(r,p) << " students in room " << r << " (capacity: " << in.getRoomCapacity(r) 
					<< ") in period " << p << std::endl;
			}
		}
	}
}

int ETT_MoveDeltaCostRoomOccupancy::ComputeDeltaCost(const ETT_State& st, const ETT_MoveExam& mv) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}

	// O(1)
	int cost = 0;
	int size = in.getNoExamStudents(mv.exam);

	// Room 1
	int cap = in.getRoomCapacity(mv.oldRoom);
	int occup = st.getRoomOccupation(mv.oldRoom, mv.from);
	cost += std::max(occup-size-cap, 0) - std::max(occup-cap, 0);

	// Room 2
	cap = in.getRoomCapacity(mv.newRoom);
	occup = st.getRoomOccupation(mv.newRoom, mv.to);
	cost += std::max(occup+size-cap, 0) - std::max(occup-cap, 0);

	if(feasibleOnly && cost > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return cost;
	}
}

int ETT_SwapDeltaCostRoomOccupancy::ComputeDeltaCost(const ETT_State& st, const ETT_SwapExam& swp) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}

	int cost = 0;
	int size1 = in.getNoExamStudents(swp.exam1);
	int size2 = in.getNoExamStudents(swp.exam2);
	int studentDiff = size1 - size2;

	if(studentDiff != 0){
		// Room 1
		int cap = in.getRoomCapacity(swp.room1);
		int occup = st.getRoomOccupation(swp.room1, swp.period1);
		cost += std::max(occup-studentDiff-cap, 0) - std::max(occup-cap, 0);

		// Room 2
		cap = in.getRoomCapacity(swp.room2);
		occup = st.getRoomOccupation(swp.room2, swp.period2);
		cost += std::max(occup+studentDiff-cap, 0) - std::max(occup-cap, 0);
	}

	if(feasibleOnly && cost > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return cost;
	}
}

int ETT_KickDeltaCostRoomOccupancy::ComputeDeltaCost(const ETT_State& st, const ETT_KickExam& kick) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}

	int cost = 0;
	int size1 = in.getNoExamStudents(kick.exam1);
	int size2 = in.getNoExamStudents(kick.exam2);
	int studentDiff = size1 - size2;

	if(kick.swap){
		// Note: small overhead if studentDiff=0, but perhaps smaller than everytime checking the condition
		// Room 1
		int cap = in.getRoomCapacity(kick.room1);
		int occup = st.getRoomOccupation(kick.room1, kick.period1);
		cost += std::max(occup-studentDiff-cap, 0) - std::max(occup-cap, 0);

		// Room 2
		cap = in.getRoomCapacity(kick.room2);
		occup = st.getRoomOccupation(kick.room2, kick.period2);
		cost += std::max(occup+studentDiff-cap, 0) - std::max(occup-cap, 0);
	} else {
		// Room1
		int cap = in.getRoomCapacity(kick.room1);
		int occup = st.getRoomOccupation(kick.room1, kick.period1);
		cost += std::max(occup-size1-cap, 0) - std::max(occup-cap, 0);

		// Room2
		cap = in.getRoomCapacity(kick.room2);
		occup = st.getRoomOccupation(kick.room2, kick.period2);
		cost += std::max(occup+studentDiff-cap, 0) - std::max(occup-cap, 0);

		// NewRoom
		cap = in.getRoomCapacity(kick.newRoom);
		occup = st.getRoomOccupation(kick.newRoom, kick.newPeriod);
		cost += std::max(occup+size2-cap, 0) - std::max(occup-cap, 0);
	}

	if(feasibleOnly && cost > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return cost;
	}
}

int ETT_KempeDeltaCostRoomOccupancy::ComputeDeltaCost(const ETT_State& st, const ETT_KempeExam& kc) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}

	// Cost of the old room assignment
	int cost = 0;
	for (int r = 0; r < in.getNoRooms(); ++r) {
		int cap = in.getRoomCapacity(r);
		cost -= std::max(st.getRoomOccupation(r, kc.from)-cap, 0);
		cost -= std::max(st.getRoomOccupation(r, kc.to)-cap, 0);
	}

	// Cost of the new room assignment
	// This was already computed to determine the room assignment
	cost += kc.newCapViol;

	if(feasibleOnly && cost > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return cost;
	}
}

int ETT_ComponentDeltaCostRoomOccupancy::ComputeDeltaCost(const ETT_State& st, const ETT_ComponentExam& c) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}

	// cost = new costs in movedFrom and movedTo	
	int cost = c.newCapViol;

	// Deduct old costs in these slots
	for(int r=0; r < in.getNoRooms(); r++){
		for(int p=0; p < in.getNoPeriods(); p++){
			if(!c.slotsMovedFrom.count(p) && !c.slotsMovedTo.count(p)) continue;
			if(st.getRoomOccupation(r,p) > in.getRoomCapacity(r)){
				cost -= st.getRoomOccupation(r,p) - in.getRoomCapacity(r);
			}
		}
	}

	if(feasibleOnly && cost > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return cost;
	}
}

int ETT_BeamDeltaCostRoomOccupancy::ComputeDeltaCost(const ETT_State& st, const ETT_BeamExam& b) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}

	//// Cost of the old room assignment
	//int cost = 0;
	//for (int r = 0; r < in.getNoRooms(); ++r) {
	//	cost -= std::max(st.getRoomOccupation(r, b.period)-in.getRoomCapacity(r), 0);
	//}

	//// Cost of the new room assignment
	//// This was already computed to determine the room assignment
	//cost += b.bestFound.capCost;

	//return cost;
	int cost = b.bestFound.capCost - b.oldRoomOccupCost;

	if(feasibleOnly && cost > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return cost;
	}
}

int ETT_ExamSequence::ComputeCost(const ETT_State& st) const {
	int cost = 0;

	int e1, e2, a;
	for(e1=0; e1 < in.getNoExams(); e1++){
		// After
		for(a=0; a < in.getNoAfter(e1); ++a){
			e2 = in.getAfter(e1, a);
			if(st.periodAssignment[e1] <= st.periodAssignment[e2]){
				cost++;
			}
		}

		// Exclusion
		for(a=0; a < in.getNoExcl(e1); ++a){
			e2 = in.getExcl(e1, a);
			if(e1 < e2 && st.periodAssignment[e1] == st.periodAssignment[e2]){
				cost++;
			}
		}

		// Coincidence
		for(a=0; a < in.getNoCoin(e1); ++a){
			e2 = in.getCoin(e1, a);
			if(e1 < e2 && st.periodAssignment[e1] != st.periodAssignment[e2]){
				cost++;
			}
		}
	}

	return cost;
}

void ETT_ExamSequence::PrintViolations(const ETT_State& st, std::ostream& os) const {
	int e1, e2, a;
	for(e1=0; e1 < in.getNoExams(); e1++){
		// After
		for(a=0; a < in.getNoAfter(e1); ++a){
			e2 = in.getAfter(e1, a);
			if(st.periodAssignment[e1] <= st.periodAssignment[e2]){
				os << "After " << e1 << " and " <<  e2 << " (" << 
					st.periodAssignment[e1] << " - " << st.periodAssignment[e2] << ")" << std::endl;
			}
		}

		// Exclusion
		for(a=0; a < in.getNoExcl(e1); ++a){
			e2 = in.getExcl(e1, a);
			if(e1 < e2 && st.periodAssignment[e1] == st.periodAssignment[e2]){
				os << "Exclusion " << e1 << " and " <<  e2 << " (" << 
					st.periodAssignment[e1] << " - " << st.periodAssignment[e2] << ")" << std::endl;
			}
		}

		// Coincidence
		for(a=0; a < in.getNoCoin(e1); ++a){
			e2 = in.getCoin(e1, a);
			if(e1 < e2 && st.periodAssignment[e1] != st.periodAssignment[e2]){
				os << "Coincidence " << e1 << " and " <<  e2 << " (" << 
					st.periodAssignment[e1] << " - " << st.periodAssignment[e2] << ")" << std::endl;
			}
		}
	}
}

int ETT_MoveDeltaCostExamSequence::ComputeDeltaCost(const ETT_State& st, const ETT_MoveExam& mv) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	
	int cost = 0;

	if(mv.from == mv.to) return cost;

	// O(E)
	int e1 = mv.exam, e2, a, p;
	// e1 must come AFTER e2
	for(a=0; a < in.getNoAfter(e1); ++a){
		e2 = in.getAfter(e1, a);
		p = st.periodAssignment[e2];
		if(mv.from <= p && mv.to > p){ // No longer violated
			cost--;
		} else if(p < mv.from && mv.to <= p){ // Becomes violated
			cost++;
		}
	}

	// e1 must come BEFORE e2
	for(a=0; a < in.getNoBefore(e1); ++a){
		e2 = in.getBefore(e1, a);
		p = st.periodAssignment[e2];
		if(p <= mv.from && mv.to < p){ // No longer violated
			cost--;
		} else if(mv.from < p && p <= mv.to){ // Becomes violated
			cost++;
		}
	}

	// Exclusion: e1 and e2 not at the same time
	for(a=0; a < in.getNoExcl(e1); ++a){
		e2 = in.getExcl(e1, a);
		p = st.periodAssignment[e2];
		if(p == mv.from){ // No longer violated
			cost--;
		} else if(p == mv.to){ // Becomes violated
			cost++;
		}
	}

	// Coincidence: e1 and e2 at the same time
	for(a=0; a < in.getNoCoin(e1); ++a){
		e2 = in.getCoin(e1, a);
		p = st.periodAssignment[e2];
		if(p == mv.to){ // No longer violated
			cost--;
		} else if(p==mv.from){ // Becomes violated
			cost++;
		}
	}

	if(feasibleOnly && cost > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return cost;
	}

}

int ETT_SwapDeltaCostExamSequence::ComputeDeltaCost(const ETT_State& st, const ETT_SwapExam& swp) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}

	int cost = 0;

	if(swp.period1 == swp.period2) return cost;

	// O(E)
	int e, a, p;
	// exam1 must come AFTER e
	for(a=0; a < in.getNoAfter(swp.exam1); ++a){
		e = in.getAfter(swp.exam1, a);
		if(e==swp.exam2){ // Take into account relation between e1 and e2
			if(swp.period1 < swp.period2){ // No longer violated
				cost--;
			} else { // Becomes violated
				cost++;
			}
		} else {
			p = st.periodAssignment[e];
			if(swp.period1 <= p && swp.period2 > p){ // No longer violated
				cost--;
			} else if(p < swp.period1 && swp.period2 <= p){ // Becomes violated
				cost++;
			}
		}
	}
	// exam2 must come AFTER e
	for(a=0; a < in.getNoAfter(swp.exam2); ++a){
		e = in.getAfter(swp.exam2, a);
		if(e==swp.exam1) continue; // Do not double count relation between exam1 and exam2
		p = st.periodAssignment[e];
		if(swp.period2 <= p && swp.period1 > p){ // No longer violated
			cost--;
		} else if(p < swp.period2 && swp.period1 <= p){ // Becomes violated
			cost++;
		}
	}

	// exam1 must come BEFORE e
	for(a=0; a < in.getNoBefore(swp.exam1); ++a){
		e = in.getBefore(swp.exam1, a);
		if(e==swp.exam2){ // Take into account relation between e1 and e2
			if(swp.period1 > swp.period2){ // No longer violated
				cost--;
			} else { // Becomes violated
				cost++;
			}
		} else {
			p = st.periodAssignment[e];
			if(p <= swp.period1 && swp.period2 < p){ // No longer violated
				cost--;
			} else if(swp.period1 < p && p <= swp.period2){ // Becomes violated
				cost++;
			}
		}
	}

	// exam2 must come BEFORE e
	for(a=0; a < in.getNoBefore(swp.exam2); ++a){
		e = in.getBefore(swp.exam2, a);
		if(e==swp.exam1) continue;
		p = st.periodAssignment[e];
		if(p <= swp.period2 && swp.period1 < p){ // No longer violated
			cost--;
		} else if(swp.period2 < p && p <= swp.period1){ // Becomes violated
			cost++;
		}
	}

	// Exclusion: exam1 and e not at the same time
	for(a=0; a < in.getNoExcl(swp.exam1); ++a){
		e = in.getExcl(swp.exam1, a);
		if(e==swp.exam2) continue;
		p = st.periodAssignment[e];
		if(p == swp.period1){ // No longer violated
			cost--;
		} else if(p == swp.period2){ // Becomes violated
			cost++;
		}
	}
	// Exclusion: exam2 and e not at the same time
	for(a=0; a < in.getNoExcl(swp.exam2); ++a){
		e = in.getExcl(swp.exam2, a);
		if(e==swp.exam1) continue;
		p = st.periodAssignment[e];
		if(p == swp.period2){ // No longer violated
			cost--;
		} else if(p == swp.period1){ // Becomes violated
			cost++;
		}
	}

	// Coincidence: exam1 and e at the same time
	for(a=0; a < in.getNoCoin(swp.exam1); ++a){
		e = in.getCoin(swp.exam1, a);
		if(e==swp.exam2) continue;
		p = st.periodAssignment[e];
		if(p == swp.period2){ // No longer violated
			cost--;
		} else if(p==swp.period1){ // Becomes violated
			cost++;
		}
	}
	// Coincidence: exam2 and e at the same time
	for(a=0; a < in.getNoCoin(swp.exam2); ++a){
		e = in.getCoin(swp.exam2, a);
		if(e==swp.exam1) continue;
		p = st.periodAssignment[e];
		if(p == swp.period1){ // No longer violated
			cost--;
		} else if(p==swp.period2){ // Becomes violated
			cost++;
		}
	}

	if(feasibleOnly && cost > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return cost;
	}

}

int ETT_KickDeltaCostExamSequence::ComputeDeltaCost(const ETT_State& st, const ETT_KickExam& kick) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}

	int cost = 0;
	int e, a, p;

	// O(E)
	if(kick.period1 != kick.period2){
		// exam1 must come AFTER e
		for(a=0; a < in.getNoAfter(kick.exam1); ++a){
			e = in.getAfter(kick.exam1, a);
			if(e==kick.exam2){ // Take into account relation between e1 and e2
				if(kick.period1 <= kick.period2){ // Was violated
					cost--;
				} 
				if (kick.period2 <= kick.newPeriod) { // Is violated
					cost++;
				}
			} else {
				p = st.periodAssignment[e];
				if(kick.period1 <= p){ // Was violated
					cost--;
				} 
				if(kick.period2 <= p){ // Is violated
					cost++;
				}
			}
		}

		// exam1 must come BEFORE e
		for(a=0; a < in.getNoBefore(kick.exam1); ++a){
			e = in.getBefore(kick.exam1, a);
			if(e==kick.exam2){ // Take into account relation between e1 and e2
				if(kick.period1 >= kick.period2){ // Was violated
					cost--;
				}
				if(kick.period2 >= kick.newPeriod){ // Is violated
					cost++;
				}
			} else {
				p = st.periodAssignment[e];
				if(p <= kick.period1){ // Was violated
					cost--;
				} 
				if(p <= kick.period2){ // Is violated
					cost++;
				}
			}
		}

		// Exclusion: exam1 and e not at the same time
		for(a=0; a < in.getNoExcl(kick.exam1); ++a){
			e = in.getExcl(kick.exam1, a);
			if(e==kick.exam2){
				if(kick.period1 == kick.period2){ // Was violated
					cost--;
				} 
				if(kick.period2 == kick.newPeriod){ // Is violated
					cost++;
				}
			} else {
				p = st.periodAssignment[e];
				if(p == kick.period1){ // Was violated
					cost--;
				} 
				if(p == kick.period2){ // Is violated
					cost++;
				}
			}
		}

		// Coincidence: exam1 and e at the same time
		for(a=0; a < in.getNoCoin(kick.exam1); ++a){
			e = in.getCoin(kick.exam1, a);
			if(e==kick.exam2){
				if(kick.period1 != kick.period2){ // Was violated
					cost--;
				} 
			        if(kick.period2 != kick.newPeriod){ // Is violated
					cost++;
				}
			} else {
				p = st.periodAssignment[e];
				if(p != kick.period1){ // Was violated
					cost--;
				}
				if(p != kick.period2){ // Is violated
					cost++;
				}
			}
		}
	}

	if(kick.period2 != kick.newPeriod){
		// exam2 must come AFTER e
		for(a=0; a < in.getNoAfter(kick.exam2); ++a){
			e = in.getAfter(kick.exam2, a);
			if(e==kick.exam1) {
				if(kick.period1 == kick.period2 && kick.newPeriod > kick.period2){ // Not yet considered! 
					cost--;
				}
			} else {
				p = st.periodAssignment[e];
				if(kick.period2 <= p){ // Was violated
					cost--;
				}
				if(kick.newPeriod <= p){ // Is violated
					cost++;
				} 
			}
		}

		// exam2 must come BEFORE e
		for(a=0; a < in.getNoBefore(kick.exam2); ++a){
			e = in.getBefore(kick.exam2, a);
			if(e==kick.exam1){
				if(kick.period1 == kick.period2 && kick.newPeriod < kick.period2){ // Not yet considered! 
					cost--;
				}
			} else {
				p = st.periodAssignment[e];
				if(p <= kick.period2){ // Was violated
					cost--;
				} 
				if(p <= kick.newPeriod){ // Is violated
					cost++;
				}
			}
		}

		// Exclusion: exam2 and e not at the same time
		for(a=0; a < in.getNoExcl(kick.exam2); ++a){
			e = in.getExcl(kick.exam2, a);
			if(e==kick.exam1){
				if(kick.period1 == kick.period2 && kick.newPeriod != kick.period2){ // Not yet considered! 
					cost--;
				}
			} else {
				p = st.periodAssignment[e];
				if(p == kick.period2){ // Was violated
					cost--;
				} 
				if(p == kick.newPeriod){ // Is violated
					cost++;
				}
			}
		}


		// Coincidence: exam2 and e at the same time
		for(a=0; a < in.getNoCoin(kick.exam2); ++a){
			e = in.getCoin(kick.exam2, a);
			if(e==kick.exam1){
				if(kick.period1 == kick.period2 && kick.newPeriod != kick.period2){ // Not yet considered! 
					cost++;
				}

			} else {
				p = st.periodAssignment[e];
				if(p != kick.period2){ // Was violated
					cost--;
				}
				if(p != kick.newPeriod){ // Is violated
					cost++;
				}
			}
		}
	}

	if(feasibleOnly && cost > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return cost;
	}
}

int ETT_KempeDeltaCostExamSequence::ComputeDeltaCost(const ETT_State& st, const ETT_KempeExam& kc) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}

	int cost = 0;

	int e2, a, dummy, period1=kc.from, period2=kc.to;
        auto otherChain = &kc.s2;
	for(auto& thisChain : {&kc.s1, &kc.s2}){
		for(auto& e1: *thisChain){
			// After
			for(a=0; a < in.getNoAfter(e1); ++a){
				e2 = in.getAfter(e1, a);
				if(thisChain->count(e2)) continue;
				if(otherChain->count(e2)){
					assert(period1 != period2);
					period1 < period2 ? cost-- : cost++;
				} else {
					dummy = st.periodAssignment[e2];
					if(period1 <= dummy && dummy < period2) cost--;
					if(period2 <= dummy && dummy < period1) cost++;
				}
			}

			// Before
			for(a=0; a < in.getNoBefore(e1); ++a){
				e2 = in.getBefore(e1, a);
				// Also avoid double counting
				if(thisChain->count(e2) || otherChain->count(e2)) continue;
				dummy = st.periodAssignment[e2];
				if(period2 < dummy && dummy <= period1) cost--;
				if(period1 < dummy && dummy <= period2) cost++;
			}

			// Exclusion
			for(a=0; a < in.getNoExcl(e1); ++a){
				e2 = in.getExcl(e1, a);
				if(thisChain->count(e2) || otherChain->count(e2)) continue;
				dummy = st.periodAssignment[e2];
				if(period1 == dummy) cost--;
				if(period2 == dummy) cost++;
			}

			// Coincidence
			for(a=0; a < in.getNoCoin(e1); ++a){
				e2 = in.getCoin(e1, a);
				if(thisChain->count(e2) || otherChain->count(e2)) continue;

				dummy = st.periodAssignment[e2];
				if(period2 == dummy) cost--;
				if(period1 == dummy) cost++;
			}
		}
		otherChain = thisChain;
		period1 = kc.to;
		period2 = kc.from;
	}

	if(feasibleOnly && cost > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return cost;
	}
}

int ETT_ComponentDeltaCostExamSequence::ComputeDeltaCost(const ETT_State& st, const ETT_ComponentExam& c) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}

	int cost = 0;

	// Check sequence relations with exams not in the component
	int p1,p2,p3,a, e2;
	for(auto& e1 : in.components[c.component]){	
		p1 = st.periodAssignment[e1];
		if((c.k < 0 && p1 > c.slot) || (c.k > 0 && p1 < c.slot)){
			// Exam will not be moved
			continue;
		}

		p3 = p1 + c.k;

		//// Coincidence
		for(a=0; a < in.getNoCoin(e1); ++a){
			e2 = in.getCoin(e1, a);
			p2 = st.periodAssignment[e2];

			// Exam is in the component and will thus be moved along
			if(in.componentsSet[c.component].count(e2)) continue;

			if(p1 == p2) cost++; 
			assert(p1!=p3);
			if(p2 == p3) cost--; 
		}

		// Exclusion
		for(a=0; a < in.getNoExcl(e1); ++a){
			e2 = in.getExcl(e1, a);
			if(in.componentsSet[c.component].count(e2)) continue;
			p2 = st.periodAssignment[e2];
			if(p1 == p2) cost--;
			assert(p1!=p3);
			if(p3==p2) cost++; 
		}

		// After
		for(a=0; a < in.getNoAfter(e1); ++a){
			e2 = in.getAfter(e1, a);
			if(in.componentsSet[c.component].count(e2)) continue;
			p2 = st.periodAssignment[e2];
			if(p3 <= p2 && p2 < p1) cost++;
			if(p1 <= p2 && p2 < p3) cost--;
		}
		
		// Before
		for(a=0; a < in.getNoBefore(e1); ++a){
			e2 = in.getBefore(e1, a);
			if(in.componentsSet[c.component].count(e2)) continue;
			p2 = st.periodAssignment[e2];
			if(p1 < p2 && p2 <= p3) cost++;
			if(p3 < p2 && p2 <= p1) cost--;
		}
	}


	if(feasibleOnly && cost > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return cost;
	}
}

int ETT_ShakeDeltaCostExamSequence::ComputeDeltaCost(const ETT_State& st, const ETT_ShakeExam& sh) const {
	// Initially, all hard constrains are satisfied
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	int cost = 0;

	// No need to check coincidence constraints
	int i, e, s, a;
	assert(sh.period1 < sh.period2);
	for(i=0; i < in.getNoExams(); ++i){
		if(st.periodAssignment[i] == sh.period1){
			// Will be moved later in time
			// -> no new after violations
			// -> Solved after violations?
			for(a=0; a < in.getNoAfter(i); ++a){
				e = in.getAfter(i, a);
				s = st.periodAssignment[e];
				if(s > sh.period1 && s <= sh.period2){
					cost--;
				}
			}

			// New before violations?
			for(a=0; a < in.getNoBefore(i); ++a){
				e = in.getBefore(i, a);
				s = st.periodAssignment[e];
				if(s > sh.period1 && s <= sh.period2){
					cost++;
				}
			}
		} else if (st.periodAssignment[i] == sh.period2){
			// Will be moved earlier in time
			// -> no new before violations
			// -> Solved before violations?
			for(a=0; a < in.getNoBefore(i); ++a){
				e = in.getBefore(i, a);
				s = st.periodAssignment[e];
				// Avoid double counting: no >= this time
				if(s > sh.period1 && s < sh.period2){
					cost--;
				}
			}

			// New after violations?
			for(a=0; a < in.getNoAfter(i); ++a){
				e = in.getAfter(i, a);
				s = st.periodAssignment[e];
				// Avoid double counting: no >= this time
				if(s > sh.period1 && s < sh.period2){
					cost++;
				}
			}
		}
	}

	if(feasibleOnly && cost > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return cost;
	}
}


// In the python validator, the penalty calculation does not depend on the number of other exams e is scheduled together
// I.e. cost += 1 if st.getRoomExams(r, p) > 1
// Here: incur a penalty for every other exam e is scheduled together with: partial improvements in feasibility of solution
int ETT_RoomExclusive::ComputeCost(const ETT_State& st) const {
	int cost = 0;
	
	int e,p,r;
	for(e=0; e < in.getNoExams(); ++e){
		if(in.getExamAlone(e)){
			r = st.roomAssignment[e];
			p = st.periodAssignment[e];
			// TODO TODO TODO
			//if(st.getRoomExams(r,p) > 1){
			//	cost++;
			//}
			cost += st.getRoomExams(r,p) - 1;
		}
	}

	return cost;
}

void ETT_RoomExclusive::PrintViolations(const ETT_State& st, std::ostream& os) const {
	int e,p,r;
	for(e=0; e < in.getNoExams(); ++e){
		if(in.getExamAlone(e)){
			r = st.roomAssignment[e];
			p = st.periodAssignment[e];
			if(st.getRoomExams(r,p) - 1 > 0){
				os << "Room exclusive: " << e << " (" << st.getRoomExams(r, p) << ")" << std::endl;
			}
		}
	}
}

int ETT_MoveDeltaCostRoomExclusive::ComputeDeltaCost(const ETT_State& st, const ETT_MoveExam& mv) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}

	int cost = 0;

	// O(1)
	if(in.getExamAlone(mv.exam)){ // The exam itself should be alone
		cost -= st.getRoomAlone(mv.oldRoom, mv.from)-1 		// Cost other alone exams
			+ st.getRoomExams(mv.oldRoom, mv.from)-1; 	// Cost exam being moved
		cost += st.getRoomAlone(mv.newRoom, mv.to) 		// Recall that exam has not yet been moved
			+ st.getRoomExams(mv.newRoom, mv.to); 	
	} else {
		cost -= st.getRoomAlone(mv.oldRoom, mv.from) - st.getRoomAlone(mv.newRoom, mv.to);
	}
	
	

	if(feasibleOnly && cost > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return cost;
	}
}

int ETT_SwapDeltaCostRoomExclusive::ComputeDeltaCost(const ETT_State& st, const ETT_SwapExam& swp) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}

	int cost = 0;
	if(in.getExamAlone(swp.exam1) + in.getExamAlone(swp.exam2) != 1){ // None or both of the exams should be alone
		return cost;
	}


	// O(1)
	if(in.getExamAlone(swp.exam1)){ // Exam1 should be alone, exam 2 not
		// Cost of other alone exams can be ignored: exam1 moves out, but exam2 moves in
		cost -= st.getRoomExams(swp.room1, swp.period1)-1;     
		cost += st.getRoomExams(swp.room2, swp.period2)-1;
	} else { // Exam 2 should be alone, exam 1 not
		cost -= st.getRoomExams(swp.room2, swp.period2)-1;     
		cost += st.getRoomExams(swp.room1, swp.period1)-1;
	}
	
	if(feasibleOnly && cost > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return cost;
	}
}

int ETT_KickDeltaCostRoomExclusive::ComputeDeltaCost(const ETT_State& st, const ETT_KickExam& kick) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}

	int cost = 0;


	// O(1)
	if(kick.swap){
		if(in.getExamAlone(kick.exam1) + in.getExamAlone(kick.exam2) != 1){ // None or both of the exams should be alone
			return cost;
		}		
		if(in.getExamAlone(kick.exam1)){ // Exam1 should be alone, exam 2 not
			// Cost of other alone exams can be ignored: exam1 moves out, but exam2 moves in
			cost -= st.getRoomExams(kick.room1, kick.period1)-1;     
			cost += st.getRoomExams(kick.room2, kick.period2)-1;
		} else { // Exam 2 should be alone, exam 1 not
			cost -= st.getRoomExams(kick.room2, kick.period2)-1;     
			cost += st.getRoomExams(kick.room1, kick.period1)-1;
		}
	} else {
		if(in.getExamAlone(kick.exam1)){
			// Period 1
			cost -= st.getRoomExams(kick.room1, kick.period1)-1 	// Cost exam1
				+ st.getRoomAlone(kick.room1, kick.period1)-1; 	// Cost other alone exams
			// Period 2
			cost += st.getRoomExams(kick.room2, kick.period2)-1; 	// Cost exam1
										// No change in cost other exams: one moves in, other moves out
										// Cost others can be ignored (not a simple move: newPeriod != period2 || newRoom != room2)
		} else {
			// Period 1
			cost -= st.getRoomAlone(kick.room1, kick.period1); 	
			// Period 2: no changes as one exam moves in, other moves out
		}

		if(in.getExamAlone(kick.exam2)){
			// Period 2
			cost -= st.getRoomExams(kick.room2, kick.period2)-1; 	// Cost exam2
										// No change in cost other exams: one moves in, other moves out
			// newPeriod (Not a swap: newPeriod != period2 || newRoom != room2
			cost += st.getRoomExams(kick.newRoom, kick.newPeriod) 	// Cost exam1
				+ st.getRoomAlone(kick.newRoom, kick.newPeriod);
		} else {
			// Period 2: no changes as one exam moves in, other moves out
			// newPeriod
			cost += st.getRoomAlone(kick.newRoom, kick.newPeriod); 	
		}
	}


	if(feasibleOnly && cost > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return cost;
	}
}

int ETT_KempeDeltaCostRoomExclusive::ComputeDeltaCost(const ETT_State& st, const ETT_KempeExam& kc) const {
	// TODO TODO Can this made faster in case of kempe beam?
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}

	int cost = 0;

	std::array<std::vector<int>,2> noExams;
	noExams[0].reserve(in.getNoRooms());
	noExams[1].reserve(in.getNoRooms());
	std::array<std::vector<int>,2> noAlone;
	noAlone[0].reserve(in.getNoRooms());
	noAlone[1].reserve(in.getNoRooms());

	for (int r = 0; r < in.getNoRooms(); ++r) {
		noExams[0][r] = st.getRoomExams(r,kc.from);
		noExams[1][r] = st.getRoomExams(r,kc.to);
		noAlone[0][r] = st.getRoomAlone(r, kc.from);
		noAlone[1][r] = st.getRoomAlone(r, kc.to);
	}
	for(auto& e : kc.s1){
		noExams[0][st.roomAssignment[e]]--;
		noExams[1][kc.newRoomAssignment[e]]++;
		if(in.getExamAlone(e)){
			noAlone[0][st.roomAssignment[e]]--;
			noAlone[1][kc.newRoomAssignment[e]]++;
		}
	}
	for(auto& e : kc.s2){
		noExams[1][st.roomAssignment[e]]--;
		noExams[0][kc.newRoomAssignment[e]]++;
		if(in.getExamAlone(e)){
			noAlone[1][st.roomAssignment[e]]--;
			noAlone[0][kc.newRoomAssignment[e]]++;
		}
	}

	for (int r = 0; r < in.getNoRooms(); ++r) {
		cost -= (st.getRoomExams(r,kc.from)-1)*st.getRoomAlone(r,kc.from); // If first term is -1, then second must be 0
		cost -= (st.getRoomExams(r,kc.to)-1)*st.getRoomAlone(r,kc.to);
		cost += (noExams[0][r]-1)*noAlone[0][r];	
		cost += (noExams[1][r]-1)*noAlone[1][r];	
	}

	if(feasibleOnly && cost > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return cost;
	}
}

int ETT_ComponentDeltaCostRoomExclusive::ComputeDeltaCost(const ETT_State& st, const ETT_ComponentExam& c) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}

	int cost = 0;

	// Impact of moving exam away
	int r;
	for(auto& s : c.slotsMovedFrom){
		for(auto& e : c.examsInSlotsUsed.at(s)){
			r = st.roomAssignment[e];

			// Cost from other exams in that room
			cost -= (st.getRoomAlone(r, s) - in.getExamAlone(e));

			if(in.getExamAlone(e)){
				// Cost exam itself
				cost -= st.getRoomExams(r,s) - 1;
				for(auto& e2 : c.examsInSlotsUsed.at(s)){
					if(in.getExamAlone(e2) && st.roomAssignment[e2] == r && e!=e2){
						cost++;
					}
				}
			}
		}
	}

	// Impact of moving exam to
	for(auto& s : c.slotsMovedTo){
		for(auto& e : c.examsInSlotsUsed.at(s-c.k)){
			r = c.newRoomAssignment[e];

			// Cost from other exams in that room that should be alone
			cost += st.getRoomAlone(r, s);
			if(in.getExamAlone(e)){
				// Cost for exams that stay there
				cost += st.getRoomExams(r,s);
			}

			// Correct for exams that move away
			if(c.examsInSlotsUsed.count(s)){
				for(auto& e2 : c.examsInSlotsUsed.at(s)){
					if(st.roomAssignment[e2] == r){
						// We need to be alone, but the exam goes away
						if(in.getExamAlone(e)) cost--;

						// An exam that needed to be allone goes away
					       	if(in.getExamAlone(e2)) cost--;
					}
				}
			}

			// Correct for exams that move along
			if(in.getExamAlone(e)){
				for(auto& e2 : c.examsInSlotsUsed.at(s-c.k)){
					if(c.newRoomAssignment[e2] == r && e2 != e){
						cost++;
					}
				}
			}
		}
	}

	if(feasibleOnly && cost > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return cost;
	}
}


int ETT_BeamDeltaCostRoomExclusive::ComputeDeltaCost(const ETT_State& st, const ETT_BeamExam& b) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}

	//int cost = 0;
	//for (int r = 0; r < in.getNoRooms(); ++r) {
	//	cost -= (st.getRoomExams(r,b.period)-1)*st.getRoomAlone(r, b.period);
	//}
	//cost += b.bestFound.exclCost;
	//return cost;
	int cost =  b.bestFound.exclCost - b.oldRoomExclCost;
	if(feasibleOnly && cost > 0){
		const_cast<ETT_State&>(st).hardViolatedAndFeasOnly = true;
		return LARGEPENALTY;
	} else {
		return cost;
	}
}

// TODO TODO TODO
// THIS IS FOR THE CARTER INSTANCES
#ifdef UNCAPACITATED
int ETT_ExamDistance::ComputeCost(const ETT_State& st) const {
	int cost = 0;

	int periodCost[5] = {16,8,4,2,1};
	int e1, e2, a, p1, p2;
	for(e1=0; e1 < in.getNoExams(); ++e1){
		p1 = st.periodAssignment[e1];
		for(a=0; a < in.noConflicts(e1); ++a){
			e2 = in.getConflictList(e1, a);
			p2 = st.periodAssignment[e2];

			if(p1 < p2) { // No double counting
				if(p2-p1 <= in.getPeriodSpread()){
					cost += periodCost[p2-p1-1]*in.getConflict(e1, e2); // periodSpread, which has cost 1 by default
				}
			}
		}
	}	

	return cost;
}
#else
int ETT_ExamDistance::ComputeCost(const ETT_State& st) const {
	int cost = 0;

	int e1, e2, a, p1, p2;
	for(e1=0; e1 < in.getNoExams(); ++e1){
		p1 = st.periodAssignment[e1];
		for(a=0; a < in.noConflicts(e1); ++a){
			e2 = in.getConflictList(e1, a);
			p2 = st.periodAssignment[e2];

			if(p1 < p2) { // No double counting
				if(p2-p1 <= in.getPeriodSpread()){
					cost += in.getConflict(e1, e2); // periodSpread, which has cost 1 by default
				}
				if(in.getPeriodDay(p1) == in.getPeriodDay(p2)){
					if(p2==p1+1){
						// twoInARow
						cost += in.getTwoInARowCost()*in.getConflict(e1,e2);
					} else {
						// twoInADay
						cost += in.getTwoInADayCost()*in.getConflict(e1,e2);
					}
				} 
			}
		}
	}	

	return cost;
}
#endif

void ETT_ExamDistance::PrintViolations(const ETT_State& st, std::ostream& os) const {
	int e1, e2, a, p1, p2;
	for(e1=0; e1 < in.getNoExams(); ++e1){
		p1 = st.periodAssignment[e1];
		for(a=0; a < in.noConflicts(e1); ++a){
			e2 = in.getConflictList(e1, a);
			p2 = st.periodAssignment[e2];

			if(p1 < p2){ // No double counting
				if(p2-p1 <= in.getPeriodSpread()){
					os << "Period spread: " << e1 << " (" << p1 << ") and " << e2 << "(" << p2 << ")" << std::endl;
				}

				if(in.getPeriodDay(p1) == in.getPeriodDay(p2)){
					if(p2==p1+1){
						// twoInARow
						os << "Two in a row: " << e1 << " (" << p1 << ") and " << e2 << "(" << p2 << ")" << std::endl;
					} else {
						// twoInADay
						os << "Two in a day: " << e1 << " (" << p1 << ") and " << e2 << "(" << p2 << ")" << std::endl;
					}
				} 
			}
		}
	}
}

int deltaCostMove(const ETT_State& st, const ETT_Input& in, const int exam, const int from, const int to) {
	int cost = 0;
	if(from == to) return cost;


	int diff,p1,p2;
        int max = std::max(in.getPeriodSpread(), in.getMaxPeriodsDay());

        for(diff=1; diff <= max; ++diff){
		// Old cost
		p1=from-diff;
		p2=from+diff;

		if(p1>=0){
			if(diff <= in.getPeriodSpread()){
				cost -= st.getExamPeriodConflicts(exam, p1);
			}
			if(in.getPeriodDay(from) == in.getPeriodDay(p1)){
				if(diff==1){
					// twoInARow
					cost -= in.getTwoInARowCost()*st.getExamPeriodConflicts(exam, p1);
				} else {
					// twoInADay
					cost -= in.getTwoInADayCost()*st.getExamPeriodConflicts(exam, p1);
				}
			}
		}

		if(p2 < in.getNoPeriods()){
			if(diff <= in.getPeriodSpread()){
				cost -= st.getExamPeriodConflicts(exam, p2);
			}
			if(in.getPeriodDay(from) == in.getPeriodDay(p2)){
				if(diff==1){
					// twoInARow
					cost -= in.getTwoInARowCost()*st.getExamPeriodConflicts(exam, p2);
				} else {
					// twoInADay
					cost -= in.getTwoInADayCost()*st.getExamPeriodConflicts(exam, p2);
				}
			}
		}

		// New cost
		p1=to-diff;
		p2=to+diff;

		if(p1>=0){
			if(diff <= in.getPeriodSpread()){
				cost += st.getExamPeriodConflicts(exam, p1);
			}
			if(in.getPeriodDay(to) == in.getPeriodDay(p1)){
				if(diff==1){
					// twoInARow
					cost += in.getTwoInARowCost()*st.getExamPeriodConflicts(exam, p1);
				} else {
					// twoInADay
					cost += in.getTwoInADayCost()*st.getExamPeriodConflicts(exam, p1);
				}
			}
		}

		if(p2 < in.getNoPeriods()){
			if(diff <= in.getPeriodSpread()){
				cost += st.getExamPeriodConflicts(exam, p2);
			}
			if(in.getPeriodDay(to) == in.getPeriodDay(p2)){
				if(diff==1){
					// twoInARow
					cost += in.getTwoInARowCost()*st.getExamPeriodConflicts(exam, p2);
				} else {
					// twoInADay
					cost += in.getTwoInADayCost()*st.getExamPeriodConflicts(exam, p2);
				}
			}
		}
	}

	// Implementation looping over all arcs
	// --> SLOWER
	
	//// O(E)
	//int e2, a;
	//for(a=0; a < in.noConflicts(exam); ++a){
	//	e2 = in.getConflictList(exam, a);
	//	p2 = st.periodAssignment[e2];

	//	// Cost from
	//	if(p2 != from) { // No conflict, but also not distinct enough
	//		diff = from < p2 ? p2 - from : from - p2; // Absolute diff
	//		if(diff <= in.getPeriodSpread()){
	//			cost -= in.getConflict(exam, e2); // periodSpread
	//		}

	//		if(in.getPeriodDay(from) == in.getPeriodDay(p2)){
	//			if(p2==from+1 || p2 == from-1){
	//				// twoInARow
	//				cost -= in.getTwoInARowCost()*in.getConflict(exam,e2);
	//			} else {
	//				// twoInADay
	//				cost -= in.getTwoInADayCost()*in.getConflict(exam,e2);
	//			}
	//		} 
	//	}

	//	// Cost to
	//	if(p2 != to) { // No conflict, but also not distinct enough 
	//		diff = to < p2 ? p2 - to : to - p2;
	//		if(diff <= in.getPeriodSpread()){
	//			cost += in.getConflict(exam, e2); // periodSpread
	//		}
	//							  //
	//		if(in.getPeriodDay(to) == in.getPeriodDay(p2)){
	//			if(p2==to+1 || p2 == to-1){
	//				// twoInARow
	//				cost += in.getTwoInARowCost()*in.getConflict(exam,e2);
	//			} else {
	//				// twoInADay
	//				cost += in.getTwoInADayCost()*in.getConflict(exam,e2);
	//			}
	//		} 
	//	}
	//}
	return cost;
}

int ETT_MoveDeltaCostExamDistance::ComputeDeltaCost(const ETT_State& st, const ETT_MoveExam& mv) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	return deltaCostMove(st, in, mv.exam, mv.from, mv.to);
}

int deltaCostSwap(const ETT_State& st, const ETT_Input& in, const int exam1, const int exam2, const int period1, const int period2) {
	int cost = 0;
	if(period1 == period2) return cost;

	cost = deltaCostMove(st, in, exam1, period1, period2) + deltaCostMove(st, in, exam2, period2, period1);
	// Correction for exams themselves
	int diff = std::abs(period2-period1);
	if(diff <= in.getPeriodSpread()){
		cost += 2*in.getConflict(exam1, exam2);
	}

	if(in.getPeriodDay(period1) == in.getPeriodDay(period2)){
		if(diff==1){
			// twoInARow
			cost += 2*in.getTwoInARowCost()*in.getConflict(exam1, exam2);
		} else {
			// twoInADay
			cost += 2*in.getTwoInADayCost()*in.getConflict(exam1, exam2);
		}
	} 

	//// O(E)
	//int a,e, p, tdiff;
        //int cdiff;
	//for(a=0; a < in.noConflicts(exam1, exam2); ++a){
	//	e = in.getConflictList(exam1, exam2, a);
	//	cdiff = in.getConflict(e, exam1) - in.getConflict(e, exam2);
	//	if(cdiff == 0 || e == exam1 || e == exam2) continue; // None of the two exams has a conflict
	//							     
	//	p = st.periodAssignment[e];
	//	// Cost from
	//	if(p != period1) { // No conflict, but also not distinct enough
	//		tdiff = period1 < p ? p - period1 : period1 - p; // Absolute diff.
	//		if(tdiff <= in.getPeriodSpread()){
	//			cost -= cdiff;
	//		}
	//		if(in.getPeriodDay(period1) == in.getPeriodDay(p)){
	//			if(p==period1+1 || p == period1-1){
	//				// twoInARow
	//				cost -= in.getTwoInARowCost()*cdiff;
	//			} else {
	//				// twoInADay
	//				cost -= in.getTwoInADayCost()*cdiff;
	//			}
	//		} 
	//	}

	//	// Cost to
	//	if(p != period2) {
	//		tdiff = period2 < p ? p - period2 : period2 - p;
	//		if(tdiff <= in.getPeriodSpread()){
	//			cost += cdiff;
	//		}
	//		if(in.getPeriodDay(period2) == in.getPeriodDay(p)){ // No conflict, but also not distinct enough
	//			if(p==period2+1 || p == period2-1){
	//				// twoInARow
	//				cost += in.getTwoInARowCost()*cdiff;
	//			} else {
	//				// twoInADay
	//				cost += in.getTwoInADayCost()*cdiff;
	//			}
	//		} 
	//	}
	//}

	return cost;
}

int ETT_SwapDeltaCostExamDistance::ComputeDeltaCost(const ETT_State& st, const ETT_SwapExam& swp) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	return deltaCostSwap(st, in, swp.exam1, swp.exam2, swp.period1, swp.period2);
}

int ETT_KickDeltaCostExamDistance::ComputeDeltaCost(const ETT_State& st, const ETT_KickExam& kick) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}

	if (kick.period1 == kick.newPeriod) {
		// Costs correspond to a swap of exam1 and exam2
		// Note: e1 > e2 is possible (<-> swap: e1 < e2)
		return deltaCostSwap(st, in, kick.exam1, kick.exam2, kick.period1, kick.period2);
	} else if (kick.period1 == kick.period2) {
		// Costs correspond to a move of exam2: period2 -> newPeriod
		return deltaCostMove(st, in, kick.exam2, kick.period2, kick.newPeriod);
	} else if (kick.period2 == kick.newPeriod){
		// Costs correspond to a move of exam1: period1 -> period2
		return deltaCostMove(st, in, kick.exam1, kick.period1, kick.period2);
	} else {
		assert(kick.period1 != kick.period2 && kick.period2 != kick.newPeriod && kick.period1 != kick.newPeriod);
		

		int cost = 0;

		cost = deltaCostMove(st, in, kick.exam1, kick.period1, kick.period2) + deltaCostMove(st, in, kick.exam2, kick.period2, kick.newPeriod);

		// Correction for exams themselves
		int diff = std::abs(kick.period2-kick.period1);
		assert(diff > 0);
		if(diff <= in.getPeriodSpread()){
			cost += in.getConflict(kick.exam1, kick.exam2);
		}

		if(in.getPeriodDay(kick.period1) == in.getPeriodDay(kick.period2)){
			if(diff==1){
				// twoInARow
				cost += in.getTwoInARowCost()*in.getConflict(kick.exam1, kick.exam2);
			} else {
				// twoInADay
				cost += in.getTwoInADayCost()*in.getConflict(kick.exam1, kick.exam2);
			}
		} 

		// If there is a new cost, include it
		diff = std::abs(kick.period2-kick.newPeriod);
		assert(diff > 0);
		if(diff <= in.getPeriodSpread()){
			cost += in.getConflict(kick.exam1, kick.exam2);
		}

		if(in.getPeriodDay(kick.period2) == in.getPeriodDay(kick.newPeriod)){
			if(diff==1){
				// twoInARow
				cost += in.getTwoInARowCost()*in.getConflict(kick.exam1, kick.exam2);
			} else {
				// twoInDay
				cost += in.getTwoInADayCost()*in.getConflict(kick.exam1, kick.exam2);
			}
		} 

		diff = std::abs(kick.period1-kick.newPeriod);
		assert(diff > 0);
		if(diff <= in.getPeriodSpread()){
			cost -= in.getConflict(kick.exam1, kick.exam2);
		}

		if(in.getPeriodDay(kick.period1) == in.getPeriodDay(kick.newPeriod)){
			if(diff==1){
				// twoInARow
				cost -= in.getTwoInARowCost()*in.getConflict(kick.exam1, kick.exam2);
			} else {
				// twoInDay
				cost -= in.getTwoInADayCost()*in.getConflict(kick.exam1, kick.exam2);
			}
		} 
		
		return cost;

		// Too slow
		//// O(E)
		//int cost = 0;
		//int e, a, p, tdiff, cdiff;

		//// Cost period1
		//for(a=0; a < in.noConflicts(kick.exam1); ++a){
		//	e = in.getConflictList(kick.exam1, a);
		//	p = st.periodAssignment[e];

		//	tdiff = kick.period1 < p ? p - kick.period1 : kick.period1 - p; // Absolute diff
		//	if(p != kick.period1) { // No conflict, but also not distinct enough
		//		if(tdiff <= in.getPeriodSpread()){
		//			cost -= in.getConflict(kick.exam1, e); // periodSpread
		//		}
		//		if(in.getPeriodDay(kick.period1) == in.getPeriodDay(p)){
		//			if(p==kick.period1+1 || p == kick.period1-1){
		//				// twoInARow
		//				cost -= in.getTwoInARowCost()*in.getConflict(kick.exam1, e);
		//			} else {
		//				// twoInADay
		//				cost -= in.getTwoInADayCost()*in.getConflict(kick.exam1, e);
		//			}
		//		} 
		//	}
		//}

		//// Cost period2
		//for(a=0; a < in.noConflicts(kick.exam1, kick.exam2); ++a){
		//	e = in.getConflictList(kick.exam1, kick.exam2, a);
		//	p = st.periodAssignment[e];
		//	cdiff = in.getConflict(e, kick.exam1) - in.getConflict(e, kick.exam2);
		//	if(cdiff == 0 || e == kick.exam1 || e == kick.exam2) continue; // None of the two exams has a conflict

		//	tdiff = kick.period2 < p ? p - kick.period2 : kick.period2 - p; // Absolute diff
		//	if(p != kick.period2) {
		//		if(tdiff <= in.getPeriodSpread()){
		//			cost += cdiff;
		//		}
		//		if(in.getPeriodDay(kick.period2) == in.getPeriodDay(p)){ // No conflict, but also not distinct enough
		//			if(p==kick.period2+1 || p == kick.period2-1){
		//				// twoInARow
		//				cost += in.getTwoInARowCost()*cdiff;
		//			} else {
		//				// twoInADay
		//				cost += in.getTwoInADayCost()*cdiff;
		//			}
		//		} 
		//	}
		//}

		//// Cost newPeriod
		//for(a=0; a < in.noConflicts(kick.exam2); ++a){
		//	e = in.getConflictList(kick.exam2, a);
		//	p = (e == kick.exam1 ? kick.period2 : st.periodAssignment[e]);

		//	tdiff = kick.newPeriod < p ? p - kick.newPeriod : kick.newPeriod - p; // Absolute diff
		//	if(p != kick.newPeriod) { // No conflict, but also not distinct enough
		//		if(tdiff <= in.getPeriodSpread()){
		//			cost += in.getConflict(kick.exam2, e); // periodSpread
		//		}
		//		if(in.getPeriodDay(kick.newPeriod) == in.getPeriodDay(p)){
		//			if(p==kick.newPeriod+1 || p == kick.newPeriod-1){
		//				// twoInARow
		//				cost += in.getTwoInARowCost()*in.getConflict(kick.exam2, e);
		//			} else {
		//				// twoInADay
		//				cost += in.getTwoInADayCost()*in.getConflict(kick.exam2, e);
		//			}
		//		} 
		//	}
		//}	
		//if(temp != cost){
		//	std::cout << "Temp vs cost: " << temp << " - " << cost << std::endl;
		//	std::cout << kick.period1 << " - " << kick.period2 << " - " << kick.newPeriod << std::endl;
		//	std::cout << "Con: " << in.getConflict(kick.exam1, kick.exam2) << std::endl;
		//	std::abort();
		//}

		//return cost;
	}
}

int ETT_KempeDeltaCostExamDistance::ComputeDeltaCost(const ETT_State& st, const ETT_KempeExam& kc) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	int cost = 0;

	int period1=kc.from, period2=kc.to;
	for(auto& chain : {&kc.s1, &kc.s2}){
		for(auto& e1: *chain){
			cost += deltaCostMove(st, in, e1, period1, period2);
		}
		period1 = kc.to;
		period2 = kc.from;
	}

	// Correction for exams themselves
	// Compute weight of conflicts between the chains	
	int diff = std::abs(period2-period1);
	if(diff <= in.getPeriodSpread() || (in.getPeriodDay(period1) == in.getPeriodDay(period2))){
		int con_p1_p2 = 0;
		int con_p1_p1 = 0;
		int con_p2_p2 = 0;

		// Conflicts between chain 1 and 2
		for(auto& e1 : kc.s1){
			for(auto& e2 : kc.s2){
				con_p1_p2 += in.getConflict(e1, e2);
			}
		}
		
		// Conflicts within chain 1
		for(auto& e1 : kc.s1){
			for(auto& e2 : kc.s1){
				if(e1 == e2) continue;
				con_p1_p1 += in.getConflict(e1, e2);
			}
		}

		// Conflicts within chain 2
		for(auto& e1 : kc.s2){
			for(auto& e2 : kc.s2){
				if(e1 == e2) continue;
				con_p2_p2 += in.getConflict(e1, e2);
			}
		}

		if(diff <= in.getPeriodSpread()){
			cost += 2*con_p1_p2;
			cost -= con_p1_p1;
			cost -= con_p2_p2;
		}
		if(in.getPeriodDay(period1) == in.getPeriodDay(period2)){
			if(diff==1){
				// twoInARow
				cost += 2*con_p1_p2*in.getTwoInARowCost();
				cost -= con_p1_p1*in.getTwoInARowCost();
				cost -= con_p2_p2*in.getTwoInARowCost();
			} else {
				// twoInADay
				cost += 2*con_p1_p2*in.getTwoInADayCost();
				cost -= con_p1_p1*in.getTwoInADayCost();
				cost -= con_p2_p2*in.getTwoInADayCost();
			}
		} 
	}

	//int temp = cost;
	//cost = 0;




	//int e2, a, p2, oldDist, newDist;
	//period1=kc.from;
       	//period2=kc.to;
	//for(auto& chain : {&kc.s1, &kc.s2}){
	//	for(auto& e1: *chain){
	//		for(a=0; a < in.noConflicts(e1); ++a){
	//			e2 = in.getConflictList(e1, a);
	//			if(!kc.s1.count(e2) && !kc.s2.count(e2)){
	//				p2 = st.periodAssignment[e2];
	//				oldDist = std::abs(p2-period1);	
	//				if(oldDist <= in.getPeriodSpread() && oldDist > 0){
	//					cost -= in.getConflict(e1, e2); // periodSpread, which has cost 1 by default
	//				}			
	//				newDist = std::abs(p2-period2);	
	//				if(newDist <= in.getPeriodSpread() && newDist > 0){
	//					cost += in.getConflict(e1, e2); // periodSpread, which has cost 1 by default
	//				}

	//				if(oldDist > 0 && in.getPeriodDay(period1) == in.getPeriodDay(p2)){
	//					if(oldDist == 1){
	//						// twoInARow
	//						cost -= in.getTwoInARowCost()*in.getConflict(e1,e2);
	//					} else {
	//						// twoInADay
	//						cost -= in.getTwoInADayCost()*in.getConflict(e1,e2);
	//					}
	//				} 
	//				if(newDist > 0 && in.getPeriodDay(period2) == in.getPeriodDay(p2)){
	//					if(newDist == 1){
	//						// twoInARow
	//						cost += in.getTwoInARowCost()*in.getConflict(e1,e2);
	//					} else {
	//						// twoInADay
	//						cost += in.getTwoInADayCost()*in.getConflict(e1,e2);
	//					}
	//				} 
	//			}
	//		}
	//	}
	//	period1 = kc.to;
	//	period2 = kc.from;
	//}

	//if(temp != cost){
	//	std::cout << "Temp vs cost: " << temp << " - " << cost << std::endl;
	//	std::abort();
	//}

	return cost;
}

int ETT_ComponentDeltaCostExamDistance::ComputeDeltaCost(const ETT_State& st, const ETT_ComponentExam& c) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	int cost = 0;
	int kAbs = std::abs(c.k);

	auto slotIndex = c.slotsUsed.find(c.slot);
	if(c.k < 0) slotIndex++;
	auto slotEnd = c.slotsUsed.end();

	int dummy;
	for(auto s1=c.slotsUsed.begin(); *s1 != *slotIndex; ++s1){
		for(auto s2=slotIndex; s2 != slotEnd; ++s2){
			// The code below should be slightly faster
			//dummy = 0;
			//for(auto& e1 : c.examsInSlotsUsed.at(*s1)){
			//	for(auto& e2 : c.examsInSlotsUsed.at(*s2)){
			//		dummy += in.getConflict(e1,e2);
			//	}
			//}

			dummy = 0;
			for(auto& e1 : c.examsInSlotsUsed.at(*s1)){
				dummy += st.getExamPeriodConflicts(e1, *s2);
			}

			if(*s2-*s1 <= in.getPeriodSpread() && *s2-*s1+kAbs > in.getPeriodSpread()){
				// The period spread issue is fixed
				cost -= dummy;
			}

			// Check the old periodDay violations
			if(in.getPeriodDay(*s1) == in.getPeriodDay(*s2)){
				if(*s1+1==*s2){
					// twoInARow
					cost -= in.getTwoInARowCost()*dummy;
				} else {
					// twoInADay
					cost -= in.getTwoInADayCost()*dummy;
				}
			}	
			// Check the new periodDay violations
			int s1New = (c.k < 0) ? *s1 + c.k : *s1;
			int s2New = (c.k < 0) ? *s2 : *s2 + c.k;
			if(in.getPeriodDay(s1New) == in.getPeriodDay(s2New)){
				if(s1New+1==s2New){
					// twoInARow
					cost += in.getTwoInARowCost()*dummy;
				} else {
					// twoInADay
					cost += in.getTwoInADayCost()*dummy;
				}
			}		
		}
	}
	// Check the period day violations between periods being moved
	for(auto s1 = c.slotsMovedFrom.begin(); s1 != c.slotsMovedFrom.end(); ++s1){
		for(auto s2 = std::next(s1,1); s2 != c.slotsMovedFrom.end(); ++s2){
			dummy = 0;
			for(auto& e1 : c.examsInSlotsUsed.at(*s1)){
				for(auto& e2 : c.examsInSlotsUsed.at(*s2)){
					dummy += in.getConflict(e1,e2);
				}
			}

			if(in.getPeriodDay(*s1) == in.getPeriodDay(*s2)){
				if(*s1+1==*s2){
					// twoInARow
					cost -= in.getTwoInARowCost()*dummy;
				} else {
					// twoInADay
					cost -= in.getTwoInADayCost()*dummy;
				}
			}
			if(in.getPeriodDay(*s1+c.k) == in.getPeriodDay(*s2+c.k)){
				if(*s1+1==*s2){
					// twoInARow
					cost += in.getTwoInARowCost()*dummy;
				} else {
					// twoInADay
					cost += in.getTwoInADayCost()*dummy;
				}
			} 
		}
	}

	return cost;
}

int ETT_ShakeDeltaCostExamDistance::ComputeDeltaCost(const ETT_State& st, const ETT_ShakeExam& sh) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	int cost = 0;

	int con_p1_p2 = 0;
	int con_p1_p1 = 0;
	int con_p2_p2 = 0;
	int i,s;
	for(i=0; i < in.getNoExams(); ++i){
		s = st.periodAssignment[i];

		// Only account con in period1 to avoid double counting
		// In con_p1_p1 and 4: double counting unavoidable
		if(s == sh.period1){
			cost += deltaCostMove(st, in, i, sh.period1, sh.period2);
			con_p1_p2 += st.getExamPeriodConflicts(i, sh.period2);
			con_p1_p1 += st.getExamPeriodConflicts(i, sh.period1);
		} else if(s == sh.period2){
			cost += deltaCostMove(st, in, i, sh.period2, sh.period1);
			con_p2_p2 += st.getExamPeriodConflicts(i, sh.period2);
		}
	}

	// Correction for exams themselves
	// Compute weight of conflicts between the chains	
	int diff = std::abs(sh.period2-sh.period1);
	if(diff <= in.getPeriodSpread()){
		// Recall: con_p1_p1 and con_p2_p2 already double count
		cost += 2*con_p1_p2;
		cost -= con_p1_p1;
		cost -= con_p2_p2;
	}

	if(in.getPeriodDay(sh.period1) == in.getPeriodDay(sh.period2)){
		if(diff==1){
			// twoInARow
			cost += 2*con_p1_p2*in.getTwoInARowCost();
			cost -= con_p1_p1*in.getTwoInARowCost();
			cost -= con_p2_p2*in.getTwoInARowCost();
		} else {
			// twoInADay
			cost += 2*con_p1_p2*in.getTwoInADayCost();
			cost -= con_p1_p1*in.getTwoInADayCost();
			cost -= con_p2_p2*in.getTwoInADayCost();
		}
	} 

	//int a,j,t,oldDist,newDist,oldP,newP;
	//for(i=0; i < in.getNoExams(); ++i){
	//	s = st.periodAssignment[i];
	//	if(s != sh.period1 && s != sh.period2) { continue; }

	//	if(s == sh.period1){
	//		oldP = sh.period1;
	//		newP = sh.period2;
	//	} else {
	//		oldP = sh.period2;
	//		newP = sh.period1;
	//	}

	//	for(a=0; a < in.noConflicts(i); ++a){
	//		j = in.getConflictList(i, a);
	//		t = st.periodAssignment[j];
	//		// Spread does not change
	//		if(t == newP || t == oldP) {continue;}

	//		oldDist = std::abs(t-oldP);	
	//		newDist = std::abs(t-newP);
	//		if(oldDist <= in.getPeriodSpread() && oldDist > 0){ // Do not account for spread if in conflict
	//			cost -= in.getConflict(i, j); // periodSpread, which has cost 1 by default
	//		}			
	//		if(newDist <= in.getPeriodSpread() && newDist > 0){
	//			cost += in.getConflict(i, j); // periodSpread, which has cost 1 by default
	//		}

	//		if(oldDist > 0 && in.getPeriodDay(oldP) == in.getPeriodDay(t)){
	//			if(oldDist == 1){
	//				// twoInARow
	//				cost -= in.getTwoInARowCost()*in.getConflict(i,j);
	//			} else {
	//				// twoInADay
	//				cost -= in.getTwoInADayCost()*in.getConflict(i,j);
	//			}
	//		} 
	//		if(newDist > 0 && in.getPeriodDay(newP) == in.getPeriodDay(t)){
	//			if(newDist == 1){
	//				// twoInARow
	//				cost += in.getTwoInARowCost()*in.getConflict(i,j);
	//			} else {
	//				// twoInADay
	//				cost += in.getTwoInADayCost()*in.getConflict(i,j);
	//			}
	//		}
	//	}
	//}

	return cost;
}


int ETT_MixedDurations::ComputeCost(const ETT_State& st) const {
	int cost = 0;

	int r,p,k;
	for (r = 0; r < in.getNoRooms(); ++r) {
		for(p=0; p < in.getNoPeriods(); ++p){
			if(st.getRoomExams(r,p) > 0){
				// Room r is used during period p
				for(k=0; k < in.getNoUniqueExamDurations(); k++){
					if(st.getRoomDurations(r,p,k) > 0) cost+=in.getNonMixedDurationsCost();
				}
				// One type of duration comes at no cost
				cost-=in.getNonMixedDurationsCost();
			}
		}	
	}

	return cost;
}

void ETT_MixedDurations::PrintViolations(const ETT_State& st, std::ostream& os) const {
	int r,p,k,cntr;
	for (r = 0; r < in.getNoRooms(); ++r) {
		for(p=0; p < in.getNoPeriods(); ++p){
			if(st.getRoomExams(r,p) > 0){
				cntr = 0;
				// Room r is used during period p
				for(k=0; k < in.getNoUniqueExamDurations(); k++){
					if(st.getRoomDurations(r,p,k) > 0) cntr++;
				}
				if(cntr > 1){
					os << "Mixed duration: room " << r << ", period " << p << " (" << cntr << ")" << std::endl;
				}
			}
		}	
	}
}

int ETT_MoveDeltaCostMixedDurations::ComputeDeltaCost(const ETT_State& st, const ETT_MoveExam& mv) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	int cost = 0;

	// O(1)
	int dur = in.getExamDurationIndex(mv.exam);
	// More than one exam, but mv.exam is the only one with this type of duration
	if(st.getRoomExams(mv.oldRoom,mv.from) > 1 && st.getRoomDurations(mv.oldRoom, mv.from, dur) == 1) cost -= in.getNonMixedDurationsCost();
	if(st.getRoomExams(mv.newRoom,mv.to) > 0 && st.getRoomDurations(mv.newRoom, mv.to, dur) == 0) cost += in.getNonMixedDurationsCost();

	return cost;
}

int ETT_SwapDeltaCostMixedDurations::ComputeDeltaCost(const ETT_State& st, const ETT_SwapExam& swp) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	int cost = 0;

	// O(1)
	int dur1 = in.getExamDurationIndex(swp.exam1);
	int dur2 = in.getExamDurationIndex(swp.exam2);
	if(dur1 == dur2) return cost;

	// More than one exam, but swp.exam1 is the only one with this type of duration
	if(st.getRoomExams(swp.room1,swp.period1) > 1 && st.getRoomDurations(swp.room1, swp.period1, dur1) == 1) cost -= in.getNonMixedDurationsCost();
	if(st.getRoomExams(swp.room2,swp.period2) > 1 && st.getRoomDurations(swp.room2, swp.period2, dur1) == 0) cost += in.getNonMixedDurationsCost();

	// Same but for swp.exam2
	if(st.getRoomExams(swp.room2,swp.period2) > 1 && st.getRoomDurations(swp.room2, swp.period2, dur2) == 1) cost -= in.getNonMixedDurationsCost();
	if(st.getRoomExams(swp.room1,swp.period1) > 1 && st.getRoomDurations(swp.room1, swp.period1, dur2) == 0) cost += in.getNonMixedDurationsCost();

	return cost;
}

int ETT_KickDeltaCostMixedDurations::ComputeDeltaCost(const ETT_State& st, const ETT_KickExam& kick) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	int cost = 0;

	// O(1)
	int dur1 = in.getExamDurationIndex(kick.exam1);
	int dur2 = in.getExamDurationIndex(kick.exam2);
	if(kick.swap && dur1 == dur2) return cost;

	if(st.getRoomExams(kick.room1,kick.period1) > 1 && st.getRoomDurations(kick.room1, kick.period1, dur1) == 1) cost -= in.getNonMixedDurationsCost();
	if (dur1 != dur2 && st.getRoomExams(kick.room2,kick.period2) > 1) {
		if(st.getRoomDurations(kick.room2, kick.period2, dur1) == 0) cost += in.getNonMixedDurationsCost();
		if(st.getRoomDurations(kick.room2, kick.period2, dur2) == 1) cost -= in.getNonMixedDurationsCost();
	}
	if(st.getRoomExams(kick.newRoom,kick.newPeriod) > (kick.swap ? 1 : 0) && st.getRoomDurations(kick.newRoom, kick.newPeriod, dur2) == 0) cost += in.getNonMixedDurationsCost();

	return cost;
}

int ETT_KempeDeltaCostMixedDurations::ComputeDeltaCost(const ETT_State& st, const ETT_KempeExam& kc) const {
	// TODO TODO
	// Can this be made faster using info from the beam operator?
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	int cost = 0;

	// Old cost
	int r,k;


	bool dummy1, dummy2;
	for(int p : {kc.from, kc.to}){
		std::vector<std::vector<int>> roomDurations(in.getNoRooms(), std::vector<int>(in.getNoUniqueExamDurations(), 0));
		for(auto& e : (p==kc.from ? kc.s1 : kc.s2)){
			roomDurations[st.roomAssignment[e]][in.getExamDurationIndex(e)]--;
		}
		for(auto& e : (p==kc.from ? kc.s2 : kc.s1)){
			roomDurations[kc.newRoomAssignment[e]][in.getExamDurationIndex(e)]++;
		}

		for (r = 0; r < in.getNoRooms(); ++r) {
			for(k=0; k < in.getNoUniqueExamDurations(); k++){
				roomDurations[r][k] += st.getRoomDurations(r,p,k);
			}

			dummy1=false;
			dummy2 = false;
			for(k=0; k < in.getNoUniqueExamDurations(); k++){
				if(roomDurations[r][k] > 0){
					dummy1=true;
				       	cost+=in.getNonMixedDurationsCost();
				}
				if(st.getRoomDurations(r,p,k) > 0){
					dummy2=true;
				       	cost-=in.getNonMixedDurationsCost();
				}
			}
			// One duration comes at no cost
			if(dummy1){
				cost-=in.getNonMixedDurationsCost();
			}
			if(dummy2){
				cost+=in.getNonMixedDurationsCost();
			}
		}
	}
	

	return cost;
}

int ETT_ComponentDeltaCostMixedDurations::ComputeDeltaCost(const ETT_State& st, const ETT_ComponentExam& c) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	int cost = c.newMixedRoom;
	int r,k;

	// The new cost of the movedTo's are calculated in newMixedroom
	// Deduct the old cost
	for(auto p : c.slotsMovedTo){
		for (r = 0; r < in.getNoRooms(); ++r) {
			if(st.getRoomExams(r,p) > 0){
				// Room r is used during period p
				for(k=0; k < in.getNoUniqueExamDurations(); k++){
					if(st.getRoomDurations(r,p,k) > 0) cost--;
				}
				// One type of duration comes at no cost
				cost++;
			}
		}	
	}

	// Calculate the delta cost of the movedFrom's that are not in movedTo
	for(auto p : c.slotsMovedFrom){
		if(c.slotsMovedTo.count(p)){ continue; }

		std::vector<std::vector<int>> roomDur; // [r][k] no. exams with duration index k in room r
		roomDur.resize(in.getNoRooms(), std::vector<int>(in.getNoUniqueExamDurations()));

		for (r = 0; r < in.getNoRooms(); ++r) {
			for(k=0; k < in.getNoUniqueExamDurations(); k++){
				roomDur[r][k] = st.getRoomDurations(r,p,k);
				if(roomDur[r][k] > 0){
					cost--;
				}	
			}

			if(st.getRoomExams(r,p) > 0){
				// One duration comes for free
				cost++;
			}
		}

		// Remove the exams
		for(auto &e : c.examsInSlotsUsed.at(p)){
			roomDur[st.roomAssignment[e]][in.getExamDurationIndex(e)]--;
		}

		// Compute the violations with the exams being removed
		bool dummy;
		for (r = 0; r < in.getNoRooms(); ++r) {
			dummy = false;
			for(k=0; k < in.getNoUniqueExamDurations(); ++k){
				if(roomDur[r][k] > 0){
					cost++;
					dummy = true;
				}
			}
			if(dummy){
				// One duration comes at no cost
				cost--;
			}	
		}
	}

	return cost*in.getNonMixedDurationsCost();
}

int ETT_BeamDeltaCostMixedDurations::ComputeDeltaCost(const ETT_State& st, const ETT_BeamExam& b) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
//	int cost = 0;
//
//	// Old cost
//	int r,k, p = b.period;
//	for (r = 0; r < in.getNoRooms(); ++r) {
//		if(st.getRoomExams(r,p) > 0){
//			// Room r is used during period p
//			for(k=0; k < in.getNoUniqueExamDurations(); k++){
//				if(st.getRoomDurations(r,p,k) > 0) cost-=in.getNonMixedDurationsCost();
//			}
//			// One type of duration comes at no cost
//			cost+=in.getNonMixedDurationsCost();
//		}
//	}
//	cost += b.bestFound.durCost;
//
//	return cost;
	return b.bestFound.durCost - b.oldMixedDurCost;
}

int ETT_FrontLoad::ComputeCost(const ETT_State& st) const {
	int cost = 0;

	int a;
	for(a=0; a < in.getFrontLoadSize(); ++a){
		if(st.periodAssignment[in.getFrontLoadIndex(a)] >= in.getFirstFrontLoadPeriod()){
			cost += in.getFrontLoadCost();
		}
	}

	return cost;
}

void ETT_FrontLoad::PrintViolations(const ETT_State& st, std::ostream& os) const {
	int a;
	for(a=0; a < in.getFrontLoadSize(); ++a){
		if(st.periodAssignment[in.getFrontLoadIndex(a)] >= in.getFirstFrontLoadPeriod()){
			os << "Frontload exam " << in.getFrontLoadIndex(a) << " on period " << in.getFrontLoadIndex(a) << std::endl;
		} 
	}
}

int ETT_MoveDeltaCostFrontLoad::ComputeDeltaCost(const ETT_State& st, const ETT_MoveExam& mv) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	int cost = 0;

	if(!in.isFrontLoad(mv.exam)) return cost;

	if(mv.from >= in.getFirstFrontLoadPeriod()){
		cost -= in.getFrontLoadCost();
	}
	if(mv.to >= in.getFirstFrontLoadPeriod()){
		cost += in.getFrontLoadCost();
	}

	return cost;
}

int ETT_SwapDeltaCostFrontLoad::ComputeDeltaCost(const ETT_State& st, const ETT_SwapExam& swp) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	int cost = 0;

	if(in.isFrontLoad(swp.exam1) + in.isFrontLoad(swp.exam2) != 1 // None or both frontload
	|| ( (swp.period1 >= in.getFirstFrontLoadPeriod()) + (swp.period2 >= in.getFirstFrontLoadPeriod()) != 1)){ // Both or none periods frontload
		return cost;
	}

	if((in.isFrontLoad(swp.exam1) && swp.period2 >= in.getFirstFrontLoadPeriod()) 
	|| (in.isFrontLoad(swp.exam2) && swp.period1 >= in.getFirstFrontLoadPeriod())){ 
		cost += in.getFrontLoadCost();
	} else { 
		cost -= in.getFrontLoadCost();
	}

	return cost;
}

int ETT_KickDeltaCostFrontLoad::ComputeDeltaCost(const ETT_State& st, const ETT_KickExam& kick) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	int cost = 0;
	int a = in.getFirstFrontLoadPeriod();
	if (in.isFrontLoad(kick.exam1)) {
		cost += in.getFrontLoadCost()*((kick.period2 >= a) - (kick.period1 >= a));
	}
	if(in.isFrontLoad(kick.exam2)){
		cost += in.getFrontLoadCost()*((kick.newPeriod >= a) - (kick.period2 >= a));
	}
	return cost;
}

int ETT_KempeDeltaCostFrontLoad::ComputeDeltaCost(const ETT_State& st, const ETT_KempeExam& kc) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	const int a = in.getFirstFrontLoadPeriod();
	const bool fromIsFrontLoad = (kc.from >= a);
	const bool toIsFrontLoad = (kc.to >= a);
	if(fromIsFrontLoad + toIsFrontLoad != 1){
		return 0;
	} else {
		int frontS1=0;
		for(auto& e : kc.s1) frontS1 += in.isFrontLoad(e);
		int frontS2=0;
		for(auto& e : kc.s2) frontS2 += in.isFrontLoad(e);

		const int diff = frontS2-frontS1;
		return (diff*fromIsFrontLoad - diff*toIsFrontLoad)*in.getFrontLoadCost();
	}

}

int ETT_ComponentDeltaCostFrontLoad::ComputeDeltaCost(const ETT_State& st, const ETT_ComponentExam& c) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	int cost = 0;
	for(auto& s : c.slotsMovedFrom){
		if(s < in.getFirstFrontLoadPeriod()) continue;
		for(auto& e : c.examsInSlotsUsed.at(s)){
			if(in.isFrontLoad(e)) cost--;
		}
	}
	for(auto& s : c.slotsMovedTo){
		if(s < in.getFirstFrontLoadPeriod()) continue;
		for(auto& e : c.examsInSlotsUsed.at(s-c.k)){
			if(in.isFrontLoad(e)) cost++;
		}
	}
	return cost*in.getFrontLoadCost();
}

int ETT_ShakeDeltaCostFrontLoad::ComputeDeltaCost(const ETT_State& st, const ETT_ShakeExam& sh) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	int cost = 0;

	assert(sh.period1 < sh.period2);
	if(sh.period1 < in.getFirstFrontLoadPeriod() && sh.period2 >= in.getFirstFrontLoadPeriod()){
		// p2 is a frontload period, whereas p1 is not

		int a,s;
		for(a=0; a < in.getFrontLoadSize(); ++a){
			s = st.periodAssignment[in.getFrontLoadIndex(a)];
			if(s == sh.period1){
				// Front load exams in p1 result in a new violations
				cost += in.getFrontLoadCost();
			}else if(s == sh.period2){
				// Front load exams in p2 are solved
				cost -= in.getFrontLoadCost();
			}
		}
	}
	return cost;
}

int ETT_RoomPeriodPenalty::ComputeCost(const ETT_State& st) const {
	int cost = 0;
	int e;
	for(e=0; e < in.getNoExams(); ++e){
		cost += in.getRoomPenalty(st.roomAssignment[e]);
		cost += in.getPeriodPenalty(st.periodAssignment[e]);
	}

	return cost;
}

void ETT_RoomPeriodPenalty::PrintViolations(const ETT_State& st, std::ostream& os) const {
	int e;
	for(e=0; e < in.getNoExams(); ++e){
		if(in.getRoomPenalty(st.roomAssignment[e]) > 0){
			os << "Room penalty: exam " << e << " in room " << st.roomAssignment[e] << " (" << in.getRoomPenalty(st.roomAssignment[e]) << ")" << std::endl;
		}
		if(in.getPeriodPenalty(st.periodAssignment[e]) > 0){
			os << "Period penalty: exam " << e << " in period " << st.periodAssignment[e] << " (" << in.getPeriodPenalty(st.periodAssignment[e]) << ")" << std::endl;
		}
	}
}

inline int ETT_MoveDeltaCostRoomPeriodPenalty::ComputeDeltaCost(const ETT_State& st, const ETT_MoveExam& mv) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	return -in.getRoomPenalty(mv.oldRoom) - in.getPeriodPenalty(mv.from)
		+ in.getRoomPenalty(mv.newRoom) + in.getPeriodPenalty(mv.to);
}

inline int ETT_KickDeltaCostRoomPeriodPenalty::ComputeDeltaCost(const ETT_State& st, const ETT_KickExam& kick) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	//return cost;
	return -in.getRoomPenalty(kick.room1) - in.getPeriodPenalty(kick.period1)
		+ in.getRoomPenalty(kick.newRoom) + in.getPeriodPenalty(kick.newPeriod);
}

int ETT_KempeDeltaCostRoomPeriodPenalty::ComputeDeltaCost(const ETT_State& st, const ETT_KempeExam& kc) const {
	// TODO Can this be made faster by info from beam?
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	const int diff = kc.s1.size()-kc.s2.size();
	int roomDelta=0;
	for(auto& chain : {kc.s1, kc.s2}){
		for(auto& e : chain){
			roomDelta -= in.getRoomPenalty(st.roomAssignment[e]);
			roomDelta += in.getRoomPenalty(kc.newRoomAssignment[e]);
		}
	}
	return -in.getPeriodPenalty(kc.from)*diff + in.getPeriodPenalty(kc.to)*diff + roomDelta;
}

int ETT_ComponentDeltaCostRoomPeriodPenalty::ComputeDeltaCost(const ETT_State& st, const ETT_ComponentExam& c) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	int cost = 0;

	int s2;
	for(auto& s : c.slotsMovedTo){
		s2 = s-c.k;
		cost += (in.getPeriodPenalty(s)-in.getPeriodPenalty(s2))*c.examsInSlotsUsed.at(s2).size();
		for(auto& e : c.examsInSlotsUsed.at(s2)){
			cost -= in.getRoomPenalty(st.roomAssignment[e]);
			cost += in.getRoomPenalty(c.newRoomAssignment[e]);
		}
	}

	return cost;
}

int ETT_ShakeDeltaCostRoomPeriodPenalty::ComputeDeltaCost(const ETT_State& st, const ETT_ShakeExam& sh) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	// We only need to check the period penalty
	int cost = 0;
	
	if(in.getPeriodPenalty(sh.period1) != in.getPeriodPenalty(sh.period2)){
		int noExamsP1 = 0;
		int noExamsP2 = 0;
		int r;
		for (r = 0; r < in.getNoRooms(); ++r) {
			noExamsP1 += st.getRoomExams(r,sh.period1);
			noExamsP2 += st.getRoomExams(r,sh.period2);
		}
		int diff = noExamsP2 - noExamsP1;
		cost = diff*in.getPeriodPenalty(sh.period1) - diff*in.getPeriodPenalty(sh.period2);
	}
	
	return cost;
}

int ETT_BeamDeltaCostRoomPeriodPenalty::ComputeDeltaCost(const ETT_State& st, const ETT_BeamExam& b) const {
	if(st.hardViolatedAndFeasOnly){
		return 0;
	}
	//for(auto e : b.examIds){
	//	roomDelta -= in.getRoomPenalty(st.roomAssignment[e.first]);
	//}
	//roomDelta += b.bestFound.roomCost;
	//return roomDelta;
	return b.bestFound.roomCost - b.oldRoomPenalty;
}
