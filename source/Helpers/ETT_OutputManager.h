#ifndef ETT_OUTPUTMANAGER_H
#define ETT_OUTPUTMANAGER_H

#include "../Basics/ETT_State.h"
#include "../Basics/ETT_Move.h"
#include "../Data/ETT_Output.h"
#include "ETT_StateManager.h"

class ETT_OutputManager : public EasyLocal::Core::OutputManager<ETT_Input,ETT_Output,ETT_State> {
	public:
		ETT_OutputManager(const ETT_Input & pin)
			: OutputManager<ETT_Input,ETT_Output,ETT_State>(pin,"ETTOutputManager") {}
		void InputState(ETT_State&, const ETT_Output&) const;  
		void OutputState(const ETT_State&, ETT_Output&) const; 
}; 


#endif /* ETT_OUTPUTMANAGER_H */
