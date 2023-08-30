#ifndef ETT_STATEMANAGER_H
#define ETT_STATEMANAGER_H

#include "../Basics/ETT_State.h"
#include "../Basics/ETT_Move.h"
const int HARD_WEIGHT = 1;
#define HARD_WEIGHT_SET
#include <easylocal.hh>

class ETT_StateManager : public EasyLocal::Core::StateManager<ETT_Input,ETT_State> {
	public:
		ETT_StateManager(const ETT_Input &);
		void RandomState(ETT_State&);   
		void HeuristicState(ETT_State&);
		bool CheckConsistency(const ETT_State& st) const;
	protected:
}; 


#endif /* ETT_STATEMANAGER_H */
