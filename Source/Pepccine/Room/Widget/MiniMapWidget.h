// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMapWidget.generated.h"

class UUniformGridPanel;
class URoomManager;
enum class ERoomType : uint8;
struct FRoomData;
class UCanvasPanel;
class UGridPanel;
class UImage;

UCLASS()
class PEPCCINE_API UMiniMapWidget : public UUserWidget
{
    GENERATED_BODY()

private:
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* MiniMapGrid;
    
    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* MiniMapCanvas;
    
    UPROPERTY(EditAnywhere, Category = "MiniMap|RoomIcon")
    UTexture2D* BossRoomIcon;
    
    UPROPERTY(EditAnywhere, Category = "MiniMap|RoomIcon")
    UTexture2D* ItemRoomIcon;
    
    UPROPERTY(EditAnywhere, Category = "MiniMap|RoomIocn")
    UTexture2D* ShopRoomIcon;
    
    UPROPERTY(EditDefaultsOnly, Category = "MiniMap|RoomIcon")
    UTexture2D* RoomBackgroundTexture;
    
    UPROPERTY(EditDefaultsOnly, Category = "MiniMap|Background")
    FLinearColor CurrentRoomColor = FLinearColor::White;

    UPROPERTY(EditDefaultsOnly, Category = "MiniMap|Background")
    FLinearColor ClearedRoomColor = FLinearColor(0.8f, 0.8f, 0.8f, 1.f);

    UPROPERTY(EditDefaultsOnly, Category = "MiniMap|Background")
    FLinearColor UnclearedRoomColor = FLinearColor(0.3f, 0.3f, 0.3f, 1.f);
    
    UPROPERTY(EditAnywhere, Category = "MiniMap")
    int MaxDepth = 5;

    UPROPERTY(EditAnywhere, Category = "MiniMap")
    int MaxMiniMapSize = 5;
    
    URoomManager* RoomManager;

public:
    UFUNCTION(BlueprintCallable, Category = "MiniMap")
    void UpdateMiniMap();

protected:
    virtual void NativeConstruct() override;

private:
    void RefreshRoomIcons();
    void FindVisibleRoomsBFS(const FIntPoint& StartRoomCoords, TMap<FIntPoint, bool>& OutVisibleRooms);
    UTexture2D* GetRoomIconByType(ERoomType RoomType);
    void AddRoomIcon(const FRoomData* RoomData, const FIntPoint& RoomPos);
    void AddEmptyIcon(const FIntPoint& RoomPos);
    void AddRoomBackground(const FRoomData* RoomData, const FIntPoint& RoomPos);
};
