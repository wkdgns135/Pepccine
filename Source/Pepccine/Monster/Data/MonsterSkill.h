#pragma once

UENUM(BlueprintType)
enum class EMonsterSkill : uint8
{
	None							UMETA(DisplayName = "None"),
	JumpAttack        UMETA(DisplayName = "JumpAttack"),
	Charge						UMETA(DisplayName = "Charge"),
	GunShot				    UMETA(DisplayName = "GunShot")
};
