// Fill out your copyright notice in the Description page of Project Settings.

#include "DynamicWidget.h"

#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"

UDynamicWidget::UDynamicWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> DestTex2DObj(TEXT("Texture2D'/Game/TopDownCPP/Blueprints/L.L'"));
	if (DestTex2DObj.Succeeded())
	{
		DestTex = DestTex2DObj.Object;
	}

}

void UDynamicWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("UDynamicWidget::NativeConstruct DestTex %p"), DestTex);


}