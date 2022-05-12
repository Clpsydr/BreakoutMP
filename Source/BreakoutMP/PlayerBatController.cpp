#include "PlayerBatController.h"
#include "Paddle.h"

APlayerBatController::APlayerBatController()
{
	bReplicates = true;
}

bool APlayerBatController::Initialize_Validate()
{
	return true;
}

void APlayerBatController::Initialize_Implementation()
{
	if (Paddle)
	{
		Paddle->Destroy();
	}
	SpawnPaddle(PaddleClass);
}

void APlayerBatController::SetStartTransform(const FTransform& ActorTransform)
{
	StartPosition = ActorTransform;
}

void APlayerBatController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("Move", this, &APlayerBatController::MoveRight);
}

void APlayerBatController::MoveRight(float AxisValue) 
{
	if (AxisValue != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Client moves paddle"));
	}

	PaddleMoveRight_ServerSide(AxisValue);
}

bool APlayerBatController::PaddleMoveRight_ServerSide_Validate(float AxisValue)
{
	return true;
	// *Selfnote*
	// attempts to validate axis like this will break player spawning
	//return AxisValue != 0;
}

void APlayerBatController::PaddleMoveRight_ServerSide_Implementation(float AxisValue)
{
	if (Paddle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Server moves paddle"));
		Paddle->MoveRight_ServerSide(AxisValue);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR: server cannot find platform to move"));
	}
}

bool APlayerBatController::SpawnPaddle_Validate(TSubclassOf<APaddle> SpawnPaddleClass)
{
	return PaddleClass != NULL;
}

void APlayerBatController::SpawnPaddle_Implementation(TSubclassOf<APaddle> SpawnPaddleClass)
{
	Paddle = (APaddle*)GetWorld()->SpawnActor<APaddle>(PaddleClass);
	if (Paddle)
	{
		Paddle->SetActorLocation(StartPosition.GetLocation());
		Paddle->SetActorRotation(StartPosition.GetRotation());
	}
}

