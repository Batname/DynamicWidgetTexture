// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/RHI/Public/RHI.h"
#include "DynamicActor.generated.h"

UCLASS()
class DYNAMICWIDGETTEXTURE_API ADynamicActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADynamicActor();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "DynamicTexture")
	class UStaticMeshComponent* PlaneMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game starts or when spawned
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DynamicTexture")
	class UTexture2D* SourceTex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DynamicTexture")
	class UMaterialInstanceDynamic* dtMaterialInstanceDynamic;

	TArray<FColor> Colors;

// Update Texture
public:
	FUpdateTextureRegion2D dtUpdateTextureRegion;

	// dTex res
	int dtWidth;
	int dtHeight;
	int dtBytesPerPixel;

	// dTex buffer
	uint8* dtBuffer = nullptr;
	int dtBufferSize;
	int dtBufferSizeSqrt;


	// Quick random function for testing
	double rnd();

	void UpdateTexture();


	void UpdateTextureRegions(class UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData);
};
