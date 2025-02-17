#include "ItemBase.h"

UItemBase::UItemBase()
{
}

FName UItemBase::GetItemName() const
{
	return Name;
}

FString UItemBase::GetItemDescription() const
{
	return Description;
}

int32 UItemBase::GetItemSellingPrice() const
{
	return SellingPrice;
}

