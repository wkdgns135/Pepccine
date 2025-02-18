#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseMonster.generated.h"

class UCapsuleComponent;

UCLASS()
class PEPCCINE_API ABaseMonster : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseMonster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	FName MonsterType;

protected:
	virtual void BeginPlay() override;
public:	
	
	void OnDeath();
	void OnHit();

	virtual void Tick(float DeltaTime) override;

};
