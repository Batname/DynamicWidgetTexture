// Fill out your copyright notice in the Description page of Project Settings.

#include "DynamicActor.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"


// Sets default values
ADynamicActor::ADynamicActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("StaticMesh'/Game/TopDownCPP/Blueprints/PlaneMesh.PlaneMesh'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create static mesh component
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());	// Set static mesh
	RootComponent = PlaneMesh;


	// Set Material
	static ConstructorHelpers::FObjectFinder<UMaterial> TexMatObj(TEXT("Material'/Game/TopDownCPP/Blueprints/DynamicMat.DynamicMat'"));
	if (TexMatObj.Succeeded())
	{
		PlaneMesh->SetMaterial(0, TexMatObj.Object);
	}

	// dynamic texture properties (hard wired here for now)
	dtBytesPerPixel = 4;
	dtWidth = 800;
	dtHeight = 600;

	dtUpdateTextureRegion = FUpdateTextureRegion2D(0, 0, 0, 0, dtWidth, dtHeight);
	dtBufferSizeSqrt = dtWidth * dtBytesPerPixel;
	dtBufferSize = dtWidth * dtHeight * dtBytesPerPixel;
}

// Called when the game starts or when spawned
void ADynamicActor::BeginPlay()
{
	Super::BeginPlay();

	dtBuffer = new uint8[dtBufferSize];

	SourceTex = UTexture2D::CreateTransient(800, 600);
	SourceTex->UpdateResource();


	dtMaterialInstanceDynamic = PlaneMesh->CreateAndSetMaterialInstanceDynamic(0);
	dtMaterialInstanceDynamic->SetTextureParameterValue(FName("BaseTex"), SourceTex);

	FColor Color(10, 10, 10, 1);
	for (int i = 0; i < 800 * 600; i++)
	{
		Colors.Add(Color);
	}

}

void ADynamicActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	delete[] dtBuffer;
	dtBuffer = nullptr;
}

// Called every frame
void ADynamicActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (dtBuffer)
	{
		UpdateTexture();
	}
}

void ADynamicActor::UpdateTexture()
{
	// Set a squares width and height
	float squareW = dtWidth / 10;
	float squareH = dtHeight / 10;

	// Choose a random position for it
	float randomX = rnd() * (dtWidth - (dtWidth / 10));
	float randomY = rnd() * (dtHeight - (dtHeight / 10));

	// Redraw the texture with a black background and a randomly positioned red square
	int pixelCount = dtBufferSize / dtBytesPerPixel;
	for (int i = 0; i < pixelCount; i++) { // > delete this comment, it's workaround for a STUPID wordpress bug

										   // figure out index for current pixels color components 
		int iBlue = i * 4 + 0;
		int iGreen = i * 4 + 1;
		int iRed = i * 4 + 2;
		int iAlpha = i * 4 + 3;

		// figure out the X,Y position (relative to dtWidth & dtHeight) of the current pixel 
		int iX = i % dtWidth;
		int iY = i / dtWidth;

		// if the current pixel is inside the randomly chosen position for the square, make it red 
		if (iX > randomX && iX < randomX + squareW && iY > randomY && iY < randomY + squareH) { // > another fixer
			dtBuffer[iBlue] = 0;
			dtBuffer[iGreen] = 0;
			dtBuffer[iRed] = 255;
			dtBuffer[iAlpha] = 255;
		}
		// otherwise, just make it black 
		else {
			dtBuffer[iBlue] = 0;
			dtBuffer[iGreen] = 0;
			dtBuffer[iRed] = 0;
			dtBuffer[iAlpha] = 255;
		}
	}
	UpdateTextureRegions(SourceTex, 0, 1, &dtUpdateTextureRegion, dtBufferSizeSqrt, (uint32)4, dtBuffer, false);
}

double ADynamicActor::rnd()
{
	return double(rand()) / RAND_MAX;
}

// https://wiki.unrealengine.com/Dynamic_Textures
void ADynamicActor::UpdateTextureRegions(UTexture2D * Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D * Regions, uint32 SrcPitch, uint32 SrcBpp, uint8 * SrcData, bool bFreeData)
{
	if (Texture && Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			UpdateTextureRegionsData,
			FUpdateTextureRegionsData*, RegionData, RegionData,
			bool, bFreeData, bFreeData,
			{
				for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
				{
					int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
					if (RegionData->MipIndex >= CurrentFirstMip)
					{
						RHIUpdateTexture2D(
							RegionData->Texture2DResource->GetTexture2DRHI(),
							RegionData->MipIndex - CurrentFirstMip,
							RegionData->Regions[RegionIndex],
							RegionData->SrcPitch,
							RegionData->SrcData
							+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
							+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
						);
					}
				}

		if (bFreeData)
		{
			FMemory::Free(RegionData->Regions);
			FMemory::Free(RegionData->SrcData);
		}
		delete RegionData;
			});
	}
}

