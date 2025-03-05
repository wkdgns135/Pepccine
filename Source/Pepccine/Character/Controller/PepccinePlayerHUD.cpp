// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Controller/PepccinePlayerHUD.h"

#include "Room/Widget/MiniMapWidget.h"

void APepccinePlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MiniMapWidgetClass)
	{
		MiniMapWidgetInstance = CreateWidget<UMiniMapWidget>(GetOwningPlayerController(), MiniMapWidgetClass);
		if (MiniMapWidgetInstance)
		{
			MiniMapWidgetInstance->AddToViewport();
		}
	}
}
