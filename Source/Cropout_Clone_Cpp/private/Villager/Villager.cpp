#include "VIllager/Villager.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Core/CropoutGameInstance.h"
#include "Core/CropoutGameMode.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StreamableManager.h"
#include "Interactable/Resource/ResourceBaseActor.h"
#include "Kismet/GameplayStatics.h"
#include "PlayMontageCallbackProxy.h"

TArray<USkeletalMesh*> AVillager::HairMeshes;

AVillager::AVillager()
{
	bUseControllerRotationYaw = true;
	StoreResourceType = EResourceType::None;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);
	Capsule->InitCapsuleSize(30.f, 92.04f);

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(Capsule);
	BodyMesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -92.04f), FRotator(0.f, -90.f, 0.f));

	ToolMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ToolMesh"));
	ToolMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform,TEXT("hand_rSocket"));

	HatMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HatMesh"));
	HatMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform,TEXT("headSocket"));

	HairMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
	HairMesh->SetupAttachment(BodyMesh);

	//ShadowDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("ShadowDecal"));
	//ShadowDecal->SetupAttachment(Capsule);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	Movement->MaxSpeed = 300.f;

	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> ab_Villager
		(TEXT("Engine.AnimBlueprint'/Game/Characters/Animations/ABP_Villager.ABP_Villager'"));
	check(ab_Villager.Succeeded());
	BodyAnimBlueprint = ab_Villager.Object;
	BodyMesh->SetAnimInstanceClass(BodyAnimBlueprint->GeneratedClass);


	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> ab_Hair
		(TEXT("Engine.AnimBlueprint'/Game/Characters/Animations/ABP_Hair.ABP_Hair'"));
	check(ab_Hair.Succeeded());
	HairAnimBlueprint = ab_Hair.Object;
	HairMesh->SetAnimInstanceClass(HairAnimBlueprint->GeneratedClass);

	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> ab_Hat
		(TEXT("Engine.AnimBlueprint'/Game/Characters/Meshes/Hats/ABP_Hat.ABP_Hat'"));
	check(ab_Hat.Succeeded());
	HatAnimBlueprint = ab_Hat.Object;
	HatMesh->SetAnimInstanceClass(HatAnimBlueprint->GeneratedClass);


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

	GameInstance = Cast<UCropoutGameInstance>(GetGameInstance());
	GameMode = Cast<ACropoutGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	AddActorWorldOffset(FVector(0.f, 0.f, Capsule->GetScaledCapsuleHalfHeight()));

	GetWorld()->GetTimerManager().SetTimer(EatTimerHandle, this, &AVillager::Eat, 24.f, true);

	ChangeJob(EVillagerJobType::Idle);
	HairMesh->SetSkeletalMesh(HairMeshes[FMath::RandRange(0, HairMeshes.Num() - 1)]);
	HairMesh->SetCustomPrimitiveDataFloat(0, FMath::RandRange(0.f, 1.f));
}

void AVillager::Destroyed()
{
	Super::Destroyed();

	BodyMesh->SetAnimInstanceClass(nullptr);
	HairMesh->SetAnimInstanceClass(nullptr);
	HatMesh->SetAnimInstanceClass(nullptr);
}

void AVillager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AVillager::Eat()
{
	int withdrawnAmount = 0;
	GameMode->ExtractResource(EResourceType::Food, 3, withdrawnAmount);
}

void AVillager::Action(AActor* jobAction)
{
	if(jobAction == nullptr || jobAction->IsValidLowLevel() == false || jobAction->Tags.Num() == 0)
	{
		return;
	}

	EVillagerJobType nextJobType = GetVillagerJobTypeFromString(jobAction->Tags[0].ToString());
	if(nextJobType == EVillagerJobType::None)
	{
		return;
	}

	TargetRef = jobAction;

	ChangeJob(nextJobType);
}

void AVillager::PlayWorkAnim(float delay)
{
	UPlayMontageCallbackProxy* playMontageCallbackProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(
		BodyMesh, WorkAnim, 1.f, 0.f, NAME_None);

	playMontageCallbackProxy->OnCompleted.AddUniqueDynamic(this, &AVillager::PlayMontageCallback);
	playMontageCallbackProxy->OnInterrupted.AddUniqueDynamic(this, &AVillager::PlayMontageCallback);

	FTimerHandle WorkAnimTimerHandle;
	GetWorldTimerManager().SetTimer(WorkAnimTimerHandle, FTimerDelegate::CreateWeakLambda(this, [this]
	{
		BodyMesh->GetAnimInstance()->Montage_StopGroupByName(
			0.f, TEXT("DefaultGroup"));
	}), delay, false);

	ToolMesh->SetVisibility(true);
	ToolMesh->SetStaticMesh(TargetTool);
}

