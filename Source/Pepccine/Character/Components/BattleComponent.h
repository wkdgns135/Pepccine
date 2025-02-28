#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BattleComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PEPCCINE_API UBattleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBattleComponent();

	void SendHitResult(AActor* HitTarget, float DamageAmount, FHitResult HitResult);
	void ReceiveHitResult(float DamageAmount, AActor* DamageCauser, FHitResult HitResult);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
														 FActorComponentTickFunction* ThisTickFunction) override;
};

