#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoalGate.generated.h"

class UBoxComponent;
class APaddle;

UCLASS()
class BREAKOUTMP_API AGoalGate : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBoxComponent* HitBox;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMesh;

	UPROPERTY()
		APaddle* OwningPlayer; 
	
public:	
	AGoalGate();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void AssumeNewOwner(APaddle* NewOwner);

	APaddle* GetOwningPlayer() { return OwningPlayer; };

	UFUNCTION(Server, Reliable, WithValidation)
		void GotScored_ServerSide(int32 EnergyAmount);
};
