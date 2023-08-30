#include "ETT_Move.h"

/**************
*  ETT_Move  *
**************/
ETT_MoveExam::ETT_MoveExam() {
	// Insert the code that initialize the move
	exam = 0;
	from = 0;
	to = 0;
	oldRoom = 0;
	newRoom = 0;
	deltaConflicts = 0;
}

bool operator==(const ETT_MoveExam& mv1, const ETT_MoveExam& mv2) {
	// Insert the code that checks if two moves are identical
	return mv1.exam == mv2.exam && 
		mv1.to == mv2.to && 
		mv1.newRoom == mv2.newRoom;
}

bool operator!=(const ETT_MoveExam& mv1, const ETT_MoveExam& mv2) {
	// Insert the code that checks if two moves are different
	return mv1.exam != mv2.exam ||
		mv1.to != mv2.to ||
		mv1.newRoom != mv2.newRoom;	
}

bool operator<(const ETT_MoveExam& mv1, const ETT_MoveExam& mv2) {
	// Insert the code that checks if one move precedes another one
	// (in any selected order)
	return mv1.exam < mv2.exam ||
		(mv1.exam == mv2.exam && mv1.to < mv2.to) ||
		(mv1.exam == mv2.exam && mv1.to == mv2.to && mv1.newRoom < mv2.newRoom);
}

std::ostream& operator<<(std::ostream& os, const ETT_MoveExam& mv) {
	// Insert the code that writes a move
	return os << mv.exam << ":" << mv.from << "->" << mv.to << "|" << mv.oldRoom << "->" << mv.newRoom;
}

std::istream& operator>>(std::istream& is, ETT_MoveExam& mv) {
	// Insert the code that reads a move
	char ch;
	return is >> mv.exam >> ch >> mv.from >> ch >> ch >> mv.to >> ch >> mv.oldRoom >> ch >> mv.newRoom;
}

/**************
*  ETT_Swap  *
**************/
ETT_SwapExam::ETT_SwapExam() {
	// Insert the code that initialize the move
	exam1 = 0;
	exam2 = 0;
	period1 = 0;
	period2 = 0;
	room1 = 0;
	room2 = 0;
	deltaConflicts = 0;
}

bool operator==(const ETT_SwapExam& swp1, const ETT_SwapExam& swp2) {
	// Insert the code that checks if two moves are identical
	return swp1.exam1 == swp2.exam1 && 
		swp1.exam2 == swp2.exam2;
}

bool operator!=(const ETT_SwapExam& swp1, const ETT_SwapExam& swp2) {
	// Insert the code that checks if two moves are different
	return swp1.exam1 != swp2.exam1 ||
		swp1.exam2 != swp2.exam2;
}

bool operator<(const ETT_SwapExam& swp1, const ETT_SwapExam& swp2) {
	// Insert the code that checks if one move precedes another one
	// (in any selected order)
	return swp1.exam1 < swp2.exam1 ||
		(swp1.exam1 == swp2.exam1 && swp1.exam2 < swp2.exam2);
}

std::ostream& operator<<(std::ostream& os, const ETT_SwapExam& swp) {
	// Insert the code that writes a move
	return os << swp.exam1 << ":" << swp.period1 << "-" << swp.room1 << "<->" << swp.exam2 << ":" << swp.period2 << "-" << swp.room2;
}

std::istream& operator>>(std::istream& is, ETT_SwapExam& swp) {
	// Insert the code that reads a move
	char ch;
	return is >> swp.exam1 >> ch >> swp.period1 >> ch >> swp.room1 >> ch >> ch >> ch >> swp.exam2 >> ch >> swp.period2 >> ch >> swp.room2;
}

/**************
*  ETT_Kick  *
**************/
ETT_KickExam::ETT_KickExam() {
	// Insert the code that initialize the move
	exam1 = 0;
	exam2 = 0;
	room1 = 0;
	room2 = 0;
	period1 = 0;
	period2 = 0;
	newPeriod = 0;
	newRoom = 0;
	deltaConflicts = 0;
}

