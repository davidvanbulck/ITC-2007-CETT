#ifndef ETT_NEIGHBORHOODEXPLORER_H
#define ETT_NEIGHBORHOODEXPLORER_H

#include "../Basics/ETT_State.h"
#include "../Basics/ETT_Move.h"
#include "ETT_StateManager.h"
#include <random>

/************************************
*  ETT_Move neighborhood explorer  *
************************************/
class ETT_MoveNeighborhoodExplorer : public EasyLocal::Core::NeighborhoodExplorer<ETT_Input,ETT_State,ETT_MoveExam> {
	public:
		ETT_MoveNeighborhoodExplorer(const ETT_Input & pin, EasyLocal::Core::StateManager<ETT_Input,ETT_State>& psm, double bias) : NeighborhoodExplorer<ETT_Input,ETT_State,ETT_MoveExam>(pin, psm, "ETT_MoveExamNeighborhoodExplorer") {

			roomRate = bias;
			for (int i = 0; i < in.getNoExams(); ++i) {
				// Instance art037: there is an exam with only one feasible period and one feasible room
				// Such an exam gets a weight of 0, since no feasible moves involving i exist
				weights.push_back(in.getNoValidPeriods(i)*in.getNoValidRooms(i) - 1);	
				distribution = std::discrete_distribution<int>(weights.begin(), weights.end());
			}
		} 
		void RandomMove(const ETT_State&, ETT_MoveExam&) const;          
		bool FeasibleMove(const ETT_State&, const ETT_MoveExam&) const;  
		void MakeMove(ETT_State&, const ETT_MoveExam&) const;             
		void FirstMove(const ETT_State&, ETT_MoveExam&) const;  
		bool NextMove(const ETT_State&, ETT_MoveExam&) const;   
	protected:
		bool AnyNextMove(const ETT_State&, ETT_MoveExam&) const;
		//void firstNewRoom(const ETT_State& st, ETT_MoveExam& mv) const; // Avoid null moves: set newRoom and newRoomIndex
		//bool nextNewRoom(const ETT_State& st, ETT_MoveExam& mv) const; 	// Avoid null moves: set newRoom an newRoomIndex
		//void firstNewPeriod(const ETT_State& st, ETT_MoveExam& mv) const; // Avoid null moves: set to and toIndex
		//bool nextNewPeriod(const ETT_State& st, ETT_MoveExam& mv) const; // Avoid null moves: set to and toIndex
		
		// Biased free selection: each exam has probability weights[i] to be selected
		// weights[i] is equal to the number of feasible moves involving exam i
		std::vector<int> weights;
		std::discrete_distribution<int> distribution;
		double roomRate; // = 0.5;
};

/************************************
*  ETT_Swap neighborhood explorer  *
************************************/
class ETT_SwapNeighborhoodExplorer : public EasyLocal::Core::NeighborhoodExplorer<ETT_Input,ETT_State,ETT_SwapExam> {
	public:
		ETT_SwapNeighborhoodExplorer(const ETT_Input & pin, EasyLocal::Core::StateManager<ETT_Input,ETT_State>& psm) : NeighborhoodExplorer<ETT_Input,ETT_State,ETT_SwapExam>(pin, psm, "ETT_SwapExamNeighborhoodExplorer") {} 
		void RandomMove(const ETT_State&, ETT_SwapExam&) const;          
		bool FeasibleMove(const ETT_State&, const ETT_SwapExam&) const;  
		void MakeMove(ETT_State&, const ETT_SwapExam&) const;             
		void FirstMove(const ETT_State&, ETT_SwapExam&) const;  
		bool NextMove(const ETT_State&, ETT_SwapExam&) const;   
	protected:
		bool nextExam1(const ETT_State& st, ETT_SwapExam& swp) const;
		bool nextExam2(const ETT_State& st, ETT_SwapExam& swp) const;
};

