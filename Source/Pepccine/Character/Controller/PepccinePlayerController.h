// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PepccinePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class PEPCCINE_API APepccinePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
  APepccinePlayerController();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
  UInputMappingContext* InputMappingContext;
  // W A S D
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
  UInputAction* MoveAction;
  // Space
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
  UInputAction* JumpAction;
  // Mouse
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
  UInputAction* LookAction;
  // Shift
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
  UInputAction* SprintAction;
  // Q
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
  UInputAction* ItemUseAction;
  // Ctrl
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
  UInputAction* CrouchAction;
  // R
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
  UInputAction* ReloadingAction;
  // Mouse Wheel
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
  UInputAction* SwapAction;
  // E
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
  UInputAction* InteractiveAction;
  // Esc
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
  UInputAction* MenuAction;
  // Tab
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
  UInputAction* InventoryAction;

protected:
  virtual void BeginPlay() override;
  virtual void SetupInputComponent() override;

  virtual void OpenMenu();
  virtual void OpenInventory();

};
