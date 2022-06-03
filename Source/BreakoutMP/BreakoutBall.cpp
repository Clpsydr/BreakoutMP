#include "BreakoutBall.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "GoalGate.h"
#include "Paddle.h"
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
	DOREPLIFETIME(ABreakoutBall, Energy);
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
			
	// Gate conditional collision and position reset
		AGoalGate* SuspectedGate = Cast<AGoalGate>(HitResult.GetActor());
	
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
	//

	// Stepping back from collising for RollBackNetcode amount of pixels
	// Backwards vector fixed by finding difference between new and old positions instead of using frontal vector
		FVector ReverseVector = CurrentLoc - NewLoc;
		ReverseVector.Normalize();
		FVector ResetPosition = CurrentLoc + ReverseVector * DeltaTime * MoveSpeed * RollBackNetcode; 
		DrawDebugDirectionalArrow(GetWorld(), CurrentLoc + ReverseVector * AuxSpeed, CurrentLoc, 
								DebugArrowLength, 
								FColor::Yellow,	false, 3.f, 0, 5.f);
	//

	// Vector of force perpendicular to the hit surface
		FVector ImpactCorrection = HitResult.ImpactPoint + HitResult.ImpactNormal * AuxSpeed;
		DrawDebugDirectionalArrow(GetWorld(), HitResult.ImpactPoint, ImpactCorrection,
								DebugArrowLength, 
								FColor::Orange,	false, 3.f, 0, 5.f);
	//

	// New direction from angle between incoming vector and surface normal 
		float AimAtAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(ReverseVector, HitResult.ImpactNormal)));

		// Rotation occurs through blind method of checking if such rotation even reasonable, 
		// ie dot product of result isnt behind the contact surface
		// would be better if I just figured out the angle to surface
		FVector MoveVector;
		if (FVector::DotProduct(ReverseVector.RotateAngleAxis(AimAtAngle * 2, FVector(0, 0, 1)), HitResult.ImpactNormal) > 0)
		{
			MoveVector = ReverseVector.RotateAngleAxis(AimAtAngle * 2, FVector(0, 0, 1));
		}
		else
		{
			MoveVector = ReverseVector.RotateAngleAxis(-AimAtAngle * 2, FVector(0, 0, 1));
		}

		FVector NewDirection = NewLoc + MoveVector * MoveSpeed;  //* AuxSpeed;
		NewDirection.Z = CurrentLoc.Z;
		

		DrawDebugDirectionalArrow(GetWorld(), NewLoc, NewDirection, 
								DebugArrowLength, 
								FColor::Cyan, false, 3.f, 0, 5.f);
	//
	
	// Direction/position correction
		ResetPosition.Z = BallGroundCorrection;
		SetActorLocation(ResetPosition);		
		FRotator CurrentRotation = GetActorRotation();
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLoc, NewDirection);

	// Angular correction
		NewRotation.Pitch = 0.f;

		// Additional angle tweak relative to paddle movement
		APaddle* MaybePaddle = Cast<APaddle>(HitResult.GetActor());
		if (MaybePaddle)
		{
			NewRotation.Yaw = NewRotation.Yaw + MaybePaddle->GetDirection() * 20;; //+FMath::RandRange(-10, 10); 
		}
		SetActorRotation(NewRotation);

	// Clientside effects
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


