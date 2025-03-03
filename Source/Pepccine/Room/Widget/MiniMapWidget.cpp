#include "MiniMapWidget.h"

#include "PepccineGameInstance.h"
#include "PepccineGameState.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Room/RoomManager.h"
#include "Room/Controller/BaseRoomController.h"

void UMiniMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 방 컨트롤러 이벤트 바인딩
	if (APepccineGameState* GameState = Cast<APepccineGameState>(UGameplayStatics::GetGameState(GetWorld())))
	{
		if (ABaseRoomController* RoomController = GameState->GetRoomController())
		{
			RoomController->OnRoomStarted.AddUObject(this, &UMiniMapWidget::UpdateMiniMap);
		}
	}
	RoomManager = Cast<UPepccineGameInstance>(GetGameInstance())->GetRoomManager();

	MaxDepth = 6;
	MaxMiniMapSize = 6;
}

void UMiniMapWidget::UpdateMiniMap()
{
	RefreshRoomIcons();
}

UTexture2D* UMiniMapWidget::GetRoomIconByType(ERoomType RoomType)
{
	switch (RoomType)
	{
	case ERoomType::EBoss: return BossRoomIcon;
	case ERoomType::EItem: return ItemRoomIcon;
	case ERoomType::EShop: return ShopRoomIcon;
	default: return nullptr;
	}
}

void UMiniMapWidget::RefreshRoomIcons()
{
    if (!MiniMapGrid || !RoomManager) return;
    
    MiniMapGrid->ClearChildren();
    
    const FRoomData* CurrentRoomData = RoomManager->GetCurrentRoomData();
    if (!CurrentRoomData) return;
    
    const FIntPoint CurrentRoomCoords = CurrentRoomData->RoomPoint;
    
    const int32 MinX = CurrentRoomCoords.X - MaxMiniMapSize / 2;
    const int32 MaxX = CurrentRoomCoords.X + MaxMiniMapSize / 2;
    const int32 MinY = CurrentRoomCoords.Y - MaxMiniMapSize / 2;
    const int32 MaxY = CurrentRoomCoords.Y + MaxMiniMapSize / 2;
    
    const int32 GridWidth = MaxX - MinX + 1;
    const int32 GridHeight = MaxY - MinY + 1;
    
    MiniMapGrid->SetSlotPadding(FMargin(2.0f));
    MiniMapGrid->SetMinDesiredSlotWidth(50.0f);
    MiniMapGrid->SetMinDesiredSlotHeight(50.0f);
    
    // BFS로 현재 방으로부터 경로 찾기
    TMap<FIntPoint, bool> VisibleRooms;
    FindVisibleRoomsBFS(CurrentRoomCoords, VisibleRooms);
    
    // 그리드 채우기 (정규화된 위치 사용)
    for (int32 Y = 0; Y < GridHeight; Y++)
    {
        for (int32 X = 0; X < GridWidth; X++)
        {
            // 월드 맵 좌표로 변환
            const int32 WorldX = MinX + X;
            const int32 WorldY = MinY + Y;
            
        	if (WorldY >= 0 && WorldY < RoomManager->GetMap().Num() && 
		        WorldX >= 0 && WorldX < RoomManager->GetMap()[WorldY].Num())
            {
                const FRoomData* RoomData = RoomManager->GetMap()[WorldY][WorldX];
                if (RoomData && VisibleRooms.Contains(RoomData->RoomPoint))
                {
                    // 방 배경 추가 (정규화된 그리드 위치 사용)
                    AddRoomBackground(RoomData, FIntPoint(X, Y));
                    
                    // 방 아이콘 추가 (정규화된 그리드 위치 사용)
                    AddRoomIcon(RoomData, FIntPoint(X, Y));
                }
                else
                {
                    AddEmptyIcon(FIntPoint(X, Y));
                }
            }
            else
            {
                AddEmptyIcon(FIntPoint(X, Y));
            }
        }
    }
}


