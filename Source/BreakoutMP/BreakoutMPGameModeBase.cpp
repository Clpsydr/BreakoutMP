#include "BreakoutMPGameModeBase.h"
#include "PlayerBatController.h"
#include "PlayerPawn.h"
//#include "Paddle.h"
#include "BreakoutBall.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

ABreakoutMPGameModeBase::ABreakoutMPGameModeBase()
{
	DefaultPawnClass = APlayerPawn::StaticClass();
	PlayerControllerClass = APlayerBatController::StaticClass();
}

void ABreakoutMPGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void ABreakoutMPGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UWorld* World = GetWorld();
	
	//Searching for existing objects
	if (World && (!P1Start || !P2Start))
	{
		TArray<AActor*> FoundActors;
		TArray<AActor*> FoundGates;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundGates);

		if (FoundActors.Num() > 0)
		{
			P1Start = (APlayerStart*)FoundActors[0];
			P2Goals = (ATargetPoint*)FoundGates[0];  
		}

		if (FoundActors.Num() > 1)
		{
			P2Start = (APlayerStart*)FoundActors[1];
			P1Goals = (ATargetPoint*)FoundGates[1];
		}
	}

	FVector NewSkin;
	APlayerBatController* CurrentPlayer = NULL;
	APlayerStart* StartPosition = NULL;
	ATargetPoint* PlayersGate = NULL;

	//incoming player assigned to the vacant position
	if (Player1 == NULL)
	{
		Player1 = (APlayerBatController*)NewPlayer;
		NewSkin = FVector(0.5f, 0.f, 0.5f);
		CurrentPlayer = Player1;
		StartPosition = P1Start;
		PlayersGate = P1Goals;
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 12.f, FColor::Green, TEXT("Player 1 Initialized"));
	}
	else if (Player2 == NULL)
	{
		Player2 = (APlayerBatController*)NewPlayer;
		NewSkin = FVector(0.5f, 0.5f, 0.f);
		CurrentPlayer = Player2;
		StartPosition = P2Start;
		PlayersGate = P2Goals;
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 12.f, FColor::Green, TEXT("Player 2 Initialized"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red, TEXT("Login fail, two players already in the game"));
		return;
	}

	//further spawning of camera, controller color init
	APlayerPawn* NewPawn = Cast<APlayerPawn>(NewPlayer->GetPawn());
	if (!NewPawn)
	{
		NewPawn = World->SpawnActor<APlayerPawn>(DefaultPawnClass);
	}

	if (StartPosition && NewPawn)
	{
		NewPawn->SetActorLocation(StartPosition->GetActorLocation());
		NewPawn->SetActorRotation(StartPosition->GetActorRotation());

		NewPlayer->SetPawn(NewPawn);

		CurrentPlayer->SetStartTransform(StartPosition->GetActorTransform());
		CurrentPlayer->Initialize(PlayersGate->GetActorLocation(), NewSkin);		// passing color for paddle skin
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red, TEXT("Failed to find any start position"));
	}

	/// /// /// Begin the game 

	if (Player1 && Player2)
	{
		MatchStart(World);
	}
}

void ABreakoutMPGameModeBase::MatchStart(UWorld* WorldToSpawn)
{
	if (WorldToSpawn)
	{
		if (GameBall)
		{
			GameBall->Destroy();
		}

		GameBall = WorldToSpawn->SpawnActor<ABreakoutBall>(BallType);
		GameBall->SetActorLocation(FVector(0.f, 0.f, 72.f));
		FRotator Something = GameBall->GetActorRotation();
		Something.Yaw = Something.Yaw + FMath::RandRange(-90, 90);
		GameBall->SetActorRotation(Something);
	}
}