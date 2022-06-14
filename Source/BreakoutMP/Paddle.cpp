#include "Paddle.h"
#include "Components/BoxComponent.h"
#include "PlayerBatController.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

APaddle::APaddle()
{
	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Platform Body Collider"));
	SetRootComponent(HitBox);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Body Mesh"));
	BodyMesh->SetupAttachment(RootComponent);
	
	BodyMesh->SetIsReplicated(true);

	bReplicates = true;
	SetReplicateMovement(true);
}

void APaddle::BeginPlay()
{
	Super::BeginPlay();

	LoadBodyMesh();
}

void APaddle::LoadBodyMesh()
{
	FStreamableDelegate LoadMeshDelegate;
	LoadMeshDelegate.BindUObject(this, &APaddle::OnBodyMeshLoaded);

	UAssetManager& AssetManager = UAssetManager::Get();
	FStreamableManager& StreamableManager = AssetManager.GetStreamableManager();

	AssetHandle = StreamableManager.RequestAsyncLoad(PaddleMeshRef.ToStringReference(), LoadMeshDelegate);
}

void APaddle::OnBodyMeshLoaded()
{
	UStaticMesh* LoadedMesh = Cast<UStaticMesh>(AssetHandle.Get()->GetLoadedAsset());
	if (LoadedMesh)
	{
		BodyMesh->SetStaticMesh(LoadedMesh);
		BodyMesh->SetMaterial(0, PaddleMaterial);
		UpdateSkin_Multicast(TexColor);
	}
}

void APaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APaddle::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APaddle, TexColor);
}

bool APaddle::MoveRight_ServerSide_Validate(float AxisValue)
{
	return true;
	// *Selfnote* 
	// Attempting to validate paddle movement on this stage will completely break multiplayer spawning
	//	return AxisValue != 0.f;			
}

void APaddle::MoveRight_ServerSide_Implementation(float AxisValue)
{
	if (AxisValue != 0.f)
	{
		LocalAxis = AxisValue;
		UE_LOG(LogTemp, Warning, TEXT("Paddle attempting to move on server"));

		FVector CurrentLocation = GetActorLocation();
		FVector NextLocation = GetActorLocation() + GetActorRightVector() * MoveSpeed * AxisValue;

		if (!SetActorLocation(NextLocation, true))
		{
			SetActorLocation(CurrentLocation);
		}
	}
	else
	{
		LocalAxis = 0;
	}
}

void APaddle::UpdateScore_ServerSide_Implementation(int32 ExtraScore)
{
	LocalScore += ExtraScore;
	UpdateScore_Multicast(LocalScore);
}

void APaddle::UpdateScore_Multicast_Implementation(int32 ExtraScore)
{
	LocalScore = ExtraScore;
}

void APaddle::UpdateSkin_ServerSide_Implementation(FVector NewColor)
{
	TexColor = NewColor;
	UpdateSkin_Multicast(NewColor);
}

void APaddle::UpdateSkin_Multicast_Implementation(FVector NewColor)
{
	TexColor = NewColor;
	if (BodyMesh)
	{
		BodyMesh->SetVectorParameterValueOnMaterials("ColorParam", NewColor);
	}
}