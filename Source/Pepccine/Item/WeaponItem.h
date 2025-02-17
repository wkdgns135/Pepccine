#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemDataRow.h"
#include "WeaponItem.generated.h"

UCLASS()
class PEPCCINE_API UWeaponItem : public UItemBase
{
	GENERATED_BODY()

public:
	FItemDataRow* Stat;
};
