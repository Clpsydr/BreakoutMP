#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIGameStatus.generated.h"

class UTextBlock;
class APaddle;
class UBorder;

UCLASS()
class BREAKOUTMP_API UUIGameStatus : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* P1ScoreDisplay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* P2ScoreDisplay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UBorder* P1Affinity;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UBorder* P2Affinity;

	APaddle* ScoreSource1;

	APaddle* ScoreSource2;

	UFUNCTION()
		void AddPaddleLinks();

public:
	virtual void NativeConstruct() override;

	virtual bool Initialize() override;

	UFUNCTION()
		FText SetNewScore1();
	
	UFUNCTION()
		FText SetNewScore2();
	
};
