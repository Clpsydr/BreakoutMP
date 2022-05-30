#include "BreakoutBall.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "GoalGate.h"
#include "BreakoutMPGameModeBase.h"
#include "Net/UnrealNetwork.h"

ABreakoutBall::ABreakoutBall()
{
	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<USphereComponent>(TEXT("Ball Hitbox"));
	SetRootComponent(HitBox);

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball BodyMesh"));
	BallMesh->SetupAttachment(RootComponent);

	BallMesh->SetIsReplicated(true);
	bReplicates = true;
	SetReplicateMovement(true);
}

void ABreakoutBall::BeginPlay()
{
	Super::BeginPlay();
	Energy = 1;
}

void ABreakoutBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetNetMode() != ENetMode::NM_Client)
	{
		Move_ServerSide(DeltaTime);
	}
}

void ABreakoutBall::StartMove()
{
	StartMove_ServerSide();
}

void ABreakoutBall::StopMove()
{
	StopMove_ServerSide();
}

void ABreakoutBall::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABreakoutBall, IsMoving);
	//DOREPLIFETIME_CONDITION(APingPongBall, IsMoving, COND_SimulatedOnly);
}

bool ABreakoutBall::Move_ServerSide_Validate(float DeltaTime)
{
	return true;
}

void ABreakoutBall::Move_ServerSide_Implementation(float DeltaTime)
{
	FVector VForward = GetActorForwardVector();
	FVector CurrentLoc = GetActorLocation();
	FVector NewLoc = CurrentLoc + VForward * MoveSpeed * DeltaTime;
	FHitResult HitResult;

	if (!SetActorLocation(NewLoc, true, &HitResult))
	{
		
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 12.f, FColor::Yellow,
			TEXT("Ball" + GetName() + "collided with" + HitResult.GetActor()->GetName()));
		
		AGoalGate* SuspectedGate = Cast<AGoalGate>(HitResult.GetActor());
	// Gate conditional collision and position reset
		if (SuspectedGate) 
		{
			SuspectedGate->GotScored_ServerSide(Energy);
			Energy = 1;

			CurrentLoc = FVector(0.f, 0.f, BallGroundCorrection);
			NewLoc = CurrentLoc + VForward * MoveSpeed * DeltaTime;
		}
		else
		{
			Energy++;
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Yellow,
				TEXT("Energy amplifies: " + FString::FromInt(Energy)));  
		}


	// Current direction
		FVector MoveVector = VForward - CurrentLoc;
		MoveVector.Normalize();
		FVector ResetPosition = CurrentLoc + MoveVector * DeltaTime * MoveSpeed * 5;  //????
		DrawDebugDirectionalArrow(GetWorld(), NewLoc + MoveVector * AuxSpeed, NewLoc, 
								DebugArrowLength, 
								FColor::Yellow,	false, 3.f, 0, 5.f);
	//

	// Fixing related to the impact point
		FVector ImpactCorrection = HitResult.ImpactPoint + HitResult.ImpactNormal * AuxSpeed;
		DrawDebugDirectionalArrow(GetWorld(), HitResult.ImpactPoint, 
								HitResult.ImpactPoint + HitResult.ImpactNormal * AuxSpeed,
								DebugArrowLength, 
								FColor::Orange,	false, 3.f, 0, 5.f);
	//

	// New direction after turning
		float AimAtAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(MoveVector, HitResult.ImpactNormal)));
		MoveVector = MoveVector.RotateAngleAxis(AimAtAngle * 2, FVector(0, 0, 1));
		FVector NewTargetMove = NewLoc + MoveVector * AuxSpeed;
		NewTargetMove.Z = CurrentLoc.Z;
		DrawDebugDirectionalArrow(GetWorld(), NewLoc, NewTargetMove, 
								DebugArrowLength, 
								FColor::Cyan, false, 3.f, 0, 5.f);
	//
	
	// Direction/position correction
		ResetPosition.Z = BallGroundCorrection;
		SetActorLocation(ResetPosition);		
		FRotator CurrentRotation = GetActorRotation();
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLoc, NewTargetMove);

	// Angular correction
		NewRotation.Pitch = 0.f;
		NewRotation.Yaw = NewRotation.Yaw + FMath::RandRange(-AngleRandomisation, AngleRandomisation);
		SetActorRotation(NewRotation);

	// Bells&Whistles
		Multicast_HitEffect();
	}
}

bool ABreakoutBall::StartMove_ServerSide_Validate()
{
	return true;
}

void ABreakoutBall::StartMove_ServerSide_Implementation()
{
	IsMoving = true;
}

bool ABreakoutBall::StopMove_ServerSide_Validate()
{
	return true;
}

void ABreakoutBall::StopMove_ServerSide_Implementation()
{
	IsMoving = false;
}

void ABreakoutBall::Multicast_HitEffect_Implementation()
{
	UWorld* World = GetWorld();
	if (World && HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetActorLocation());
	}
}


