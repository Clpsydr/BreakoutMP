#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerBatController.generated.h"

class APlayerPawn;
class APaddle;

UCLASS()
class BREAKOUTMP_API APlayerBatController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		FTransform StartPosition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<APaddle> PaddleClass;

	UPROPERTY()
		APaddle* Paddle;

public:
	APlayerBatController();

	UFUNCTION(Server, Reliable, WithValidation)
		void Initialize();

	UFUNCTION()
		void SetStartTransform(const FTransform& ActorTransform);

	UFUNCTION(Server, Reliable, WithValidation)
		void SpawnPaddle(TSubclassOf<APaddle> SpawnPaddleClass);

	virtual void SetupInputComponent() override;

protected:
	UFUNCTION()
		void MoveRight(float AxisValue);

	UFUNCTION(Server, Reliable, WithValidation)
		void PaddleMoveRight_ServerSide(float AxisValue);
}; 
