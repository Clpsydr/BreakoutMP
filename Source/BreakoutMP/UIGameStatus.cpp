#include "UIGameStatus.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Containers/UnrealString.h"
#include "Kismet/GameplayStatics.h"
#include "Paddle.h"
#include "BreakoutBall.h"

//WaitingMsg border
//Blinkin animation

//The whole class is a mortal sin, but it works and I'm already behind all deadlines, so-
void UUIGameStatus::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UUIGameStatus::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	//^TODO:  I have no virtual idea, why running playanimation on client makes server collapse instantly
	/*
	if (FlickerAnimation)
	{
		PlayAnimationForward(FlickerAnimation, 1.f, false);
		PlayAnimation(FlickerAnimation, 0.f, 5, EUMGSequencePlayMode::PingPong, false);
	}
	*/

	AddPaddleLinks();

	if (P1ScoreDisplay)
	{
		P1ScoreDisplay->TextDelegate.BindUFunction(this, "SetNewScore1");
	}

	if (P2ScoreDisplay)
	{
		P2ScoreDisplay->TextDelegate.BindUFunction(this, "SetNewScore2");
	}

	if (EnergyDisplay)
	{
		EnergyDisplay->TextDelegate.BindUFunction(this, "SetEnergy");
	}

	return true;
}

void UUIGameStatus::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (WaitingMSG)
	{
		if (WaitingMSG->Visibility == ESlateVisibility::Visible && ScoreSource1 && ScoreSource2)
		{
			WaitingMSG->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
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

FText UUIGameStatus::SetEnergy()
{
	if (EnergySource != nullptr)
	{
		return FText::FromString(FString::FromInt(*EnergySource));
	}
	else
	{
		AddEnergyLink();
		return FText::FromString("no ball!");
	}
	return FText::FromString("no will");
}

void UUIGameStatus::AddEnergyLink()
{
	TArray<AActor*> Balls;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABreakoutBall::StaticClass(), Balls);
	if (Balls.Num() > 0 && Cast<ABreakoutBall>(Balls[0]))
	{
		EnergySource = Cast<ABreakoutBall>(Balls[0])->GetEnergy();
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
