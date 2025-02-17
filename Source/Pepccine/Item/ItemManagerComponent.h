#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemManagerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UItemManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UItemManagerComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UDataTable* ItemStatDataTable;
};
