#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakoutBall.generated.h"

class USphereComponent;
class UParticleSystem;

UCLASS()
class BREAKOUTMP_API ABreakoutBall : public AActor
{
	GENERATED_BODY()
	
public:	
	ABreakoutBall();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USphereComponent* HitBox;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BallMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball params")
		float MoveSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball params")
		float AuxSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug params")
		float DebugArrowLength = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug params")
		float BallGroundCorrection = 72.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug params")
		int AngleRandomisation = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball params")
		UParticleSystem* HitEffect;

	UPROPERTY(Replicated)
		bool IsMoving = true;

		int32 Energy = 0;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
		void Move_ServerSide(float DeltaTime);

	UFUNCTION(Server, Reliable, WithValidation)
		void StartMove_ServerSide();

	UFUNCTION(Server, Reliable, WithValidation)
		void StopMove_ServerSide();

	UFUNCTION(NetMulticast, Unreliable)
		void Multicast_HitEffect();

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void StartMove();

	UFUNCTION(BlueprintCallable)
		void StopMove();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const;
};
