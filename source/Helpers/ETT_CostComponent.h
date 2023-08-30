#ifndef ETT_COSTCOMPONENT_H
#define ETT_COSTCOMPONENT_H


#include <iostream>
#include "../Basics/ETT_State.h"
#include "../Basics/ETT_Move.h"
#include "ETT_StateManager.h"

// Hard
// At most one exam for each student in each period
// For each pair of exams, a deviation equal to the number of students in common
class ETT_ExamConflicts : public EasyLocal::Core::CostComponent<ETT_Input,ETT_State> {
	public:
		ETT_ExamConflicts(const ETT_Input & in, int w, bool hard) : CostComponent<ETT_Input,ETT_State>(in,w,hard,"ExamConflicts"){}
		int ComputeCost(const ETT_State& st) const;
		void PrintViolations(const ETT_State& st, std::ostream& os = std::cout) const;
};

class ETT_MoveDeltaCostExamConflicts : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_MoveExam> {
	public:
		ETT_MoveDeltaCostExamConflicts(const ETT_Input & in, ETT_ExamConflicts& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_MoveExam>(in,cc,"ETT_MoveDeltaCostConflicts"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_MoveExam& mv) const;
	public:
		bool feasibleOnly=false;
};

class ETT_SwapDeltaCostExamConflicts : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_SwapExam> {
	public:
		ETT_SwapDeltaCostExamConflicts(const ETT_Input & in, ETT_ExamConflicts& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_SwapExam>(in,cc,"ETT_SwapDeltaCostConflicts"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_SwapExam& swp) const;
	public:
		bool feasibleOnly=false;
};

class ETT_KickDeltaCostExamConflicts : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_KickExam> {
	public:
		ETT_KickDeltaCostExamConflicts(const ETT_Input & in, ETT_ExamConflicts& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_KickExam>(in,cc,"ETT_KickDeltaCostConflicts"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_KickExam& swp) const;
	public:
		bool feasibleOnly=false;
};

class ETT_KempeDeltaCostExamConflicts : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_KempeExam> {
	public:
		ETT_KempeDeltaCostExamConflicts(const ETT_Input & in, ETT_ExamConflicts& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_KempeExam>(in,cc,"ETT_KempeDeltaCostConflicts"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_KempeExam& mv) const;
	public:
		bool feasibleOnly=false;
};

// Hard
// Respect capacity of each room in every period
// For each room and period, a deviation equal to the number of allocated students more than the room capacity
class ETT_RoomOccupancy : public EasyLocal::Core::CostComponent<ETT_Input,ETT_State> {
	public:
		ETT_RoomOccupancy(const ETT_Input & in, int w, bool hard) : CostComponent<ETT_Input,ETT_State>(in,w,hard,"RoomOccupancy"){}
		int ComputeCost(const ETT_State& st) const;
		void PrintViolations(const ETT_State& st, std::ostream& os = std::cout) const;
};

class ETT_MoveDeltaCostRoomOccupancy : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_MoveExam> {
	public:
		ETT_MoveDeltaCostRoomOccupancy(const ETT_Input & in, ETT_RoomOccupancy& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_MoveExam>(in,cc,"ETT_MoveDeltaCostRoomOccupancy"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_MoveExam& mv) const;
	public:
		bool feasibleOnly=false;
};

class ETT_SwapDeltaCostRoomOccupancy : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_SwapExam> {
	public:
		ETT_SwapDeltaCostRoomOccupancy(const ETT_Input & in, ETT_RoomOccupancy& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_SwapExam>(in,cc,"ETT_SwapDeltaCostRoomOccupancy"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_SwapExam& swp) const;
	public:
		bool feasibleOnly=false;
};

class ETT_KickDeltaCostRoomOccupancy : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_KickExam> {
	public:
		ETT_KickDeltaCostRoomOccupancy(const ETT_Input & in, ETT_RoomOccupancy& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_KickExam>(in,cc,"ETT_KickDeltaCostRoomOccupancy"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_KickExam& swp) const;
	public:
		bool feasibleOnly=false;
};

class ETT_KempeDeltaCostRoomOccupancy : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_KempeExam> {
	public:
		ETT_KempeDeltaCostRoomOccupancy(const ETT_Input & in, ETT_RoomOccupancy& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_KempeExam>(in,cc,"ETT_KempeDeltaCostRoomOccupancy"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_KempeExam& mv) const;
	public:
		bool feasibleOnly=false;
};

class ETT_ComponentDeltaCostRoomOccupancy : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_ComponentExam> {
	public:
		ETT_ComponentDeltaCostRoomOccupancy(const ETT_Input & in, ETT_RoomOccupancy& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_ComponentExam>(in,cc,"ETT_ComponentDeltaCostRoomOccupancy"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_ComponentExam& mv) const;
	public:
		bool feasibleOnly=false;
};


class ETT_BeamDeltaCostRoomOccupancy : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_BeamExam> {
	public:
		ETT_BeamDeltaCostRoomOccupancy(const ETT_Input & in, ETT_RoomOccupancy& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_BeamExam>(in,cc,"ETT_BeamDeltaCostRoomOccupancy"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_BeamExam& b) const;
	public:
		bool feasibleOnly=false;
};

// Hard
// Respect exam sequencing relations
// For each violated sequence relation, a deviation of 1
class ETT_ExamSequence : public EasyLocal::Core::CostComponent<ETT_Input,ETT_State> {
	public:
		ETT_ExamSequence(const ETT_Input & in, int w, bool hard) : CostComponent<ETT_Input,ETT_State>(in,w,hard,"ExamSequence"){}
		int ComputeCost(const ETT_State& st) const;
		void PrintViolations(const ETT_State& st, std::ostream& os = std::cout) const;
};

class ETT_MoveDeltaCostExamSequence : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_MoveExam> {
	public:
		ETT_MoveDeltaCostExamSequence(const ETT_Input & in, ETT_ExamSequence& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_MoveExam>(in,cc,"ETT_MoveDeltaCostExamSequence"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_MoveExam& mv) const;
	public:
		bool feasibleOnly=false;
};

class ETT_SwapDeltaCostExamSequence : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_SwapExam> {
	public:
		ETT_SwapDeltaCostExamSequence(const ETT_Input & in, ETT_ExamSequence& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_SwapExam>(in,cc,"ETT_SwapDeltaCostExamSequence"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_SwapExam& mv) const;
	public:
		bool feasibleOnly=false;
};

class ETT_KickDeltaCostExamSequence : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_KickExam> {
	public:
		ETT_KickDeltaCostExamSequence(const ETT_Input & in, ETT_ExamSequence& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_KickExam>(in,cc,"ETT_KickDeltaCostExamSequence"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_KickExam& mv) const;
	public:
		bool feasibleOnly=false;
};

class ETT_KempeDeltaCostExamSequence : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_KempeExam> {
	public:
		ETT_KempeDeltaCostExamSequence(const ETT_Input & in, ETT_ExamSequence& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_KempeExam>(in,cc,"ETT_KempeDeltaCostExamSequence"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_KempeExam& mv) const;
	public:
		bool feasibleOnly=false;
};

class ETT_ComponentDeltaCostExamSequence : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_ComponentExam> {
	public:
		ETT_ComponentDeltaCostExamSequence(const ETT_Input & in, ETT_ExamSequence& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_ComponentExam>(in,cc,"ETT_ComponentDeltaCostExamSequence"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_ComponentExam& c) const;
	public:
		bool feasibleOnly=false;
};

class ETT_ShakeDeltaCostExamSequence : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_ShakeExam> {
	public:
		ETT_ShakeDeltaCostExamSequence(const ETT_Input & in, ETT_ExamSequence& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_ShakeExam>(in,cc,"ETT_ShakeDeltaCostExamSequence"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_ShakeExam& sh) const;
	public:
		bool feasibleOnly=false;
};

// Hard
// Exam e should receive its own room
// Penalty equal to the number of exams scheduled simultaneously with e
class ETT_RoomExclusive : public EasyLocal::Core::CostComponent<ETT_Input,ETT_State> {
	public:
		ETT_RoomExclusive(const ETT_Input & in, int w, bool hard) : CostComponent<ETT_Input,ETT_State>(in,w,hard,"RoomExclusive"){}
		int ComputeCost(const ETT_State& st) const;
		void PrintViolations(const ETT_State& st, std::ostream& os = std::cout) const;
};

class ETT_MoveDeltaCostRoomExclusive : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_MoveExam> {
	public:
		ETT_MoveDeltaCostRoomExclusive(const ETT_Input & in, ETT_RoomExclusive& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_MoveExam>(in,cc,"ETT_MoveDeltaCostRoomExclusive"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_MoveExam& mv) const;
	public:
		bool feasibleOnly=false;
};

class ETT_SwapDeltaCostRoomExclusive : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_SwapExam> {
	public:
		ETT_SwapDeltaCostRoomExclusive(const ETT_Input & in, ETT_RoomExclusive& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_SwapExam>(in,cc,"ETT_SwapDeltaCostRoomExclusive"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_SwapExam& mv) const;
	public:
		bool feasibleOnly=false;
};

class ETT_KickDeltaCostRoomExclusive : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_KickExam> {
	public:
		ETT_KickDeltaCostRoomExclusive(const ETT_Input & in, ETT_RoomExclusive& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_KickExam>(in,cc,"ETT_KickDeltaCostRoomExclusive"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_KickExam& kick) const;
	public:
		bool feasibleOnly=false;
};

class ETT_KempeDeltaCostRoomExclusive : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_KempeExam> {
	public:
		ETT_KempeDeltaCostRoomExclusive(const ETT_Input & in, ETT_RoomExclusive& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_KempeExam>(in,cc,"ETT_KempeDeltaCostRoomExclusive"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_KempeExam& kc) const;
	public:
		bool feasibleOnly=false;
};

class ETT_ComponentDeltaCostRoomExclusive : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_ComponentExam> {
	public:
		ETT_ComponentDeltaCostRoomExclusive(const ETT_Input & in, ETT_RoomExclusive& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_ComponentExam>(in,cc,"ETT_ComponentDeltaCostRoomExclusive"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_ComponentExam& c) const;
	public:
		bool feasibleOnly=false;
};

class ETT_BeamDeltaCostRoomExclusive : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_BeamExam> {
	public:
		ETT_BeamDeltaCostRoomExclusive(const ETT_Input & in, ETT_RoomExclusive& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_BeamExam>(in,cc,"ETT_BeamDeltaCostRoomExclusive"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_BeamExam& beam) const;
	public:
		bool feasibleOnly=false;
};

// Soft
// Combination of TwoInARow, TwoInADay, and PeriodSpread
// TwoInARow: no student should take two exams in a row on the same day; penalty of 1 otherwise
// TwoInADay: no student should take two exams on the same day; penalty of 1 otherwise
// PeriodSpread: no student should have two exams within PeriodSpread days; penalty of 1 otherwise
class ETT_ExamDistance : public EasyLocal::Core::CostComponent<ETT_Input,ETT_State> {
	public:
		ETT_ExamDistance(const ETT_Input & in, int w, bool hard) : CostComponent<ETT_Input,ETT_State>(in,w,hard,"ExamDistance"){}
		int ComputeCost(const ETT_State& st) const;
		void PrintViolations(const ETT_State& st, std::ostream& os = std::cout) const;
};

class ETT_MoveDeltaCostExamDistance : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_MoveExam> {
	public:
		ETT_MoveDeltaCostExamDistance(const ETT_Input & in, ETT_ExamDistance& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_MoveExam>(in,cc,"ETT_MoveDeltaCostRoomExclusive"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_MoveExam& mv) const;
	public:
		bool feasibleOnly=false;
};

class ETT_SwapDeltaCostExamDistance : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_SwapExam> {
	public:
		ETT_SwapDeltaCostExamDistance(const ETT_Input & in, ETT_ExamDistance& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_SwapExam>(in,cc,"ETT_SwapDeltaCostRoomExclusive"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_SwapExam& mv) const;
	public:
		bool feasibleOnly=false;
};

class ETT_KickDeltaCostExamDistance : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_KickExam> {
	public:
		ETT_KickDeltaCostExamDistance(const ETT_Input & in, ETT_ExamDistance& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_KickExam>(in,cc,"ETT_KickDeltaCostRoomExclusive"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_KickExam& kick) const;
	public:
		bool feasibleOnly=false;
};

class ETT_KempeDeltaCostExamDistance : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_KempeExam> {
	public:
		ETT_KempeDeltaCostExamDistance(const ETT_Input & in, ETT_ExamDistance& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_KempeExam>(in,cc,"ETT_KempeDeltaCostRoomExclusive"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_KempeExam& kc) const;
	public:
		bool feasibleOnly=false;
};

class ETT_ComponentDeltaCostExamDistance : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_ComponentExam> {
	public:
		ETT_ComponentDeltaCostExamDistance(const ETT_Input & in, ETT_ExamDistance& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_ComponentExam>(in,cc,"ETT_ComponentDeltaCostRoomExclusive"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_ComponentExam& c) const;
	public:
		bool feasibleOnly=false;
};

class ETT_ShakeDeltaCostExamDistance : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_ShakeExam> {
	public:
		ETT_ShakeDeltaCostExamDistance(const ETT_Input & in, ETT_ExamDistance& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_ShakeExam>(in,cc,"ETT_ShakeDeltaCostRoomExclusive"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_ShakeExam& sh) const;
	public:
		bool feasibleOnly=false;
};

// Soft
// All exams in the same period and room should have the same length
// One penalty for each additional type of duration
class ETT_MixedDurations : public EasyLocal::Core::CostComponent<ETT_Input,ETT_State> {
	public:
		ETT_MixedDurations(const ETT_Input & in, int w, bool hard) : CostComponent<ETT_Input,ETT_State>(in,w,hard,"MixedDurations"){}
		int ComputeCost(const ETT_State& st) const;
		void PrintViolations(const ETT_State& st, std::ostream& os = std::cout) const;
};

class ETT_MoveDeltaCostMixedDurations : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_MoveExam> {
	public:
		ETT_MoveDeltaCostMixedDurations(const ETT_Input & in, ETT_MixedDurations& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_MoveExam>(in,cc,"ETT_MoveDeltaCostMixedDurations"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_MoveExam& mv) const;
	public:
		bool feasibleOnly=false;
};

class ETT_SwapDeltaCostMixedDurations : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_SwapExam> {
	public:
		ETT_SwapDeltaCostMixedDurations(const ETT_Input & in, ETT_MixedDurations& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_SwapExam>(in,cc,"ETT_SwapDeltaCostMixedDurations"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_SwapExam& mv) const;
	public:
		bool feasibleOnly=false;
};

class ETT_KickDeltaCostMixedDurations : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_KickExam> {
	public:
		ETT_KickDeltaCostMixedDurations(const ETT_Input & in, ETT_MixedDurations& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_KickExam>(in,cc,"ETT_KickDeltaCostMixedDurations"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_KickExam& kick) const;
	public:
		bool feasibleOnly=false;
};

class ETT_KempeDeltaCostMixedDurations : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_KempeExam> {
	public:
		ETT_KempeDeltaCostMixedDurations(const ETT_Input & in, ETT_MixedDurations& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_KempeExam>(in,cc,"ETT_KempeDeltaCostMixedDurations"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_KempeExam& kempe) const;
	public:
		bool feasibleOnly=false;
};

class ETT_ComponentDeltaCostMixedDurations : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_ComponentExam> {
	public:
		ETT_ComponentDeltaCostMixedDurations(const ETT_Input & in, ETT_MixedDurations& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_ComponentExam>(in,cc,"ETT_ComponentDeltaCostMixedDurations"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_ComponentExam& c) const;
	public:
		bool feasibleOnly=false;
};

class ETT_BeamDeltaCostMixedDurations : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_BeamExam> {
	public:
		ETT_BeamDeltaCostMixedDurations(const ETT_Input & in, ETT_MixedDurations& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_BeamExam>(in,cc,"ETT_BeamDeltaCostMixedDurations"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_BeamExam& b) const;
	public:
		bool feasibleOnly=false;
};

// Soft
// Frontload exams should not be scheduled in one of the frontLoadCost[1] latest periods
// A penalty of frontLoadCost[2] for each frontload exam scheduled in one of the latest periods
class ETT_FrontLoad : public EasyLocal::Core::CostComponent<ETT_Input,ETT_State> {
	public:
		ETT_FrontLoad(const ETT_Input & in, int w, bool hard) : CostComponent<ETT_Input,ETT_State>(in,w,hard,"FrontLoad"){}
		int ComputeCost(const ETT_State& st) const;
		void PrintViolations(const ETT_State& st, std::ostream& os = std::cout) const;
};

class ETT_MoveDeltaCostFrontLoad : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_MoveExam> {
	public:
		ETT_MoveDeltaCostFrontLoad(const ETT_Input & in, ETT_FrontLoad& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_MoveExam>(in,cc,"ETT_MoveDeltaCostFrontLoad"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_MoveExam& mv) const;
	public:
		bool feasibleOnly=false;
};

class ETT_SwapDeltaCostFrontLoad : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_SwapExam> {
	public:
		ETT_SwapDeltaCostFrontLoad(const ETT_Input & in, ETT_FrontLoad& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_SwapExam>(in,cc,"ETT_SwapDeltaCostFrontLoad"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_SwapExam& mv) const;
	public:
		bool feasibleOnly=false;
};

class ETT_KickDeltaCostFrontLoad : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_KickExam> {
	public:
		ETT_KickDeltaCostFrontLoad(const ETT_Input & in, ETT_FrontLoad& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_KickExam>(in,cc,"ETT_KickDeltaCostFrontLoad"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_KickExam& kick) const;
	public:
		bool feasibleOnly=false;
};

class ETT_KempeDeltaCostFrontLoad : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_KempeExam> {
	public:
		ETT_KempeDeltaCostFrontLoad(const ETT_Input & in, ETT_FrontLoad& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_KempeExam>(in,cc,"ETT_KempeDeltaCostFrontLoad"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_KempeExam& kempe) const;
	public:
		bool feasibleOnly=false;
};

class ETT_ComponentDeltaCostFrontLoad : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_ComponentExam> {
	public:
		ETT_ComponentDeltaCostFrontLoad(const ETT_Input & in, ETT_FrontLoad& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_ComponentExam>(in,cc,"ETT_ComponentDeltaCostFrontLoad"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_ComponentExam& component) const;
	public:
		bool feasibleOnly=false;
};

class ETT_ShakeDeltaCostFrontLoad : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_ShakeExam> {
	public:
		ETT_ShakeDeltaCostFrontLoad(const ETT_Input & in, ETT_FrontLoad& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_ShakeExam>(in,cc,"ETT_ShakeDeltaCostFrontLoad"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_ShakeExam& sh) const;
	public:
		bool feasibleOnly=false;
};

// Soft
// Cost for using period and/or room
class ETT_RoomPeriodPenalty : public EasyLocal::Core::CostComponent<ETT_Input,ETT_State> {
	public:
		ETT_RoomPeriodPenalty(const ETT_Input & in, int w, bool hard) : CostComponent<ETT_Input,ETT_State>(in,w,hard,"RoomPeriodPenalty"){}
		int ComputeCost(const ETT_State& st) const;
		void PrintViolations(const ETT_State& st, std::ostream& os = std::cout) const;
};

class ETT_MoveDeltaCostRoomPeriodPenalty : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_MoveExam> {
	public:
		ETT_MoveDeltaCostRoomPeriodPenalty(const ETT_Input & in, ETT_RoomPeriodPenalty& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_MoveExam>(in,cc,"ETT_MoveDeltaCostRoomPeriodPenalty"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_MoveExam& mv) const;
	public:
		bool feasibleOnly=false;
};

// There is no SwapDeltaCost component: same number of exams in every period/room

class ETT_KickDeltaCostRoomPeriodPenalty : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_KickExam> {
	public:
		ETT_KickDeltaCostRoomPeriodPenalty(const ETT_Input & in, ETT_RoomPeriodPenalty& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_KickExam>(in,cc,"ETT_KickDeltaCostRoomPeriodPenalty"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_KickExam& kick) const;
	public:
		bool feasibleOnly=false;
};

class ETT_KempeDeltaCostRoomPeriodPenalty : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_KempeExam> {
	public:
		ETT_KempeDeltaCostRoomPeriodPenalty(const ETT_Input & in, ETT_RoomPeriodPenalty& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_KempeExam>(in,cc,"ETT_KempeDeltaCostRoomPeriodPenalty"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_KempeExam& kempe) const;
	public:
		bool feasibleOnly=false;
};

class ETT_ComponentDeltaCostRoomPeriodPenalty : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_ComponentExam> {
	public:
		ETT_ComponentDeltaCostRoomPeriodPenalty(const ETT_Input & in, ETT_RoomPeriodPenalty& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_ComponentExam>(in,cc,"ETT_ComponentDeltaCostRoomPeriodPenalty"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_ComponentExam& kempe) const;
	public:
		bool feasibleOnly=false;
};

class ETT_ShakeDeltaCostRoomPeriodPenalty : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_ShakeExam> {
	public:
		ETT_ShakeDeltaCostRoomPeriodPenalty(const ETT_Input & in, ETT_RoomPeriodPenalty& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_ShakeExam>(in,cc,"ETT_ShakeDeltaCostRoomPeriodPenalty"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_ShakeExam& mv) const;
	public:
		bool feasibleOnly=false;
};

class ETT_BeamDeltaCostRoomPeriodPenalty : public EasyLocal::Core::DeltaCostComponent<ETT_Input,ETT_State,ETT_BeamExam> {
	public:
		ETT_BeamDeltaCostRoomPeriodPenalty(const ETT_Input & in, ETT_RoomPeriodPenalty& cc, bool feasibleOnly) : DeltaCostComponent<ETT_Input,ETT_State,ETT_BeamExam>(in,cc,"ETT_BeamDeltaCostRoomPeriodPenalty"), feasibleOnly(feasibleOnly) {}
		int ComputeDeltaCost(const ETT_State& st, const ETT_BeamExam& beam) const;
	public:
		bool feasibleOnly=false;
};
#endif /* ETT_COSTCOMPONENT_H */
