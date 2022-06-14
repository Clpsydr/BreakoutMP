#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakoutBall.generated.h"

class USphereComponent;
class UParticleSystem;
struct FStreamableHandle;
struct TSoftObjectPath;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSoftObjectPtr<UStaticMesh> BallMeshRef;

	//definitely needs an asset manager just to rotate async handles
	TSharedPtr<FStreamableHandle> AssetHandle;
	TSharedPtr<FStreamableHandle> AssetHandle2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSoftObjectPtr<UMaterial> BallMatRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball params")
		float MoveSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball params")
		float AuxSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug params")
		float DebugArrowLength = 30.f;

	// fixed altitude of the ball
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug params")
		float BallGroundCorrection = 72.f;

	// responsible for pushing back the ball movement 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug params")
		int32 RollBackNetcode = 5;

	UPROPERTY(Replicated)
		bool IsMoving = true;

	UPROPERTY(Replicated)
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

	UParticleSystem* HitEffect;

	UMaterial* LoadBallMat();

	UParticleSystem* LoadVFXHard();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void LoadBodyMesh();

	void LoadEffects();

	void OnParticlesLoaded();

	void OnBodyMeshLoaded();

private:
	FSoftObjectPath ParticlePath = TEXT("/Game/Breakout/Effects/E_AltShock.E_AltShock");

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void StartMove();

	UFUNCTION(BlueprintCallable)
		void StopMove();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const;

	int32* GetEnergy() { return &Energy; };
};
