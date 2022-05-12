#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Paddle.generated.h"

class UBoxComponent;

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

public:
	APaddle();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable, WithValidation)
		void MoveRight_ServerSide(float AxisValue);

};
