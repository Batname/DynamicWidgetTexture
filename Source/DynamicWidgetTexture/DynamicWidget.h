// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DynamicWidget.generated.h"

/**
 * 
 */
UCLASS()
class DYNAMICWIDGETTEXTURE_API UDynamicWidget : public UUserWidget
{
	GENERATED_BODY()

	UDynamicWidget(const FObjectInitializer& ObjectInitializer);
	
	
protected:
	void NativeConstruct() override;
	
	class UTexture2D* DestTex;
};
