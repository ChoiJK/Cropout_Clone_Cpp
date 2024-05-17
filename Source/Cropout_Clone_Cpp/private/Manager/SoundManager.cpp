// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/SoundManager.h"

#include "AudioModulationStatics.h"
#include "SoundControlBus.h"
#include "Core/CropoutGameInstance.h"
#include "Data/SoundMixClassInfo.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/TableManager.h"

USoundManager::USoundManager()
{
}

void USoundManager::Init()
{
	Super::Init();

	FName pianoVolPath = TEXT(
		"AudioModulation.SoundControlBus'/Game/Audio/DATA/ControlBus/Cropout_Music_Piano_Vol.Cropout_Music_Piano_Vol'");
	USoundControlBus* pianoVolControlBus = Cast<USoundControlBus>(
		StaticLoadObject(USoundControlBus::StaticClass(), nullptr, *pianoVolPath.ToString()));

	FName percVolPath = TEXT(
		"AudioModulation.SoundControlBus'/Game/Audio/DATA/ControlBus/Cropout_Music_Perc_Vol.Cropout_Music_Perc_Vol'");
	USoundControlBus* percVolControlBus = Cast<USoundControlBus>(
		StaticLoadObject(USoundControlBus::StaticClass(), nullptr, *percVolPath.ToString()));

	FName stringsVolPath = TEXT(
		"AudioModulation.SoundControlBus'/Game/Audio/DATA/ControlBus/Cropout_Music_Strings_Delay.Cropout_Music_Strings_Delay'");
	USoundControlBus* stringsVolControlBus = Cast<USoundControlBus>(
		StaticLoadObject(USoundControlBus::StaticClass(), nullptr, *stringsVolPath.ToString()));

	FName winLosePath = TEXT(
		"AudioModulation.SoundControlBus'/Game/Audio/DATA/ControlBus/Cropout_Music_WinLose.Cropout_Music_WinLose'");
	USoundControlBus* winLoseControlBus = Cast<USoundControlBus>(
		StaticLoadObject(USoundControlBus::StaticClass(), nullptr, *winLosePath.ToString()));

	FName musicStopPath = TEXT(
		"AudioModulation.SoundControlBus'/Game/Audio/DATA/ControlBus/Cropout_Music_Stop.Cropout_Music_Stop'");
	USoundControlBus* musicStopControlBus = Cast<USoundControlBus>(
		StaticLoadObject(USoundControlBus::StaticClass(), nullptr, *musicStopPath.ToString()));

	FName newMapPath = TEXT(
		"AudioModulation.SoundControlBus'/Game/Audio/DATA/ControlBus/Cropout_Music_NewMap.Cropout_Music_NewMap'");
	USoundControlBus* newMapControlBus = Cast<USoundControlBus>(
		StaticLoadObject(USoundControlBus::StaticClass(), nullptr, *newMapPath.ToString()));

	UAudioModulationStatics::SetGlobalBusMixValue(GetWorld(), pianoVolControlBus, 1.f, 1.f);
	UAudioModulationStatics::SetGlobalBusMixValue(GetWorld(), percVolControlBus, 1.f, 1.f);
	UAudioModulationStatics::SetGlobalBusMixValue(GetWorld(), stringsVolControlBus, 0.f, 1.f);

	UAudioModulationStatics::SetGlobalBusMixValue(GetWorld(), winLoseControlBus, 0.5f, 0.f);
	UAudioModulationStatics::SetGlobalBusMixValue(GetWorld(), musicStopControlBus, 0.f, 0.f);

	UAudioModulationStatics::SetGlobalBusMixValue(GetWorld(), newMapControlBus, 1.f);

	FName mainMusicPath = TEXT(
		"MetasoundEngine.MetaSoundSource'/Game/Audio/MUSIC/MUS_Main_MSS.MUS_Main_MSS'");
	MainMusic = Cast<USoundBase>(
		StaticLoadObject(USoundBase::StaticClass(), nullptr, *mainMusicPath.ToString()));
}

void USoundManager::Prepare()
{
	Super::Prepare();
}

void USoundManager::Release()
{
	Super::Release();
}

void USoundManager::Destroy()
{
	Super::Destroy();
}

void USoundManager::OnLevelLoaded()
{
	Super::OnLevelLoaded();
	MusicComponent = UGameplayStatics::CreateSound2D(GetWorld(), MainMusic, 1.f, 1.f, 0.f,
		nullptr, true);

	ApplySoundVolume();
}

void USoundManager::OnLevelDestroy()
{
	Super::OnLevelDestroy();

	if(MusicComponent)
	{
		MusicComponent->Stop();
		MusicComponent = nullptr;
	}
}

void USoundManager::OnSaveData(UOptionSaveData* SaveData)
{
	Super::OnSaveData(SaveData);

	SaveData->SetSoundMix(0, SoundMixes[0]);
	SaveData->SetSoundMix(1, SoundMixes[1]);
}

void USoundManager::OnLoadData(UOptionSaveData* SaveData)
{
	Super::OnLoadData(SaveData);

	SoundMixes[0] = SaveData->GetSoundMix(0);
	SoundMixes[1] = SaveData->GetSoundMix(1);

	ApplySoundVolume();
}


void USoundManager::PlayMusic()
{
	if(MusicComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("MusicComponent is nullptr"));
		return;
	}

	MusicComponent->Play();
}

void USoundManager::StopMusic()
{
	if(MusicComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("MusicComponent is nullptr"));
		return;
	}

	MusicComponent->Stop();
}

float USoundManager::GetSoundMix(int index) const
{
	if(index < 0 || index >= 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid SoundMix index"));
		return 1.f;
	}

	return SoundMixes[index];
}

void USoundManager::SetSoundMix(int index, float value)
{
	if(index < 0 || index >= 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid SoundMix index"));
		return;
	}

	SoundMixes[index] = value;

	ApplySoundVolume();
}

void USoundManager::ApplySoundVolume() const
{
	UCropoutGameInstance* gameInstance = UCropoutGameInstance::GetInstance();
	UTableManager* tableManager = gameInstance->GetSystemManager<UTableManager>();

	for(int index = 0; index < 2; ++index)
	{
		const FSoundMixClassInfo* soundMixClass = tableManager->GetSoundMixClassInfo(static_cast<ESoundMixType>(index));

		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), soundMixClass->InSoundMixModifier,
			soundMixClass->InSoundClass, SoundMixes[index]);
	}
}
