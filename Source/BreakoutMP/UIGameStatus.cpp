#include "UIGameStatus.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Containers/UnrealString.h"
#include "Kismet/GameplayStatics.h"
#include "Paddle.h"

//The whole class is a mortal sin, but it works and I'm already behind all deadlines, so-
void UUIGameStatus::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UUIGameStatus::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	AddPaddleLinks();

	if (P1ScoreDisplay)
	{
		P1ScoreDisplay->TextDelegate.BindUFunction(this, "SetNewScore1");
		
	}

	if (P2ScoreDisplay)
	{
		P2ScoreDisplay->TextDelegate.BindUFunction(this, "SetNewScore2");
	}

	return true;
}

FText UUIGameStatus::SetNewScore1()
{
	if (ScoreSource1)
	{
		return FText::FromString(FString::FromInt(ScoreSource1->GetScore()));
	}
	else
	{
		AddPaddleLinks();
		return FText::FromString("no p1 paddle!");
	}
}

FText UUIGameStatus::SetNewScore2()
{
	if (ScoreSource2)
	{
		return FText::FromString(FString::FromInt(ScoreSource2->GetScore()));
	}
	else
	{
		AddPaddleLinks();
		return FText::FromString("no p2 paddle!");
	}
}

//World search for paddles, thankfully it doesnt happen once all paddles are found, but could still be better
void UUIGameStatus::AddPaddleLinks()
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		TArray<AActor*> Paddles;
		UGameplayStatics::GetAllActorsOfClass(World, APaddle::StaticClass(), Paddles);

		if (Paddles.Num() > 0 && Cast<APaddle>(Paddles[0]))
		{
			ScoreSource1 = Cast<APaddle>(Paddles[0]);

			if (P1Affinity)
			{
				FLinearColor NewColor = ScoreSource1->GetPaddleColor();
				NewColor.A = 1.f;

				P1Affinity->SetBrushColor(NewColor);
			}
		}

		if (Paddles.Num() > 1 && Cast<APaddle>(Paddles[1]))
		{
			ScoreSource2 = Cast<APaddle>(Paddles[1]);

			if (P2Affinity)
			{
				FLinearColor NewColor = ScoreSource2->GetPaddleColor();
				NewColor.A = 1.f;

				P2Affinity->SetBrushColor(NewColor);
			}
		}
	}
}