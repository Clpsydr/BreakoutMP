#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerBatController.generated.h"

class APlayerPawn;
class APaddle;
class AGoalGate;

UCLASS()
class BREAKOUTMP_API APlayerBatController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		FTransform StartPosition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<AGoalGate> GateClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<APaddle> PaddleClass;

	UPROPERTY()
		APaddle* Paddle;

	UPROPERTY()
		AGoalGate* Gate;

public:
	APlayerBatController();

	UFUNCTION(Server, Reliable, WithValidation)
		void Initialize(FVector StartingPoint, FVector PlayerColor);

	UFUNCTION()
		void SetStartTransform(const FTransform& ActorTransform);

	UFUNCTION(Server, Reliable, WithValidation)
		void SpawnPaddle(TSubclassOf<APaddle> SpawnPaddleClass);

	UFUNCTION(Server, Reliable, WithValidation)
		void SpawnGate(TSubclassOf<AGoalGate> SpawnGateClass);

	virtual void SetupInputComponent() override;

	UFUNCTION()
		APaddle* GetPaddle() { return Paddle; };

	UPROPERTY()
		FVector PColor;

protected:
	UFUNCTION()
		void MoveRight(float AxisValue);

	UFUNCTION(Server, Reliable, WithValidation)
		void PaddleMoveRight_ServerSide(float AxisValue);

	UFUNCTION(Server, Reliable)
		void PaintPaddle_ServerSide(FVector NewColor);

}; 
