#include "VIllager/Villager.h"

#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Engine/SkeletalMesh.h"

TArray<USkeletalMesh*> AVillager::HairMeshes;

AVillager::AVillager()
{
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);
	Capsule->InitCapsuleSize(30.f, 92.04f);

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(Capsule);
	BodyMesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -92.04f), FRotator(0.f, -90.f, 0.f));

	ToolMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ToolMesh"));
	ToolMesh->SetupAttachment(BodyMesh);

	HatMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HatMesh"));
	HatMesh->SetupAttachment(BodyMesh);

	HairMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
	HairMesh->SetupAttachment(BodyMesh);

	//ShadowDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("ShadowDecal"));
	//ShadowDecal->SetupAttachment(Capsule);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sm_Villager
		(TEXT("Engine.SkeletalMesh'/Game/Characters/Meshes/SKM_Villager.SKM_Villager'"));

	check(sm_Villager.Succeeded());
	BodyMesh->SetSkeletalMesh(sm_Villager.Object);

	if(HairMeshes.IsEmpty())
	{
		TArray<const TCHAR*> hairPath;
		hairPath.Add(TEXT("Engine.SkeletalMesh'/Game/Characters/Meshes/Hair/SKM_Hair01.SKM_Hair01'"));
		hairPath.Add(TEXT("Engine.SkeletalMesh'/Game/Characters/Meshes/Hair/SKM_Hair02.SKM_Hair02'"));
		hairPath.Add(TEXT("Engine.SkeletalMesh'/Game/Characters/Meshes/Hair/SKM_Hair03.SKM_Hair03'"));
		hairPath.Add(TEXT("Engine.SkeletalMesh'/Game/Characters/Meshes/Hair/SKM_Hair04.SKM_Hair04'"));
		hairPath.Add(TEXT("Engine.SkeletalMesh'/Game/Characters/Meshes/Hair/SKM_Hair05.SKM_Hair05'"));
		hairPath.Add(TEXT("Engine.SkeletalMesh'/Game/Characters/Meshes/Hair/SKM_Hair06.SKM_Hair06'"));

		for(auto HairPath : hairPath)
		{
			static ConstructorHelpers::FObjectFinder<USkeletalMesh> sm_hair(HairPath);

			check(sm_hair.Succeeded());
			HairMeshes.Add(sm_hair.Object);
		}
	}

	ConstructorHelpers::FObjectFinder<UDataTable> villagerJob
		(TEXT("Engine.DataTable'/Game/Blueprint/Villager/DT_VillagerJobs.DT_VillagerJobs'"));
	check(villagerJob.Succeeded());

	TArray<FName> rowNames = villagerJob.Object->GetRowNames();
	for(auto rowName : rowNames)
	{
		FVillagerJob* job = villagerJob.Object->FindRow<FVillagerJob>(rowName, TEXT("VillagerJob"));
		MapVillagerJob.Add(rowName, *job);
	}
}

void AVillager::BeginPlay()
{
	Super::BeginPlay();
	AddActorWorldOffset(FVector(0.f, 0.f, Capsule->GetScaledCapsuleHalfHeight()));

	GetWorld()->GetTimerManager().SetTimer(EatTimerHandle, this, &AVillager::Eat, 24.f, true);

	ChangeJob(EVillagerJobType::Idle);
	HairMesh->SetSkeletalMesh(HairMeshes[FMath::RandRange(0, HairMeshes.Num() - 1)]);
	HairMesh->SetCustomPrimitiveDataFloat(0, FMath::RandRange(0.f, 1.f));
}

void AVillager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AVillager::Eat()
{
}

void AVillager::Action(AActor* jobAction)
{
}

void AVillager::ChangeJob(EVillagerJobType jobType)
{
	if(JobType == jobType)
	{
		return;
	}

	JobType = jobType;
}
