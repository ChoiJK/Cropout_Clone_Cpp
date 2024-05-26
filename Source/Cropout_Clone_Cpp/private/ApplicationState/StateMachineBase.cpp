#include "ApplicationState/StateMachineBase.h"
#include "ApplicationState/StateTransitionProcessor.h"

FStateTransition::FStateTransition(): Trigger(), ToState(), TransitionProcessor(nullptr)
{
}

FStateTransition::FStateTransition(EStateTrigger trigger, ECropoutState toState,
	UStateTransitionProcessor* transitionProcessor): Trigger(trigger), ToState(toState),
	                                                 TransitionProcessor(transitionProcessor)
{
}
