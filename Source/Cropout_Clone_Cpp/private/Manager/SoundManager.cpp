// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/SoundManager.h"

#include "AudioModulationStatics.h"
#include "SoundControlBus.h"
#include "Kismet/GameplayStatics.h"

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
	USoundBase* mainMusic = Cast<USoundBase>(
		StaticLoadObject(USoundBase::StaticClass(), nullptr, *mainMusicPath.ToString()));
	MusicComponent = UGameplayStatics::CreateSound2D(GetWorld(), mainMusic, 1.f, 1.f, 0.f,
		nullptr, true);
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


void USoundManager::PlayMusic()
{
	MusicComponent->Play();
}

void USoundManager::StopMusic()
{
	MusicComponent->Stop();
}
