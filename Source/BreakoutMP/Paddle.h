#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Paddle.generated.h"

class UBoxComponent;
class UUIGameStatus;
class APlayerBatController;

UCLASS()
class BREAKOUTMP_API APaddle : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBoxComponent* HitBox;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MoveSpeed = 100.f;

	UPROPERTY()
		int32 LocalScore;

	UPROPERTY(Replicated)
		FVector TexColor = FVector(1.f, 1.f, 1.f);

public:
	APaddle();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable, WithValidation)
		void MoveRight_ServerSide(float AxisValue);

	UFUNCTION(Server, Reliable)
		void UpdateScore_ServerSide(int32 ExtraScore);

	UFUNCTION(NetMulticast, Reliable)
		void UpdateScore_Multicast(int32 ExtraScore);

	UFUNCTION(BlueprintCallable)
		int32 GetScore() { return LocalScore; };

	UFUNCTION()
		FVector GetPaddleColor() { return TexColor; };

	UFUNCTION(Server, Reliable)
		void UpdateSkin_ServerSide(FVector NewColor);

	UFUNCTION(NetMulticast, Reliable)
		void UpdateSkin_Multicast(FVector NewColor);

	

};
