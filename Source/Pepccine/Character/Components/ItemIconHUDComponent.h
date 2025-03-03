#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Widget/ActiveItemWidget.h"
#include "Character/Widget/WeaponWidget.h"
#include "ItemIconHUDComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PEPCCINE_API UItemIconHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UItemIconHUDComponent();

	UPROPERTY(EditAnywhere, Category = "UI")
	UActiveItemWidget* ActiveItemWidget;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	UWeaponWidget* WeaponWidget;

	void SetActiveItem(UTexture2D* ActiveImage, const FString& ActiveName, const FString& ActiveButton, const int32 Cooldown, const int32 MaxCooldown);
	void SetWeaponItem(UTexture2D* MainWeaponImage, UTexture2D* SubWeaponImage, const FString& WeaponName, const int32 Ammo, const int32 SpareAmmo, bool bIsMainWeapon);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
