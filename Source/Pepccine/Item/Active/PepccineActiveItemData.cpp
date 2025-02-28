#include "PepccineActiveItemData.h"

#include "Item/Passive/PepccineStatModifier.h"

void UPepccinePotionItemData::ActivateItem()
{
	Super::ActivateItem();
}

TArray<FPepccineWeaponStatModifier> UPepccinePotionItemData::GetWeaponStatModifiers() const
{
	TArray<FPepccineWeaponStatModifier> Modifiers;
	WeaponStatModifiers.GenerateKeyArray(Modifiers);
	return Modifiers;
}

TArray<FPepccineCharacterStatModifier> UPepccinePotionItemData::GetCharacterStatModifiers() const
{
	TArray<FPepccineCharacterStatModifier> Modifiers;
	CharacterStatModifiers.GenerateKeyArray(Modifiers);
	return Modifiers;
}
