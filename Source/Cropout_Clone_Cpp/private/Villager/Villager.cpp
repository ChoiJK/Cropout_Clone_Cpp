#include "VIllager/Villager.h"

AVillager::AVillager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AVillager::BeginPlay()
{
	Super::BeginPlay();
}

void AVillager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVillager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AVillager::Action(AActor* jobAction)
{
}
