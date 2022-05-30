#include "GoalGate.h"
#include "Components/BoxComponent.h"
#include "BreakoutMPGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Paddle.h"

AGoalGate::AGoalGate()
{
	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Gate Body Collider"));
	SetRootComponent(HitBox);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate Body Mesh"));
	BodyMesh->SetupAttachment(RootComponent);

	BodyMesh->SetIsReplicated(true);
	bReplicates = true;
}

void AGoalGate::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGoalGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGoalGate::AssumeNewOwner(APaddle* NewOwner)
{
	if (NewOwner)
	{
		OwningPlayer = NewOwner;
	}
}

bool AGoalGate::GotScored_ServerSide_Validate(int32 EnergyAmount)
{
	return true;
}

//Gate finds all gates in the world like a blind mole, and gives score to whoever else
//not going to work properly for 3+ players obviously
void AGoalGate::GotScored_ServerSide_Implementation(int32 EnergyAmount)
{
	if (GetWorld() != nullptr)
	{
		TArray<AActor*> Gates;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGoalGate::StaticClass(), Gates);

		for (AActor* Gt: Gates)
		{
			AGoalGate* NextGate = Cast<AGoalGate>(Gt);

			if (NextGate && NextGate->GetOwningPlayer() != OwningPlayer)
			{
				NextGate->GetOwningPlayer()->UpdateScore_ServerSide(EnergyAmount);
			}
		}
	}
}

