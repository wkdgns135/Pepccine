#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemManagerComponent.generated.h"

class UItemDataAssetBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UItemManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UItemManagerComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UItemDataAssetBase* ItemDataAsset;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void PrintWeaponItemData(int32 Index);
	
};
