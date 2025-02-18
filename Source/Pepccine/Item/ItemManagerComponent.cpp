#include "ItemManagerComponent.h"
#include "Data/ItemDataAssetBase.h"

UItemManagerComponent::UItemManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UItemManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UItemManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UItemManagerComponent::PrintWeaponItemData(int32 Index)
{
	if (ItemDataAsset && Index < ItemDataAsset->Weapons.Num())
	{
		const FWeaponItemData& Weapon = ItemDataAsset->Weapons[Index];

		UE_LOG(LogTemp, Warning, TEXT("Weapon Name : %s / Weapon Description : %s / Weapon SellingPrice : %d"), *Weapon.Info.DisplayName, *Weapon.Info.Description, Weapon.Info.SellingPrice);
	}
}

