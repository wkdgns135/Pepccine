// Fill out your copyright notice in the Description page of Project Settings.


#include "Pepccine/Character/Player/PepCharacter.h"

// Sets default values
APepCharacter::APepCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APepCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APepCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APepCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

