#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BreakoutMPGameModeBase.generated.h"

class APlayerBatController;
class APlayerStart;

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

public:
	ABreakoutMPGameModeBase();

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;


};