void AVillager::PlayMontageCallback(FName NotifyName)
{
	ToolMesh->SetVisibility(false);
}

void AVillager::SetDefaultJob()
{
	ChangeJob(EVillagerJobType::Idle);
}

void AVillager::StoreResource(EResourceType type, int amount)
{
	StoreResourceType = type;
	StoreAmount = amount;
}

void AVillager::DeliverResource()
{
	if(StoreResourceType != EResourceType::None)
	{
		GameMode->StoreResource(StoreResourceType, StoreAmount);
		StoreResourceType = EResourceType::None;
		StoreAmount = 0;
	}
}

void AVillager::ChangeJob(EVillagerJobType jobType)
{
	if(CurrentJobType == jobType)
	{
		return;
	}

	CurrentJobType = jobType;
	CurrentJobName = FName(GetVillagerJobTypeString(CurrentJobType));

	if(MapVillagerJob.Find(CurrentJobName) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Villager %s has no job"), *GetName());
		return;
	}

	if(CurrentJobType == EVillagerJobType::None)
	{
		return;
	}

	if(Tags.Find(CurrentJobName) == INDEX_NONE)
	{
		Tags = {CurrentJobName};
	}

	ResetJobState();

	const FVillagerJob currentJob = MapVillagerJob.FindRef(CurrentJobName);

	TArray<FSoftObjectPath> classRefs;
	classRefs.Add(currentJob.BehaviorTree.ToSoftObjectPath());
	classRefs.Add(currentJob.WorkAnim.ToSoftObjectPath());
	classRefs.Add(currentJob.Hat.ToSoftObjectPath());
	classRefs.Add(currentJob.Tool.ToSoftObjectPath());

	GameInstance->GetStreamableManager().RequestAsyncLoad(classRefs, FStreamableDelegate::CreateUObject(
		                                                      this, &AVillager::AsyncJobResourceLoaded,
		                                                      CurrentJobName));
}

void AVillager::AsyncJobResourceLoaded(FName jobName)
{
	if(jobName != CurrentJobName)
	{
		// async작업도중 CurrentJob이 바뀌었을 때	
		return;
	}

	const FVillagerJob currentJob = MapVillagerJob.FindRef(CurrentJobName);
	UBehaviorTree* behaviorTree = Cast<UBehaviorTree>(currentJob.BehaviorTree.Get());
	UAnimMontage* workAnimMontage = Cast<UAnimMontage>(currentJob.WorkAnim.Get());
	USkeletalMesh* hatSkeletalMesh = Cast<USkeletalMesh>(currentJob.Hat.Get());
	UStaticMesh* toolStaticMesh = Cast<UStaticMesh>(currentJob.Tool.Get());

	AAIController* aiController = Cast<AAIController>(GetController());
	if(aiController && aiController->IsValidLowLevel())
	{
		ActiveBehaviorTree = behaviorTree;
		aiController->RunBehaviorTree(behaviorTree);
		if(TargetRef && TargetRef->IsValidLowLevel())
		{
			aiController->GetBlackboardComponent()->SetValueAsObject(FName("Target"), TargetRef);
		}
	}

	HatMesh->SetVisibility(true);
	HatMesh->SetSkeletalMesh(hatSkeletalMesh);

	WorkAnim = workAnimMontage;
	TargetTool = toolStaticMesh;
}

void AVillager::ResetJobState()
{
	StopJob();

	HatMesh->SetSkeletalMesh(nullptr);
	HatMesh->SetVisibility(false);

	ToolMesh->SetStaticMesh(nullptr);
	ToolMesh->SetVisibility(false);

	TargetTool = nullptr;
}

void AVillager::StopJob()
{
	ToolMesh->SetStaticMesh(nullptr);
	ToolMesh->SetVisibility(false);

	UAnimInstance* animIns = BodyMesh->GetAnimInstance();
	if(animIns != nullptr)
	{
		animIns->Montage_StopGroupByName(0.f, TEXT("DefaultGroup"));
	}
	AAIController* aiController = Cast<AAIController>(GetController());
	if(aiController && aiController->IsValidLowLevel())
	{
		aiController->StopMovement();
	}

	Quantity = 0;
}
