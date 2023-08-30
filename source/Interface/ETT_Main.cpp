#include <fstream>
#include "../Data/ETT_Input.h"
#include "../Helpers/ETT_CostComponent.h"
#include "../Helpers/ETT_StateManager.h"
#include "../Helpers/ETT_OutputManager.h"
#include "../Helpers/ETT_NeighborhoodExplorer.h"
#include <algorithm>
#include <iterator>

int main(int argc, const char* argv[]) {
	EasyLocal::Debug::ParameterBox main_parameters("main", "Main Program options");

	// The set of arguments added are the following:

	EasyLocal::Debug::Parameter<std::string> instance("instance", "Input instance", main_parameters); 
	EasyLocal::Debug::Parameter<long> seed("seed", "Random seed", main_parameters);
	EasyLocal::Debug::Parameter<std::string> method("method", "Solution method (empty for tester)", main_parameters);   
	EasyLocal::Debug::Parameter<std::string> init_state("init_state", "Initial state (to be read from file)", main_parameters);
	EasyLocal::Debug::Parameter<std::string> init_method("init_method", "random, SD, OBSI, SDFeas, or OBSIFeas", main_parameters);
	EasyLocal::Debug::Parameter<std::string> output_file("output_file", "Write the output to a file (filename required)", main_parameters);
	EasyLocal::Debug::Parameter<double> swap_rate("swap_rate", "Swap rate", main_parameters);
	EasyLocal::Debug::Parameter<double> swap_bias("swap_bias", "Swap bias inside Kick neighborhood", main_parameters);
	EasyLocal::Debug::Parameter<double> room_bias_move("room_bias_move", "Room bias", main_parameters);
	EasyLocal::Debug::Parameter<double> room_bias_kick("room_bias_kick", "Room bias", main_parameters);
	EasyLocal::Debug::Parameter<double> kick_rate("kick_rate", "Kick rate", main_parameters);
	EasyLocal::Debug::Parameter<double> kempe_rate("kempe_rate", "Kempe rate", main_parameters);
	EasyLocal::Debug::Parameter<int> kempe_beamwidth("kempe_beamwidth", "Kempe beam width", main_parameters);
	EasyLocal::Debug::Parameter<double> shake_rate("shake_rate", "Shake rate", main_parameters);
	EasyLocal::Debug::Parameter<double> component_rate("component_rate", "Component rate", main_parameters); 
	EasyLocal::Debug::Parameter<double> beam_rate("beam_rate", "Beam rate", main_parameters);
	EasyLocal::Debug::Parameter<double> beamwidth("beamwidth", "Beam width inside Beam neighborhood", main_parameters);
	EasyLocal::Debug::Parameter<double> beamdepth("beamdepth", "Beam depth inside Beam neighborhood", main_parameters);
	EasyLocal::Debug::Parameter<int> hard_weight("hard_weight", "Weight of the hard components", main_parameters);
	EasyLocal::Debug::Parameter<int> kempeAssignNewRoom("kempeAssignNewRoom", "True iff kempe also assigns a new room to e", main_parameters);
	EasyLocal::Debug::Parameter<int> kempeRoomMode("kempeRoomMode", "0 if tightest fit, 1 if room mode", main_parameters);
	EasyLocal::Debug::Parameter<int> feasibleOnly("feasibleOnly", "3 iff we look for feasible timetables only, 2 iff we allow the move and swap operator to operate in the infeasible space, 1 if all operators can move in the infeasible space", main_parameters);
	EasyLocal::Debug::Parameter<unsigned long> max_evaluations("max_evaluations", "max no. evaluations", main_parameters);

	room_bias_move = 0.5;
	room_bias_kick = 0;
	swap_rate = 0.5;
	swap_bias = 0;
	kick_rate = 0.5;
	kempe_rate = 0.5;
	shake_rate = 0.5;
	beam_rate = 0.5;
	component_rate = 0.5;
	beamwidth = 5;
	beamdepth = 5;
	kempe_beamwidth = 1;
	kempeAssignNewRoom = 1;
	kempeRoomMode = 0;
	feasibleOnly=1;
	init_method="random";

	// 3rd parameter: false = do not check unregistered parameters
	// 4th parameter: true = silent
	EasyLocal::Debug::CommandLineParameters::Parse(argc, argv, false, true);  
	std::cout << "Parameters: " << std::endl;
	std::copy( argv+1, argv+argc, std::ostream_iterator<const char*>( std::cout, "\n" ) ) ;

	if(feasibleOnly <= 0 || feasibleOnly > 3){
		std::cout << "Error: --feasibleOnly should take value 1, 2, or 3" << std::endl;
		return 1;
	}

	if(kempeRoomMode != 0 && kempeRoomMode != 1){
		std::cout << "Error: --kempeRoomMode should take value 0 or 1" << std::endl;
		return 1;
	}

	if (!instance.IsSet())
	{
		std::cout << "Error: --main::instance filename option must always be set" << std::endl;
		return 1;
	}
	ETT_Input in(instance, {1,1,1,1});

	// Hard weight constraint penalty computed by Battistuta et al.
	// TODO TODO
	//const int default_hard_weight = 1000;
	if(!hard_weight.IsSet()){
		const int default_hard_weight = std::max(16.731+102.3*in.CD-0.4833*in.getNoPeriods()-0.1774*in.SxE-1.239*in.ExR-0.00076*in.getNoStudents()+0.1159*in.PHC+0.6666*in.FLP+0.7808*in.RHC+32.46*in.SCap+0.101*in.PC,10.0);
		hard_weight = default_hard_weight; 		
		std::cout << "Hard weight: " << hard_weight << std::endl;
	}


	if (seed.IsSet())
		EasyLocal::Debug::Random::SetSeed(seed);

	// If there are no components, disable component operator
	std::cout << "Components: " << in.getNoComponents() << std::endl;
	if(in.getNoComponents() == 0){
		component_rate = 0;
#ifndef NDEBUG
		std::cout << "Zero components. Disable component operator."<< std::endl;
#endif
	}

	// Overrule the room rate if there is only one room...
	if(in.getNoRooms() == 1){
#ifndef NDEBUG
		std::cout << "Only one room: set room rate to 0, and disable beam operator" << std::endl;
#endif
		room_bias_move = 0;
		room_bias_kick = 0;
		beam_rate = 0;
	}

	// cost components: second parameter is the cost, third is the type (true -> hard, false -> soft)
	ETT_ExamConflicts cc1(in, hard_weight, true);
#ifndef UNCAPACITATED
	ETT_RoomOccupancy cc2(in, hard_weight, true);
	ETT_ExamSequence cc3(in, hard_weight, true);
	ETT_RoomExclusive cc4(in, hard_weight, true);
#endif
	ETT_ExamDistance cc5(in, 1, false);
#ifndef UNCAPACITATED
	ETT_MixedDurations cc6(in, 1, false);
	ETT_FrontLoad cc7(in, 1, false);
	ETT_RoomPeriodPenalty cc8(in, 1, false);
#endif

	ETT_MoveDeltaCostExamConflicts dccM1(in, cc1, (feasibleOnly == 3 ? true: false));
#ifndef UNCAPACITATED
	ETT_MoveDeltaCostRoomOccupancy dccM2(in, cc2, (feasibleOnly == 3 ? true: false));
	ETT_MoveDeltaCostExamSequence dccM3(in, cc3, (feasibleOnly == 3 ? true: false));
	ETT_MoveDeltaCostRoomExclusive dccM4(in, cc4,(feasibleOnly == 3 ? true: false));
#endif
	ETT_MoveDeltaCostExamDistance dccM5(in, cc5, (feasibleOnly == 3 ? true: false));
#ifndef UNCAPACITATED
	ETT_MoveDeltaCostMixedDurations dccM6(in, cc6, (feasibleOnly == 3 ? true: false));
	ETT_MoveDeltaCostFrontLoad dccM7(in, cc7, (feasibleOnly == 3 ? true: false));
	ETT_MoveDeltaCostRoomPeriodPenalty dccM8(in, cc8, (feasibleOnly == 3 ? true: false));
#endif

	ETT_SwapDeltaCostExamConflicts dccS1(in, cc1, (feasibleOnly == 3 ? true: false));
#ifndef UNCAPACITATED
	ETT_SwapDeltaCostRoomOccupancy dccS2(in, cc2, (feasibleOnly == 3 ? true: false));
	ETT_SwapDeltaCostExamSequence dccS3(in, cc3, (feasibleOnly == 3 ? true: false));
	ETT_SwapDeltaCostRoomExclusive dccS4(in, cc4, (feasibleOnly == 3 ? true: false));
#endif
	ETT_SwapDeltaCostExamDistance dccS5(in, cc5, (feasibleOnly == 3 ? true: false));
#ifndef UNCAPACITATED
	ETT_SwapDeltaCostMixedDurations dccS6(in, cc6, (feasibleOnly == 3 ? true: false));
	ETT_SwapDeltaCostFrontLoad dccS7(in, cc7, (feasibleOnly == 3 ? true: false));
#endif

	ETT_KickDeltaCostExamConflicts dccK1(in, cc1, (feasibleOnly == 3 ? true: false));
#ifndef UNCAPACITATED
	ETT_KickDeltaCostRoomOccupancy dccK2(in, cc2, (feasibleOnly == 3 ? true: false));
	ETT_KickDeltaCostExamSequence dccK3(in, cc3, (feasibleOnly == 3 ? true: false));
	ETT_KickDeltaCostRoomExclusive dccK4(in, cc4, (feasibleOnly == 3 ? true: false));
#endif
	ETT_KickDeltaCostExamDistance dccK5(in, cc5, (feasibleOnly == 3 ? true: false));
#ifndef UNCAPACITATED
	ETT_KickDeltaCostMixedDurations dccK6(in, cc6, (feasibleOnly == 3 ? true: false));
	ETT_KickDeltaCostFrontLoad dccK7(in, cc7, (feasibleOnly == 3 ? true: false));
	ETT_KickDeltaCostRoomPeriodPenalty dccK8(in, cc8, (feasibleOnly == 3 ? true: false));
#endif

	ETT_KempeDeltaCostExamConflicts dccKE1(in, cc1, (feasibleOnly >= 2 ? true: false));
#ifndef UNCAPACITATED
	ETT_KempeDeltaCostRoomOccupancy dccKE2(in, cc2, (feasibleOnly >= 2 ? true: false));
	ETT_KempeDeltaCostExamSequence dccKE3(in, cc3, (feasibleOnly >= 2 ? true: false));
	ETT_KempeDeltaCostRoomExclusive dccKE4(in, cc4, (feasibleOnly >= 2 ? true: false));
#endif
	ETT_KempeDeltaCostExamDistance dccKE5(in, cc5, (feasibleOnly >= 2 ? true: false));
#ifndef UNCAPACITATED
	ETT_KempeDeltaCostMixedDurations dccKE6(in, cc6, (feasibleOnly >= 2 ? true: false));
	ETT_KempeDeltaCostFrontLoad dccKE7(in, cc7, (feasibleOnly >= 2 ? true: false));
	ETT_KempeDeltaCostRoomPeriodPenalty dccKE8(in, cc8, (feasibleOnly >= 2 ? true: false));
#endif

#ifndef UNCAPACITATED
	ETT_ComponentDeltaCostRoomOccupancy dccC2(in, cc2, (feasibleOnly >= 2 ? true: false));
	ETT_ComponentDeltaCostExamSequence dccC3(in, cc3, (feasibleOnly >= 2 ? true: false));
	ETT_ComponentDeltaCostRoomExclusive dccC4(in, cc4, (feasibleOnly >= 2 ? true: false));
#endif
	ETT_ComponentDeltaCostExamDistance dccC5(in, cc5, (feasibleOnly >= 2 ? true: false));
#ifndef UNCAPACITATED
	ETT_ComponentDeltaCostMixedDurations dccC6(in, cc6, (feasibleOnly >= 2 ? true: false));
	ETT_ComponentDeltaCostFrontLoad dccC7(in, cc7, (feasibleOnly >= 2 ? true: false));
	ETT_ComponentDeltaCostRoomPeriodPenalty dccC8(in, cc8, (feasibleOnly >= 2 ? true: false));
#endif

#ifndef UNCAPACITATED
	ETT_ShakeDeltaCostExamSequence dccSH3(in, cc3, (feasibleOnly >= 2 ? true: false));
#endif

	ETT_ShakeDeltaCostExamDistance dccSH5(in, cc5, (feasibleOnly >= 2 ? true: false));
#ifndef UNCAPACITATED
	ETT_ShakeDeltaCostFrontLoad dccSH7(in, cc7, (feasibleOnly >= 2 ? true: false));
	ETT_ShakeDeltaCostRoomPeriodPenalty dccSH8(in, cc8, (feasibleOnly >= 2 ? true: false));
#endif

#ifndef UNCAPACITATED
	ETT_BeamDeltaCostRoomOccupancy dccB2(in, cc2, (feasibleOnly >= 2 ? true: false));
	ETT_BeamDeltaCostRoomExclusive dccB4(in, cc4, (feasibleOnly >= 2 ? true: false));
	ETT_BeamDeltaCostMixedDurations dccB6(in, cc6, (feasibleOnly >= 2 ? true: false));
	ETT_BeamDeltaCostRoomPeriodPenalty dccB8(in, cc8, (feasibleOnly >= 2 ? true: false));
#endif

	// helpers
	ETT_StateManager ETT_sm(in);
	ETT_MoveNeighborhoodExplorer ETT_nheMove(in, ETT_sm,room_bias_move);
	ETT_SwapNeighborhoodExplorer ETT_nheSwap(in, ETT_sm);
	ETT_KickNeighborhoodExplorer ETT_nheKick(in, ETT_sm,swap_bias,room_bias_kick);
	ETT_KempeNeighborhoodExplorer ETT_nheKempe(in, ETT_sm, kempeAssignNewRoom, kempeRoomMode, kempe_beamwidth);
	ETT_ShakeNeighborhoodExplorer ETT_nheShake(in, ETT_sm);
	ETT_ComponentNeighborhoodExplorer ETT_nheComponent(in, ETT_sm);

#ifndef UNCAPACITATED
	ETT_BeamNeighborhoodExplorer ETT_nheBeam(in, ETT_sm,beamwidth, beamdepth);
#endif

	ETT_OutputManager ETT_om(in);

	std::vector<std::string> components_order;
	// All cost components must be added to the state manager
	ETT_sm.AddCostComponent(cc1);
	components_order.push_back(cc1.name);
#ifndef UNCAPACITATED
	ETT_sm.AddCostComponent(cc2);
	components_order.push_back(cc2.name);
	ETT_sm.AddCostComponent(cc3);
	components_order.push_back(cc3.name);
	ETT_sm.AddCostComponent(cc4);
	components_order.push_back(cc4.name);
#endif
	ETT_sm.AddCostComponent(cc5);
	components_order.push_back(cc5.name);
#ifndef UNCAPACITATED
	if(in.getNonMixedDurationsCost()>0){
		ETT_sm.AddCostComponent(cc6);
		components_order.push_back(cc6.name);
	} 
	if(in.getFrontLoadCost()>0){
		ETT_sm.AddCostComponent(cc7);
		components_order.push_back(cc7.name);
	} 
	ETT_sm.AddCostComponent(cc8);
	components_order.push_back(cc8.name);
#endif

	// All delta cost components must be added to the neighborhood explorer
	ETT_nheMove.AddDeltaCostComponent(dccM1);
#ifndef UNCAPACITATED
	ETT_nheMove.AddDeltaCostComponent(dccM3);
	ETT_nheMove.AddDeltaCostComponent(dccM2);
	ETT_nheMove.AddDeltaCostComponent(dccM4);
#endif
	ETT_nheMove.AddDeltaCostComponent(dccM5);
#ifndef UNCAPACITATED
	if(in.getNonMixedDurationsCost()>0){
		ETT_nheMove.AddDeltaCostComponent(dccM6);
	}
	if(in.getFrontLoadCost()>0){
		ETT_nheMove.AddDeltaCostComponent(dccM7);
	}
	ETT_nheMove.AddDeltaCostComponent(dccM8);
#endif

	ETT_nheSwap.AddDeltaCostComponent(dccS1);
#ifndef UNCAPACITATED
	ETT_nheSwap.AddDeltaCostComponent(dccS3);
	ETT_nheSwap.AddDeltaCostComponent(dccS2);
	ETT_nheSwap.AddDeltaCostComponent(dccS4);
#endif

	ETT_nheSwap.AddDeltaCostComponent(dccS5);
#ifndef UNCAPACITATED
	if(in.getNonMixedDurationsCost()>0){
		ETT_nheSwap.AddDeltaCostComponent(dccS6);
	}
	if(in.getFrontLoadCost()>0){
		ETT_nheSwap.AddDeltaCostComponent(dccS7);
	}
#endif


#ifndef UNCAPACITATED
	ETT_nheShake.AddDeltaCostComponent(dccSH3);
#endif

#ifndef UNCAPACITATED
	ETT_nheShake.AddDeltaCostComponent(dccSH5);
	if(in.getFrontLoadCost()>0){
		ETT_nheShake.AddDeltaCostComponent(dccSH7);
	}
	ETT_nheShake.AddDeltaCostComponent(dccSH8);
#endif

	ETT_nheKick.AddDeltaCostComponent(dccK1);
#ifndef UNCAPACITATED
	ETT_nheKick.AddDeltaCostComponent(dccK3);

	ETT_nheKick.AddDeltaCostComponent(dccK2);
	ETT_nheKick.AddDeltaCostComponent(dccK4);
#endif
	ETT_nheKick.AddDeltaCostComponent(dccK5);
#ifndef UNCAPACITATED
	if(in.getNonMixedDurationsCost()>0){
		ETT_nheKick.AddDeltaCostComponent(dccK6);
	}
	if(in.getFrontLoadCost()>0){
		ETT_nheKick.AddDeltaCostComponent(dccK7);
	}	
	ETT_nheKick.AddDeltaCostComponent(dccK8);
#endif

	ETT_nheKempe.AddDeltaCostComponent(dccKE1);
#ifndef UNCAPACITATED
	ETT_nheKempe.AddDeltaCostComponent(dccKE3);

	ETT_nheKempe.AddDeltaCostComponent(dccKE2);
	ETT_nheKempe.AddDeltaCostComponent(dccKE4);
#endif
	ETT_nheKempe.AddDeltaCostComponent(dccKE5);
#ifndef UNCAPACITATED
	if(in.getNonMixedDurationsCost()>0){
		ETT_nheKempe.AddDeltaCostComponent(dccKE6);
	}
	if(in.getFrontLoadCost()>0){
		ETT_nheKempe.AddDeltaCostComponent(dccKE7);
	}
	ETT_nheKempe.AddDeltaCostComponent(dccKE8);
#endif

#ifndef UNCAPACITATED
	ETT_nheComponent.AddDeltaCostComponent(dccC3);
	ETT_nheComponent.AddDeltaCostComponent(dccC2);
	ETT_nheComponent.AddDeltaCostComponent(dccC4);
	ETT_nheComponent.AddDeltaCostComponent(dccC5);
	if(in.getNonMixedDurationsCost()>0){
		ETT_nheComponent.AddDeltaCostComponent(dccC6);
	}
	if(in.getFrontLoadCost()>0){
		ETT_nheComponent.AddDeltaCostComponent(dccC7);
	}
	ETT_nheComponent.AddDeltaCostComponent(dccC8);
#endif

#ifndef UNCAPACITATED
	ETT_nheBeam.AddDeltaCostComponent(dccB2);
	ETT_nheBeam.AddDeltaCostComponent(dccB4);
	if(in.getNonMixedDurationsCost()>0){
		ETT_nheBeam.AddDeltaCostComponent(dccB6);
	}
	ETT_nheBeam.AddDeltaCostComponent(dccB8);
#endif

	EasyLocal::Core::SetUnionNeighborhoodExplorer<ETT_Input, ETT_State, EasyLocal::Core::DefaultCostStructure<int>, decltype(ETT_nheMove), decltype(ETT_nheSwap)> 
	ETT_nheMoveSwap(in, ETT_sm, "Bimodal Move/Swap", ETT_nheMove, ETT_nheSwap, {1.0 - swap_rate, swap_rate});

	EasyLocal::Core::SetUnionNeighborhoodExplorer<ETT_Input, ETT_State, EasyLocal::Core::DefaultCostStructure<int>, decltype(ETT_nheMove), decltype(ETT_nheKick)> 
	ETT_nheMoveKick(in, ETT_sm, "Bimodal Move/Kick", ETT_nheMove, ETT_nheKick, {1.0 - kick_rate, kick_rate});

	// NOTE Move = Kempe with repair chain of length 0
	// In contrast to the kick generalization of the swap move, I do not add a "moveRate" to the kick operator, but instead keep the move as a separate neighborhood. Reason: it may be preferable to not repair a move which causes a conflict (so as to explore the infeasible space as well)
	EasyLocal::Core::SetUnionNeighborhoodExplorer<ETT_Input, ETT_State, EasyLocal::Core::DefaultCostStructure<int>, decltype(ETT_nheMove), decltype(ETT_nheKick), decltype(ETT_nheKempe)> 
	ETT_nheMoveKempe(in, ETT_sm, "Trimodal Move/Kick/Kempe", ETT_nheMove, ETT_nheKick, ETT_nheKempe, {1.0 - kick_rate - kempe_rate, kick_rate, kempe_rate});

	EasyLocal::Core::SetUnionNeighborhoodExplorer<ETT_Input, ETT_State, EasyLocal::Core::DefaultCostStructure<int>, decltype(ETT_nheMove), decltype(ETT_nheKick), decltype(ETT_nheKempe), decltype(ETT_nheShake)> 
	ETT_nheMoveShake(in, ETT_sm, "Fourmodal Move/Kick/Kempe/Shake", ETT_nheMove, ETT_nheKick, ETT_nheKempe, ETT_nheShake, {1.0 - kick_rate - kempe_rate - shake_rate, kick_rate, kempe_rate, shake_rate});

	EasyLocal::Core::SetUnionNeighborhoodExplorer<ETT_Input, ETT_State, EasyLocal::Core::DefaultCostStructure<int>, decltype(ETT_nheMove), decltype(ETT_nheKick), decltype(ETT_nheKempe), decltype(ETT_nheComponent), decltype(ETT_nheShake)> 
	ETT_nheMoveComponent(in, ETT_sm, "Fivemodal Move/Kick/Kempe/Component/Shake", ETT_nheMove, ETT_nheKick, ETT_nheKempe, ETT_nheComponent, ETT_nheShake, {1.0 - kick_rate - kempe_rate - component_rate - shake_rate, kick_rate, kempe_rate, component_rate, shake_rate});

#ifndef UNCAPACITATED
	EasyLocal::Core::SetUnionNeighborhoodExplorer<ETT_Input, ETT_State, EasyLocal::Core::DefaultCostStructure<int>, decltype(ETT_nheMove), decltype(ETT_nheSwap), decltype(ETT_nheKempe), decltype(ETT_nheShake)> 
	// Replace Kick by Swap: too many possible kicks for steepest descent
	ETT_nheMoveSwapShakeKempe(in, ETT_sm, "Multimodal Move/Swap/Shake/Kempe", ETT_nheMove, ETT_nheSwap, ETT_nheKempe, ETT_nheShake, {1.0 - kick_rate - kempe_rate - shake_rate, kick_rate, kempe_rate, shake_rate});

	EasyLocal::Core::SetUnionNeighborhoodExplorer<ETT_Input, ETT_State, EasyLocal::Core::DefaultCostStructure<int>, decltype(ETT_nheMove), decltype(ETT_nheKick), decltype(ETT_nheKempe), decltype(ETT_nheBeam), decltype(ETT_nheShake), decltype(ETT_nheComponent)> 
	ETT_nheMoveBeam(in, ETT_sm, "Sixmodal Move/Kick/Kempe/Beam/Shake/Component", ETT_nheMove, ETT_nheKick, ETT_nheKempe, ETT_nheBeam, ETT_nheShake, ETT_nheComponent, {1.0 - kick_rate - kempe_rate - beam_rate - shake_rate - component_rate, kick_rate, kempe_rate, beam_rate, shake_rate, component_rate});
#endif

	// TODO TODO TODO TODO
	// TODO TODO TODO TODO
	// TODO TODO TODO TODO
	// TODO TODO TODO TODO
	
	// If I use the following neighborhood, it should be as fast as the MoveSwap neighborhood as it is in fact the same... Nevertheless, it turns out to be much slower...
	// Why??
	//ETT_nheMoveBeam(in, ETT_sm, "Sixmodal Move/Kick/Kempe/Beam/Shake/Component", ETT_nheMove, ETT_nheSwap, ETT_nheKick, ETT_nheKempe, ETT_nheBeam, ETT_nheShake, ETT_nheComponent, {1-swap_rate, swap_rate, 0, 0, 0, 0, 0});

	// TODO TODO TODO TODO
	// TODO TODO TODO TODO
	// TODO TODO TODO TODO
	// TODO TODO TODO TODO

	// runners
	EasyLocal::Core::HillClimbing<ETT_Input, ETT_State, ETT_MoveExam> ETT_hc(in, ETT_sm, ETT_nheMove, "MHC");
	EasyLocal::Core::SteepestDescent<ETT_Input, ETT_State, ETT_MoveExam> ETT_sd(in, ETT_sm, ETT_nheMove, "MSD");
	EasyLocal::Core::SimulatedAnnealing<ETT_Input, ETT_State, ETT_MoveExam> ETT_sa(in, ETT_sm, ETT_nheMove, "MSA");

	EasyLocal::Core::SimulatedAnnealingEvaluationBased<ETT_Input, ETT_State, decltype(ETT_nheMoveSwap)::MoveType> ETT_bsa(in, ETT_sm, ETT_nheMoveSwap, "BSA");
	EasyLocal::Core::LateAcceptanceHillClimbing<ETT_Input, ETT_State, decltype(ETT_nheMoveSwap)::MoveType> ETT_bla(in, ETT_sm, ETT_nheMoveSwap, "BLA");

	EasyLocal::Core::SimulatedAnnealingEvaluationBased<ETT_Input, ETT_State, decltype(ETT_nheMoveKick)::MoveType> ETT_bsa_kick(in, ETT_sm, ETT_nheMoveKick, "BSA2");
	EasyLocal::Core::SimulatedAnnealingEvaluationBased<ETT_Input, ETT_State, decltype(ETT_nheMoveKempe)::MoveType> ETT_bsa_kempe(in, ETT_sm, ETT_nheMoveKempe, "TSA");
	EasyLocal::Core::SimulatedAnnealingEvaluationBased<ETT_Input, ETT_State, decltype(ETT_nheMoveShake)::MoveType> ETT_bsa_shake(in, ETT_sm, ETT_nheMoveShake, "QSA");
	EasyLocal::Core::SimulatedAnnealingEvaluationBased<ETT_Input, ETT_State, decltype(ETT_nheMoveComponent)::MoveType> ETT_bsa_component(in, ETT_sm, ETT_nheMoveComponent, "CSA");

#ifndef UNCAPACITATED
	EasyLocal::Core::SimulatedAnnealingTimeBased<ETT_Input, ETT_State, decltype(ETT_nheMoveBeam)::MoveType> ETT_bsa_beam_b(in, ETT_sm, ETT_nheMoveBeam, "SSA_B");
	EasyLocal::Core::SimulatedAnnealingEvaluationBased<ETT_Input, ETT_State, decltype(ETT_nheMoveBeam)::MoveType> ETT_bsa_beam(in, ETT_sm, ETT_nheMoveBeam, "SSA");
	EasyLocal::Core::HillClimbing<ETT_Input, ETT_State, decltype(ETT_nheMoveBeam)::MoveType> ETT_hca(in, ETT_sm, ETT_nheMoveBeam, "HCA");

	EasyLocal::Core::LateAcceptanceHillClimbing<ETT_Input, ETT_State, decltype(ETT_nheMoveBeam)::MoveType> ETT_lsa_beam(in, ETT_sm, ETT_nheMoveBeam, "SLA");
#endif

	// tester
	EasyLocal::Debug::Tester<ETT_Input, ETT_Output, ETT_State> tester(in,ETT_sm,ETT_om);
	EasyLocal::Debug::MoveTester<ETT_Input, ETT_Output, ETT_State, ETT_MoveExam> move_test(in,ETT_sm,ETT_om,ETT_nheMove, "ETT_MoveExam move", tester);
	EasyLocal::Debug::MoveTester<ETT_Input, ETT_Output, ETT_State, ETT_SwapExam> swap_test(in,ETT_sm,ETT_om,ETT_nheSwap, "ETT_SwapExam swap", tester);
	EasyLocal::Debug::MoveTester<ETT_Input, ETT_Output, ETT_State, ETT_KickExam> kick_test(in,ETT_sm,ETT_om,ETT_nheKick, "ETT_KickExam kick", tester);
	EasyLocal::Debug::MoveTester<ETT_Input, ETT_Output, ETT_State, ETT_KempeExam> kempe_test(in,ETT_sm,ETT_om,ETT_nheKempe, "ETT_KempeExam kempe", tester);
	EasyLocal::Debug::MoveTester<ETT_Input, ETT_Output, ETT_State, ETT_ShakeExam> shake_test(in,ETT_sm,ETT_om,ETT_nheShake, "ETT_ShakeExam shake", tester);
	EasyLocal::Debug::MoveTester<ETT_Input, ETT_Output, ETT_State, ETT_ComponentExam> component_test(in,ETT_sm,ETT_om,ETT_nheComponent, "ETT_ComponentExam component", tester);

#ifndef UNCAPACITATED
	EasyLocal::Debug::MoveTester<ETT_Input, ETT_Output, ETT_State, ETT_BeamExam> beam_test(in,ETT_sm,ETT_om,ETT_nheBeam, "ETT_BeamExam beam", tester);
#endif

	EasyLocal::Debug::MoveTester<ETT_Input, ETT_Output, ETT_State, decltype(ETT_nheMoveSwap)::MoveType> move_swap_test(in,ETT_sm,ETT_om,ETT_nheMoveSwap, "ETT_MoveSwapExam move/swap", tester);
	EasyLocal::Debug::MoveTester<ETT_Input, ETT_Output, ETT_State, decltype(ETT_nheMoveKick)::MoveType> move_kick_test(in,ETT_sm,ETT_om,ETT_nheMoveKick, "ETT_MoveKickExam move/kick", tester);
	EasyLocal::Debug::MoveTester<ETT_Input, ETT_Output, ETT_State, decltype(ETT_nheMoveKempe)::MoveType> move_kempe_test(in,ETT_sm,ETT_om,ETT_nheMoveKempe, "ETT_MoveKickExam move/kick/kempe", tester);

	EasyLocal::Core::SimpleLocalSearch<ETT_Input, ETT_Output, ETT_State> ETT_solver(in, ETT_sm, ETT_om, "ETT solver");
	if (!EasyLocal::Debug::CommandLineParameters::Parse(argc, argv, true, false))
		return 1;

	if (!method.IsSet())
	{ // If no search method is set -> enter in the tester
	 	if (feasibleOnly != 1) {
			// We return a very large cost when entering the infeasible state
			// --> Delta's could be wrong
			std::cout << "=========================================" << std::endl;
			std::cout << "FEASIBLE ONLY != 1" << std::endl;
			std::cout << "WARNING: DELTA CHECKS COULD GIVE ERRORS" << std::endl;
			std::cout << "=========================================" << std::endl;
		}
 
		if (init_state.IsSet())
			tester.RunMainMenu(init_state);
		else
			tester.RunMainMenu();
	}
	else
	{

		if (method == std::string("SA"))
		{
			ETT_solver.SetRunner(ETT_sa);
		}
		else if (method == std::string("BLA"))
		{
			ETT_solver.SetRunner(ETT_bla);
		}
		else if (method == std::string("BSA"))
		{
			ETT_solver.SetRunner(ETT_bsa);
		}
		else if (method == std::string("BSA2"))
		{
			ETT_solver.SetRunner(ETT_bsa_kick);
		}
		else if (method == std::string("TSA"))
		{
			ETT_solver.SetRunner(ETT_bsa_kempe);
		}
		else if (method == std::string("QSA"))
		{
			ETT_solver.SetRunner(ETT_bsa_shake);
		}
		else if (method == std::string("CSA"))
		{
			ETT_solver.SetRunner(ETT_bsa_component);
		}
#ifndef UNCAPACITATED
		else if (method == std::string("SSA"))
		{
			ETT_solver.SetRunner(ETT_bsa_beam);
		}
                else if (method == std::string("SSA_B"))
		{
			ETT_solver.SetRunner(ETT_bsa_beam_b);
		}
		else if (method == std::string("SLA"))
		{
			ETT_solver.SetRunner(ETT_lsa_beam);
		}
#endif
		else if (method == std::string("HC"))
		{
			ETT_solver.SetRunner(ETT_hc);
		}
		else // if (method.GetValue() == std::string("SD"))
		{
			ETT_solver.SetRunner(ETT_sd);
		}
		

		//SolverResult<Input, Output, CostStructure> 
		ETT_Output out(in);
		EasyLocal::Core::SolverResult<ETT_Input, ETT_Output, EasyLocal::Core::DefaultCostStructure<int>> result(out);
		//auto result = ETT_solver.Solve();
			

		/********************************
		*  Determine evaluation limit  *
		********************************/
		//// In order to have deterministic behaviour with random seeds: always perform this	
		//	// Determine for how many evaluations we can run the algorithm
		//	// This is done by computing how long 10000 iterations take, and then scaling up
		//	// Start from a random solution
		//	const unsigned long smallEval = 1000000;
		//	ETT_solver.GetRunner()->SetParameter("max_evaluations", smallEval);

		//	// Read an initial solution
		//	if (init_method == std::string("SD") || init_method == std::string("OBSI") || init_method == std::string("SDFeas") || init_method == std::string("OBSIFeas")){
		//		size_t lastdot = static_cast<std::string>(instance).find_last_of(".");
		//		std::string prefix = static_cast<std::string>(instance).substr(0, lastdot);

		//		size_t lastdash = static_cast<std::string>(instance).find_last_of("/");
		//		if(lastdash != std::string::npos){
		//			prefix = static_cast<std::string>(prefix).substr(lastdash+1);
		//		} 

		//		int id;
		//		if(init_method == std::string("SD") || init_method == std::string("OBSI")){
		//			id = EasyLocal::Core::Random::Uniform<int>(0, 99);
		//		} else {
		//			id = EasyLocal::Core::Random::Uniform<int>(0, 19);
		//		}
		//		std::string fileName = "./InitMethods/SolStore/" + prefix + "_" + static_cast<std::string>(init_method) + "_" + std::to_string(id) + ".sol";
		//		std::ifstream is(static_cast<std::string>(fileName));
		//		if(is.fail()){
		//			std::cout << "File " << fileName << " does not exist!" << std::endl;
		//			std::abort();
		//		}
		//		ETT_Output out_warmstart(in);
		//		is >> out_warmstart;
		//		result = ETT_solver.Resolve(out_warmstart); 
		//	} else if (init_method == std::string("random")) {
		//		result = ETT_solver.Solve();
		//	} else {
		//		std::cout << "Init method " << static_cast<std::string>(init_method) << " not known" << std::endl;
		//		std::abort();
		//	}
		
		// In order to determine the budget, determine the number of evaluations that are possible
		const unsigned long smallEval = 1000000;
		ETT_solver.GetRunner()->SetParameter("max_evaluations", smallEval);
		result = ETT_solver.Solve();
		auto smallRunTime = result.running_time;


		if (!max_evaluations.IsSet()){
			// TODO TODO
			const int ITC_FACTOR = 1;
			const int ITC_TIME = 264*ITC_FACTOR; // Total time determined by the official ITC2007 benchmarking tool
			max_evaluations = (unsigned long)(((ITC_TIME+0.0)/(smallRunTime+0.0))*smallEval);
			std::cout << smallEval << " evaluations took " << smallRunTime << " seconds" << std::endl;
			std::cout << "Max evaluations within " << ITC_TIME << ": " << max_evaluations << std::endl;
			ETT_solver.GetRunner()->SetParameter("max_evaluations", (unsigned long) max_evaluations);
		} else {
			ETT_solver.GetRunner()->SetParameter("max_evaluations", (unsigned long) max_evaluations);
		}

		/***************************
		*  Do the actual solving  *
		***************************/
		

		// Read an initial solution
		if (init_method == std::string("SD") || init_method == std::string("OBSI") || init_method == std::string("SDFeas") || init_method == std::string("OBSIFeas")){
			size_t lastdot = static_cast<std::string>(instance).find_last_of(".");
			std::string prefix = static_cast<std::string>(instance).substr(0, lastdot);

			size_t lastdash = static_cast<std::string>(instance).find_last_of("/");
			if(lastdash != std::string::npos){
				prefix = static_cast<std::string>(prefix).substr(lastdash+1);
			} 

			int id;
			if(init_method == std::string("SD") || init_method == std::string("OBSI")){
		       		id = EasyLocal::Core::Random::Uniform<int>(0, 99);
			} else {
		       		id = EasyLocal::Core::Random::Uniform<int>(0, 19);
			}
			std::string fileName = "./InitMethods/SolStore/" + prefix + "_" + static_cast<std::string>(init_method) + "_" + std::to_string(id) + ".sol";
			std::ifstream is(static_cast<std::string>(fileName));
			if(is.fail()){
				std::cout << "File " << fileName << " does not exist!" << std::endl;
				std::abort();
			}
			ETT_Output out_warmstart(in);
			is >> out_warmstart;
			result = ETT_solver.Resolve(out_warmstart); 
		} else if (init_method == std::string("random")) {
			result = ETT_solver.Solve();
		} else {
			std::cout << "Init method " << static_cast<std::string>(init_method) << " not known" << std::endl;
			std::abort();
		}


		auto time = result.running_time;
		auto pre_time = time;
		out = result.output;
		// If pre_violations > 0, update violations
		int violations = 0, pre_violations = result.cost.violations/hard_weight;

		if (pre_violations > 0) {
			// there are violations (due to a low value of hard_weight)
  	        	// run a post-processing MHC for removing them 
//#ifndef NDEBUG
			std::cerr << "Pre time: " << pre_time << std::endl;
			std::cerr << "Cost: " << result.cost << std::endl;
			std::cerr << pre_violations << " violations, repair the solution" << std::endl;
//#endif
			hard_weight = 1000;
			cc1.SetWeight(hard_weight);
#ifndef UNCAPACITATED
			cc2.SetWeight(hard_weight);
			cc3.SetWeight(hard_weight);
			cc4.SetWeight(hard_weight);
#endif
		        ETT_solver.SetRunner(ETT_sd);
			result = ETT_solver.Resolve(result.output); 
			time += result.running_time;
			violations = result.cost.violations/hard_weight;

			if(violations > 0){
				ETT_solver.SetRunner(ETT_hc);
				// TODO TODO TODO
				ETT_hc.SetParameter("max_idle_iterations", 10000000UL);
				result = ETT_solver.Resolve(result.output); 
				time += result.running_time;
				violations = result.cost.violations/hard_weight;
			}
			std::cerr << "Repair time: " << time << std::endl;
			std::cerr << violations << " violations after repair" << std::endl;
		} else {
			// A small local search to ensure we are in a local optimum
			std::cout << "Cost before local search: " << result.cost << std::endl;
			std::cout << "Time: " << time << "s " << std::endl;
			std::cout << "Start local search" << std::endl;

			const int POST_TIME = 10; // 10 seconds of post processing time
			max_evaluations = (unsigned long)(((POST_TIME+0.0)/(smallRunTime+0.0))*smallEval);


			ETT_hca.SetParameter("max_evaluations", (unsigned long) max_evaluations);
			ETT_hca.SetParameter("max_idle_iterations", (unsigned long) max_evaluations);
		        ETT_solver.SetRunner(ETT_hca);
			result = ETT_solver.Resolve(result.output); 
			time += result.running_time;			
			std::cout << "Cost after local search: " << result.cost << std::endl;
			std::cout << "Time: " << time << "s " << std::endl;
		}

		if (output_file.IsSet()){
			// write the output on the file passed in the command line
			std::ofstream os(static_cast<std::string>(output_file).c_str());
			os << out << std::endl;
#ifndef UNCAPACITATED
			if(result.cost.violations > 0){
				os << "Cost: " << -1 << std::endl;
			} else {
				os << "Cost: " << result.cost.total << std::endl;
			}
#else
			os << "Cost: " << (result.cost.total+0.0)/in.getNoStudents() << std::endl;
#endif
			os << "Time: " << time << "s " << std::endl;
			os.close();
		}
		else
		  { 
		    // write the solution in the standard output
#ifndef UNCAPACITATED
		    std::cout << "{\"cost\": " <<  result.cost.total  <<  ", "
#else
		    std::cout << "{\"cost\": " <<  (result.cost.total+0.0)/in.getNoStudents()  <<  ", "
#endif
			      << "\"objective\": " << result.cost.objective << ", "
			      << "\"pre_violations\": " << pre_violations << ", "
			      << "\"violations\": " << violations << ", ";
		    for (int i = 0; i < components_order.size(); i++)
		      std::cout << "\"" << components_order[i] << "\": " << result.cost[i] << ", " ;
		    std::cout  << "\"time\": " << time << ", "
			       << "\"pretime\": " << pre_time << ", "
			       << "\"seed\": " << EasyLocal::Core::Random::GetSeed() << "} " << std::endl;				
		}
	}
	return 0;
}