bool operator==(const ETT_KickExam& kick1, const ETT_KickExam& kick2) {
	// Insert the code that checks if two moves are identical
	return kick1.exam1 == kick2.exam1 &&
		kick1.exam2 == kick2.exam2 &&
		kick1.newPeriod == kick2.newPeriod &&
		kick1.newRoom == kick2.newRoom;
}

bool operator!=(const ETT_KickExam& kick1, const ETT_KickExam& kick2) {
	// Insert the code that checks if two moves are different
	return kick1.exam1 != kick2.exam1 ||
		kick1.exam2 != kick2.exam2 ||
		kick1.newPeriod != kick2.newPeriod ||
		kick1.newRoom != kick2.newRoom;
}

bool operator<(const ETT_KickExam& kick1, const ETT_KickExam& kick2) {
	// Insert the code that checks if one move precedes another one
	// (in any selected order)
	return kick1.exam1 < kick2.exam1 ||
		(kick1.exam1 == kick2.exam1 && kick1.exam2 < kick2.exam2) ||
		(kick1.exam1 == kick2.exam1 && kick1.exam2 == kick2.exam2 && kick1.newPeriod < kick2.newPeriod) ||
		(kick1.exam1 == kick2.exam1 && kick1.exam2 == kick2.exam2 && kick1.newPeriod == kick2.newPeriod && kick1.newRoom < kick2.newRoom);
}

std::ostream& operator<<(std::ostream& os, const ETT_KickExam& kick) {
	// Insert the code that writes a move
	return os << kick.exam1 << "-" << kick.exam2 << ":" << kick.period1 << "->" << kick.period2 << "->" << kick.newPeriod << "|" << kick.room1 << "->" << kick.room2 << "->" << kick.newRoom;
}

std::istream& operator>>(std::istream& is, ETT_KickExam& kick) {
	// Insert the code that reads a move
	char ch;
	return is >> kick.exam1 >> ch >> kick.exam2 >> ch >> kick.period1 >> ch >> ch >> ch >> kick.period2 >> ch >> ch >> ch >> kick.newPeriod >> ch >> kick.room1 >> ch >> ch >> ch >> kick.room2 >> ch >> ch >> ch >> kick.newRoom;
}

/**************
*  ETT_Kempe  *
**************/
ETT_KempeExam::ETT_KempeExam() {
	// Insert the code that initialize the move
	exam = 0;
	from = 0;
	to = 0;
	oldRoom = 0;
	newRoom = -1;
	newCapViol = 0;
}

bool operator==(const ETT_KempeExam& kc1, const ETT_KempeExam& kc2) {
	// Insert the code that checks if two moves are identical
	return kc1.exam == kc2.exam && 
		kc1.to == kc2.to && 
		kc1.newRoom == kc2.newRoom;
}

bool operator!=(const ETT_KempeExam& kc1, const ETT_KempeExam& kc2) {
	// Insert the code that checks if two moves are different
	return kc1.exam != kc2.exam ||
		kc1.to != kc2.to ||
		kc1.newRoom != kc2.newRoom;	
}

bool operator<(const ETT_KempeExam& kc1, const ETT_KempeExam& kc2) {
	// Insert the code that checks if one move precedes another one
	// (in any selected order)
	return kc1.exam < kc2.exam ||
		(kc1.exam == kc2.exam && kc1.to < kc2.to) ||
		(kc1.exam == kc2.exam && kc1.to == kc2.to && kc1.newRoom < kc2.newRoom);
}

std::ostream& operator<<(std::ostream& os, const ETT_KempeExam& kc) {
	// Insert the code that writes a move
	return os << kc.exam << ":" << kc.from << "->" << kc.to << "|" << kc.oldRoom << "-" << kc.newRoom;
}

std::istream& operator>>(std::istream& is, ETT_KempeExam& kc) {
	// Insert the code that reads a move
	char ch;
	return is >> kc.exam >> ch >> kc.from >> ch >> ch >> kc.to >> ch >> kc.oldRoom >> ch >> kc.newRoom;
}

/***************
*  ETT_Shake  *
***************/
ETT_ShakeExam::ETT_ShakeExam() {
	// Insert the code that initialize the move
	period1 = 0;
	period2 = 0;
}

bool operator==(const ETT_ShakeExam& sh1, const ETT_ShakeExam& sh2) {
	// Insert the code that checks if two moves are identical
	return sh1.period1 == sh2.period1 && 
		sh1.period2 == sh2.period2;
}