void UMiniMapWidget::FindVisibleRoomsBFS(const FIntPoint& StartRoomCoords, TMap<FIntPoint, bool>& OutVisibleRooms)
{
    TMap<FIntPoint, bool> Visited;
	TQueue<TPair<FIntPoint, int32>> Queue;
    
    Queue.Enqueue(TPair<FIntPoint, int32>(StartRoomCoords, 0));
    Visited.Add(StartRoomCoords, true);
    OutVisibleRooms.Add(StartRoomCoords, true);
    
    const FIntPoint Directions[] = {
        FIntPoint(0, -1), 
        FIntPoint(0, 1),  
        FIntPoint(-1, 0), 
        FIntPoint(1, 0) 
    };
    
    while (!Queue.IsEmpty())
    {
        TPair<FIntPoint, int32> Current;
        Queue.Dequeue(Current);
        
        FIntPoint CurrentPos = Current.Key;
        int32 CurrentDepth = Current.Value;
        
        if (CurrentDepth >= MaxDepth) continue;
        
        for (const FIntPoint& Dir : Directions)
        {
            FIntPoint NextPos = CurrentPos + Dir;
            
            if (NextPos.Y >= 0 && NextPos.Y < RoomManager->GetMap().Num() &&
                NextPos.X >= 0 && NextPos.X < RoomManager->GetMap()[NextPos.Y].Num())
            {
                if (Visited.Contains(NextPos)) continue;
                
                FRoomData* NextRoom = RoomManager->GetMap()[NextPos.Y][NextPos.X];
                if (!NextRoom || NextRoom->RoomType == ERoomType::ENone) continue;
                
                Visited.Add(NextPos, true);
                OutVisibleRooms.Add(NextPos, true);
                Queue.Enqueue(TPair<FIntPoint, int32>(NextPos, CurrentDepth + 1));
            }
        }
    }
}

void UMiniMapWidget::AddRoomIcon(const FRoomData* RoomData, const FIntPoint& RoomPos)
{
	UImage* RoomIcon = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
    
	FSlateBrush Brush;
	Brush.SetImageSize(FVector2D(50, 50));
	RoomIcon->SetBrush(Brush);
    
	UTexture2D* RoomTexture = GetRoomIconByType(RoomData->RoomType);
	if (RoomTexture)
	{
		RoomIcon->SetBrushFromTexture(RoomTexture);
        
		RoomIcon->SetColorAndOpacity(RoomData == RoomManager->GetCurrentRoomData() ? CurrentRoomColor : RoomData->bIsCleared ? ClearedRoomColor : UnclearedRoomColor);
        
		UUniformGridSlot* GridSlot = MiniMapGrid->AddChildToUniformGrid(RoomIcon);
		if (GridSlot)
		{
			GridSlot->SetColumn(RoomPos.X);
			GridSlot->SetRow(RoomPos.Y);
            
			GridSlot->SetHorizontalAlignment(HAlign_Fill);
			GridSlot->SetVerticalAlignment(VAlign_Fill);
		}
	}
}

// 빈 셀 생성 함수
void UMiniMapWidget::AddEmptyIcon(const FIntPoint& RoomPos)
{
	// 빈 이미지 위젯 생성 (회색 배경 또는 투명)
	UImage* EmptyCell = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
    
	FSlateBrush Brush;
	Brush.SetImageSize(FVector2D(50, 50));
	EmptyCell->SetBrush(Brush);
    
	EmptyCell->SetColorAndOpacity(FLinearColor(0,0,0, 0));
	
	UUniformGridSlot* GridSlot = MiniMapGrid->AddChildToUniformGrid(EmptyCell);
	if (GridSlot)
	{
		GridSlot->SetColumn(RoomPos.X);
		GridSlot->SetRow(RoomPos.Y);
        
		GridSlot->SetHorizontalAlignment(HAlign_Fill);
		GridSlot->SetVerticalAlignment(VAlign_Fill);
	}
}


void UMiniMapWidget::AddRoomBackground(const FRoomData* RoomData, const FIntPoint& RoomPos)
{
	UImage* BackgroundIcon = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
    
	FSlateBrush Brush;
	Brush.SetImageSize(FVector2D(50, 50));
	BackgroundIcon->SetBrush(Brush);
    
	FLinearColor BackgroundColor;
    
	if (RoomData == RoomManager->GetCurrentRoomData())
	{
		BackgroundColor = CurrentRoomColor; // 현재 방 색상 (흰색)
	}
	else if (RoomData->bIsCleared)
	{
		BackgroundColor = ClearedRoomColor; // 클리어한 방 색상 (밝은 회색)
	}
	else
	{
		BackgroundColor = UnclearedRoomColor; // 클리어하지 않은 방 색상 (어두운 회색)
	}
    
	BackgroundIcon->SetBrushFromTexture(RoomBackgroundTexture);
	BackgroundIcon->SetColorAndOpacity(BackgroundColor);

	UUniformGridSlot* GridSlot = MiniMapGrid->AddChildToUniformGrid(BackgroundIcon);
	if (GridSlot)
	{
		GridSlot->SetColumn(RoomPos.X);
		GridSlot->SetRow(RoomPos.Y);
        
		GridSlot->SetHorizontalAlignment(HAlign_Fill);
		GridSlot->SetVerticalAlignment(VAlign_Fill);
	}
}