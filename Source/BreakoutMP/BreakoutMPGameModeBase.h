#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BreakoutMPGameModeBase.generated.h"

class APlayerBatController;
class APlayerStart;
class ATargetPoint;
class ABreakoutBall;

UCLASS()
class BREAKOUTMP_API ABreakoutMPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		APlayerBatController* Player1 = NULL;
	UPROPERTY()
		APlayerBatController* Player2 = NULL;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		APlayerStart* P1Start;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		APlayerStart* P2Start;

	//markers for possible gate placement
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ATargetPoint* P1Goals;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ATargetPoint* P2Goals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game objects")
		TSubclassOf<ABreakoutBall> BallType;

	UPROPERTY()
		ABreakoutBall* GameBall;

public:
	ABreakoutMPGameModeBase();

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	void MatchStart(UWorld* WorldToSpawn);
};
