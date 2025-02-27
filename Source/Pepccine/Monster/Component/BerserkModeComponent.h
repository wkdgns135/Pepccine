#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BerserkModeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UBerserkModeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBerserkModeComponent();

	UFUNCTION(BlueprintCallable, Category = "Berserk")
	void EnterBerserkMode();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Berserk")
	bool bIsBerserkMode = false;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Berserk")
	UAnimMontage* ModeChangeMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Berserk")
	float AttackMultiplier = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Berserk")
	float DefenseMultiplier = 1.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Berserk")
	float SpeedMultiplier = 1.2f;

	virtual void BeginPlay() override;

private:	
	void PlayModeChangeMontage();
};