/************************************
*  ETT_Kick neighborhood explorer  *
************************************/
class ETT_KickNeighborhoodExplorer : public EasyLocal::Core::NeighborhoodExplorer<ETT_Input,ETT_State,ETT_KickExam> {
	public:
		ETT_KickNeighborhoodExplorer(const ETT_Input & pin, EasyLocal::Core::StateManager<ETT_Input,ETT_State>& psm, double bias1, double bias2) : NeighborhoodExplorer<ETT_Input,ETT_State,ETT_KickExam>(pin, psm, "ETT_KickExamNeighborhoodExplorer") {
			swapRate = bias1;
			roomRate = bias2;
			for (unsigned i = 0; i < in.getNoExams(); ++i) {
				// Instance art037: there is an exam with only one feasible period and one feasible room
				// Such an exam gets a weight of 0, since no feasible moves involving i exist
				weights.push_back(in.getNoValidPeriods(i)*in.getNoValidRooms(i) - 1);	
				distribution = std::discrete_distribution<int>(weights.begin(), weights.end());
			}
		
			// Initialize random exam ids
			randomExamIds.resize(in.getNoExams());
			std::iota(randomExamIds.begin(), randomExamIds.end(), 0);
		} 
		void RandomMove(const ETT_State&, ETT_KickExam&) const;          
		void AnyRandomMove(const ETT_State&, ETT_KickExam&) const;          
		bool FeasibleMove(const ETT_State&, const ETT_KickExam&) const;  
		void MakeMove(ETT_State&, const ETT_KickExam&) const;             
		void FirstMove(const ETT_State&, ETT_KickExam&) const;  
		bool NextMove(const ETT_State&, ETT_KickExam&) const;   
		bool AnyNextMove(const ETT_State&, ETT_KickExam&) const;   
	
	private:
		double swapRate; // = 0.5;
		double roomRate;
		bool nextNewRoom(const ETT_State& st, ETT_KickExam& kick) const; 	
		bool firstNewRoom(const ETT_State& st, ETT_KickExam& kick) const; 	
		bool nextNewPeriod(const ETT_State& st, ETT_KickExam& kick) const; 	
		bool firstNewPeriod(const ETT_State& st, ETT_KickExam& kick) const; 	
		bool nextExam2(const ETT_State& st, ETT_KickExam& kick) const;
		bool nextExam1(const ETT_State& st, ETT_KickExam& kick) const;

		// Biased free selection: each exam has probability weights[i] to be selected
		// weights[i] is equal to the number of feasible moves involving exam i
		std::vector<int> weights;
		std::discrete_distribution<int> distribution;
		std::vector<int> randomExamIds;
};

/*************************************
*  ETT_Kempe neighborhood explorer  *
*************************************/
class ETT_KempeNeighborhoodExplorer : public EasyLocal::Core::NeighborhoodExplorer<ETT_Input,ETT_State,ETT_KempeExam> {
	public:
		ETT_KempeNeighborhoodExplorer(const ETT_Input & pin, EasyLocal::Core::StateManager<ETT_Input,ETT_State>& psm, const bool assignNewRoomFlag, const int kempeRoomMode, const int kempe_beamwidth) : NeighborhoodExplorer<ETT_Input,ETT_State,ETT_KempeExam>(pin, psm, "ETT_KempeExamNeighborhoodExplorer"), assignNewRoom(assignNewRoomFlag), roomMode(kempeRoomMode), beamwidth(kempe_beamwidth) {
			for (int i = 0; i < in.getNoExams(); ++i) {
				// Instance art037: there is an exam with only one feasible period and one feasible room
				// Such an exam gets a weight of 0, since no feasible moves involving i exist
				weights.push_back(in.getNoValidPeriods(i)*in.getNoValidRooms(i) - 1);	
				distribution = std::discrete_distribution<int>(weights.begin(), weights.end());
			}
		} 
		void AnyRandomMove(const ETT_State&, ETT_KempeExam&) const;          
		void RandomMove(const ETT_State&, ETT_KempeExam&) const;          
		bool FeasibleMove(const ETT_State&, const ETT_KempeExam&) const;  
		void MakeMove(ETT_State&, const ETT_KempeExam&) const;             
		void FirstMove(const ETT_State&, ETT_KempeExam&) const;  
		bool NextMove(const ETT_State&, ETT_KempeExam&) const;   
	
	protected:
		bool AnyNextMove(const ETT_State&, ETT_KempeExam&) const;
		// Biased free selection: each exam has probability weights[i] to be selected
		// weights[i] is equal to the number of feasible moves involving exam i
		std::vector<int> weights;
		std::discrete_distribution<int> distribution;

