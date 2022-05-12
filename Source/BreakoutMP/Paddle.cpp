#include "Paddle.h"
#include "Components/BoxComponent.h"

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
	
}

void APaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		UE_LOG(LogTemp, Warning, TEXT("Paddle attempting to move on server"));

		FVector CurrentLocation = GetActorLocation();
		FVector NextLocation = GetActorLocation() + GetActorRightVector() * MoveSpeed * AxisValue;

		if (!SetActorLocation(NextLocation, true))
		{
			SetActorLocation(CurrentLocation);
		}
	}
}

