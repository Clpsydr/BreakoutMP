#include "PlayerPawn.h"
#include "Camera/CameraComponent.h"
#include "UIGameStatus.h"
#include "GameFramework/SpringArmComponent.h"

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	bReplicates = true;
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	//Calling in the widget
	if (StatusWidgetClass)
	{
		PlayerWidget = CreateWidget(GetWorld(), StatusWidgetClass);

		PlayerWidget->AddToViewport(0);
	}
}


void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