bool operator!=(const ETT_ShakeExam& sh1, const ETT_ShakeExam& sh2) {
	// Insert the code that checks if two moves are different
	return sh1.period1 != sh2.period1 ||
		sh1.period2 != sh2.period2;
}

bool operator<(const ETT_ShakeExam& sh1, const ETT_ShakeExam& sh2) {
	// Insert the code that checks if one move precedes another one
	// (in any selected order)
	return sh1.period1 < sh2.period1 ||
		(sh1.period1 == sh2.period1 && sh1.period2 < sh2.period2);
}

std::ostream& operator<<(std::ostream& os, const ETT_ShakeExam& sh) {
	// Insert the code that writes a move
	return os << sh.period1 << "<->" << sh.period2;
}

std::istream& operator>>(std::istream& is, ETT_ShakeExam& sh) {
	// Insert the code that reads a move
	char ch;
	return is >> sh.period1 >> ch >> ch >> ch >> sh.period2 >> ch;
}

/******************
*  ETT_Component  *
*******************/
ETT_ComponentExam::ETT_ComponentExam() {
	// Insert the code that initialize the move
	component = 0;
	slot = 0;
	k = 0;
}

bool operator==(const ETT_ComponentExam& c1, const ETT_ComponentExam& c2) {
	// Insert the code that checks if two moves are identical
	return c1.component == c2.component && 
		c1.slot == c2.slot && 
		c1.k == c2.k;
}

bool operator!=(const ETT_ComponentExam& c1, const ETT_ComponentExam& c2) {
	// Insert the code that checks if two moves are different
	return c1.component != c2.component ||
		c1.slot != c2.slot ||
		c1.k != c2.k;
}

bool operator<(const ETT_ComponentExam& c1, const ETT_ComponentExam& c2) {
	// Insert the code that checks if one move precedes another one
	// (in any selected order)
	return c1.component < c2.component ||
		(c1.component == c2.component && c1.slot < c2.slot) ||
		(c1.component == c2.component && c1.slot == c2.slot && c1.k < c2.k);
}

std::ostream& operator<<(std::ostream& os, const ETT_ComponentExam& c) {
	// Insert the code that writes a move
	return os << c.component << " - " << c.slot << " - " << c.k;
}

std::istream& operator>>(std::istream& is, ETT_ComponentExam& c) {
	// Insert the code that reads a move
	char ch;
	return is >> c.component >> ch >> c.slot >> ch >> c.k;
}

/**************
*  ETT_Beam  *
**************/
ETT_BeamExam::ETT_BeamExam() {
	// Insert the code that initializes the move
	examIds.clear();
}

bool operator==(const ETT_BeamExam& b1, const ETT_BeamExam& b2) {
	// Insert the code that checks if two moves are identical
	// This requires C++11!
	return b1.examIds == b2.examIds;
}

bool operator!=(const ETT_BeamExam& b1, const ETT_BeamExam& b2) {
	// Insert the code that checks if two moves are different
	return b1.examIds != b1.examIds;
}

bool operator<(const ETT_BeamExam& b1, const ETT_BeamExam& b2) {
	// Insert the code that checks if one move precedes another one
	// (in any selected order)
	
	//assert(b1.examIds.size() == b2.examIds.size());

	std::set<int> setIds1 = {};
	for(auto &s : b1.examIds) setIds1.insert(s.first);

	std::set<int> setIds2 = {};
	for(auto &s : b2.examIds) setIds2.insert(s.first);

	return std::lexicographical_compare(setIds1.begin(),setIds1.end(),setIds2.begin(),setIds2.end());

}

std::ostream& operator<<(std::ostream& os, const ETT_BeamExam& b) {
	// Insert the code that writes a move
	std::string o = std::to_string(b.period) + ": ";
	for(auto &e : b.examIds){
		o += std::to_string(e.first) + " ";
	}
	return os << o;

}

std::istream& operator>>(std::istream& is, ETT_BeamExam& b) {
	// Insert the code that reads a move
	char ch;
	return is >> b >> ch;
}

bool nodeComparison(node *lhs, node *rhs) {
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
