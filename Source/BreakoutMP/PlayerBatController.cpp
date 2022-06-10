#include "PlayerBatController.h"
#include "Paddle.h"
#include "GoalGate.h"

APlayerBatController::APlayerBatController()
{
	bReplicates = true;
}

bool APlayerBatController::Initialize_Validate(FVector StartingPoint, FVector PlayerColor)
{
	return true;
}

void APlayerBatController::Initialize_Implementation(FVector StartingPoint, FVector PlayerColor)
{
	PColor = PlayerColor;

	if (Paddle)
	{
		Paddle->Destroy();
	}
	SpawnPaddle(PaddleClass);

	if (Gate)
	{
		Gate->Destroy();
	}
	SpawnGate(GateClass);
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
	//return AxisValue != 0.f;
}

void APlayerBatController::PaddleMoveRight_ServerSide_Implementation(float AxisValue)
{
	if (Paddle)
	{
		if (AxisValue != 0.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Server moves paddle"));
		}
		
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
	Paddle = (APaddle*)GetWorld()->SpawnActorDeferred<APaddle>(PaddleClass, StartPosition, this, nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	Paddle->FinishSpawning(StartPosition);

	PaintPaddle_ServerSide(PColor);
}

bool APlayerBatController::SpawnGate_Validate(TSubclassOf<AGoalGate> SpawnGateClass)
{
	return GateClass != NULL;
}

void APlayerBatController::SpawnGate_Implementation(TSubclassOf<AGoalGate> SpawnGateClass)
{
	Gate = (AGoalGate*)GetWorld()->SpawnActor<AGoalGate>(SpawnGateClass);
	if (Gate && Paddle)
	{
		Gate->SetActorLocation(StartPosition.GetLocation() - Paddle->GetActorForwardVector() * 430.f);
		Gate->SetActorRotation(StartPosition.GetRotation());
		Gate->AssumeNewOwner(Paddle);
	}
}

void APlayerBatController::PaintPaddle_ServerSide_Implementation(FVector NewColor)
{
	if (Paddle)
	{
		Paddle->UpdateSkin_ServerSide(NewColor);
	}
}