		void CreateChain(const ETT_State& st, ETT_KempeExam& kc) const;
		void TightestFit(const ETT_State& st, ETT_KempeExam& kc) const;
		void BeamFit(const ETT_State& st, ETT_KempeExam& kc) const;
		void ComputeNewInsertions(const ETT_State& st, const std::unordered_set<int>& current, const std::unordered_set<int>& to_be_checked, std::unordered_set<int>& new_elements, int p, int s) const;

		const bool assignNewRoom; // True if we also need to assign a new room
		const int roomMode; // 0 iff tightest fit, 1 if beam
		const int beamwidth;
};

/************************************
*  ETT_Shake neighborhood explorer  *
************************************/
class ETT_ShakeNeighborhoodExplorer : public EasyLocal::Core::NeighborhoodExplorer<ETT_Input,ETT_State,ETT_ShakeExam> {
	public:
		ETT_ShakeNeighborhoodExplorer(const ETT_Input & pin, EasyLocal::Core::StateManager<ETT_Input,ETT_State>& psm) : NeighborhoodExplorer<ETT_Input,ETT_State,ETT_ShakeExam>(pin, psm, "ETT_ShakeExamNeighborhoodExplorer") {} 
		void RandomMove(const ETT_State&, ETT_ShakeExam&) const;          
		bool FeasibleMove(const ETT_State&, const ETT_ShakeExam&) const;  
		void MakeMove(ETT_State&, const ETT_ShakeExam&) const;             
		void FirstMove(const ETT_State&, ETT_ShakeExam&) const;  
		bool NextMove(const ETT_State&, ETT_ShakeExam&) const;   
	protected:
		bool nextPeriod1(const ETT_State& st, ETT_ShakeExam& sh) const;
		bool nextPeriod2(const ETT_State& st, ETT_ShakeExam& sh) const;
};

/************************************
*  ETT_Component neighborhood explorer  *
************************************/
class ETT_ComponentNeighborhoodExplorer : public EasyLocal::Core::NeighborhoodExplorer<ETT_Input,ETT_State,ETT_ComponentExam> {
	public:
		ETT_ComponentNeighborhoodExplorer(const ETT_Input & pin, EasyLocal::Core::StateManager<ETT_Input,ETT_State>& psm) : NeighborhoodExplorer<ETT_Input,ETT_State,ETT_ComponentExam>(pin, psm, "ETT_ComponentExamNeighborhoodExplorer") {} 
		void RandomMove(const ETT_State&, ETT_ComponentExam&) const;          
		bool FeasibleMove(const ETT_State&, const ETT_ComponentExam&) const;  
		void MakeMove(ETT_State&, const ETT_ComponentExam&) const;             
		void FirstMove(const ETT_State&, ETT_ComponentExam&) const;  
		bool NextMove(const ETT_State&, ETT_ComponentExam&) const;   
		void TightestFit(const ETT_State& st, ETT_ComponentExam& c) const;

	protected:
		bool nextK(const ETT_State& st, ETT_ComponentExam& c) const;
		bool nextSlot(const ETT_State& st, ETT_ComponentExam& c) const;
		bool nextComponent(const ETT_State& st, ETT_ComponentExam& c) const;
		bool computePeriods(const ETT_State& st, ETT_ComponentExam& c) const;
};



/************************************
*  ETT_Beam neighborhood explorer  *
************************************/

class ETT_BeamNeighborhoodExplorer : public EasyLocal::Core::NeighborhoodExplorer<ETT_Input,ETT_State,ETT_BeamExam> {
	public:
		ETT_BeamNeighborhoodExplorer(const ETT_Input & pin, EasyLocal::Core::StateManager<ETT_Input,ETT_State>& psm, const int beamwidth, const int beamdepth);
		void RandomMove(const ETT_State&, ETT_BeamExam&) const;          
		void MakeMove(ETT_State&, const ETT_BeamExam&) const;             
		void FirstMove(const ETT_State&, ETT_BeamExam&) const;  
		bool NextMove(const ETT_State&, ETT_BeamExam&) const;   
		bool FeasibleMove(const ETT_State&, const ETT_BeamExam&) const;
		bool FirstBeamPeriod(const ETT_State&, ETT_BeamExam&, const int periodStart) const;

	private:
		int beamDepth;
		int beamwidth;
		void beamSearch(const ETT_State& st, ETT_BeamExam& b) const;
		std::vector<int> randomExamIds;
};

#endif /* ETT_NEIGHBORHOODEXPLORER_H */
