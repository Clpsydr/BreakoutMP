#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BreakoutMPGameModeBase.generated.h"

class APlayerBatController;
class APlayerStart;
class ATargetPoint;

UCLASS()
class BREAKOUTMP_API ABreakoutMPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		APlayerBatController* Player1 = NULL;
	UPROPERTY()
		APlayerBatController* Player2 = NULL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		APlayerStart* P1Start;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		APlayerStart* P2Start;

	//markers for possible gate placement
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ATargetPoint* P1Goals;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ATargetPoint* P2Goals;

public:
	ABreakoutMPGameModeBase();

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
