#include "Core/ApplicationState/StateMachineBase.h"
#include "Core/ApplicationState/StateTransitionProcessor.h"

FStateTransition::FStateTransition(): Trigger(), ToState(), TransitionProcessor(nullptr)
{
}

FStateTransition::FStateTransition(EStateTrigger trigger, ECropoutState toState,
	UStateTransitionProcessor* transitionProcessor): Trigger(trigger), ToState(toState),
	                                                 TransitionProcessor(transitionProcessor)
{
}
