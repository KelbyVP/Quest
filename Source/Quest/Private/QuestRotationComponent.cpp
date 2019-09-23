// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestRotationComponent.h"
#include "GameFramework/Pawn.h"
#include "Math/Vector.h"
#include "QuestCharacterBase.h"
#include "QuestGlobalTags.h"


// Sets default values for this component's properties
UQuestRotationComponent::UQuestRotationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UQuestRotationComponent::BeginPlay()
{
	Super::BeginPlay();
	AQuestCharacterBase* QuestCharacter = Cast<AQuestCharacterBase>(GetOwner());
	if (QuestCharacter)
	{
		QuestCharacter->OnDeath.AddDynamic(this, &UQuestRotationComponent::OnCharacterDeath);
	}
	SetComponentTickEnabled(false);
}


// Called every frame
void UQuestRotationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!bHasStartingRotationPosition)
	{
		GetStartingRotationPosition();
	}
	if (bShouldMoveToStartingPosition)
	{
		MoveActorToStartingRotationPosition(DeltaTime);
	}
	if (bShouldRotate)
	{
		CurrentRotation = (RotationSpeed * DeltaTime * ClockwiseFloat) + CurrentRotation;
		RotateActorAroundPoint();
	}
}

void UQuestRotationComponent::StartRotating(float InRadius, float InMovementSpeedToStartingRotatingPosition,
	float InRotationSpeed, float InRotationHeight, bool bInClockwise, AActor* InActorAtCenter)
{
	Radius = InRadius;
	MovementSpeedToStartingRotationPosition = InMovementSpeedToStartingRotatingPosition;
	RotationSpeed = InRotationSpeed;
	RotationHeight = InRotationHeight;
	bClockwise = bInClockwise;
	ActorAtCenter = InActorAtCenter;
	InitialLocation = GetOwner()->GetActorLocation();
	ClockwiseFloat = (bClockwise) ? 1 : -1;
	CharacterToRotate = GetOwner();

	// Allow component to start ticking so rotation can function
	SetComponentTickEnabled(true);

	AQuestCharacterBase* QuestCharacterToRotate = Cast<AQuestCharacterBase>(CharacterToRotate);
	if (IsValid(QuestCharacterToRotate))
	{
		QuestCharacterToRotate->AddGameplayTag(UQuestGlobalTags::Status_Spinning());
	}
}

void UQuestRotationComponent::RotateActorAroundPoint()
{
	bShouldRotate = true;
	if (ActorAtCenter && CharacterToRotate)
	{
		// Rotate the character around the center axis
		FVector CenterCharacterLocation = ActorAtCenter->GetActorLocation();
		CenterCharacterLocation.Z = RotationHeight;
		FVector RotatedVectorAroundAxis = FVector(Radius, 0, 0);
		CharacterToRotate->SetActorLocation(CenterCharacterLocation + RotatedVectorAroundAxis.RotateAngleAxis((StartingAngle + CurrentRotation), FVector(0, 0, 1)));

		// Rotate the character around its own axis
		FRotator RotatedCharacterRotator = CharacterToRotate->GetActorRotation();
		RotatedCharacterRotator.Yaw = (StartingAngle - 180 + CurrentRotation);
		CharacterToRotate->SetActorRotation(RotatedCharacterRotator);
	}
	return;
}

void UQuestRotationComponent::MoveActorToStartingRotationPosition(float DeltaTime)
{

		// Moving to starting position is handled in Blueprint so that we can LerpToLocation
		AQuestCharacterBase* QuestCharacter = Cast<AQuestCharacterBase>(GetOwner());
		if (IsValid(QuestCharacter))
		{
			QuestCharacter->BP_MoveToStartPositionForRotationActor(StartingRotationPosition);
		}
		bShouldMoveToStartingPosition = false;
}

void UQuestRotationComponent::GetStartingRotationPosition()
{
	if (ActorAtCenter && CharacterToRotate)
	{
		// Adjust rotation height based on the fact that the character's Z position is different than the center's Z position
		RotationHeight = RotationHeight + (CharacterToRotate->GetActorLocation().Z);
		FVector CharacterLocation = CharacterToRotate->GetActorLocation();
		FVector CenterLocation = ActorAtCenter->GetActorLocation();
		CenterLocation.Z = CharacterLocation.Z;


		// Get the starting angle for rotation
		FVector NormalCharacter = CharacterLocation.GetSafeNormal();
		FVector NormalCenter = CenterLocation.GetSafeNormal();
		StartingAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(NormalCenter, NormalCharacter))) + 180;

		// Set the starting position based on a tangent to the radius of the center actor
		FVector RotatedVectorAroundAxis = FVector(Radius, 0, 0);
		StartingRotationPosition = CenterLocation + RotatedVectorAroundAxis.RotateAngleAxis(StartingAngle, FVector(0, 0, 1));
		StartingRotationPosition.Z = RotationHeight;
		bHasStartingRotationPosition = true;
		bShouldMoveToStartingPosition = true;
		CurrentPosition = InitialLocation;
	}
}

void UQuestRotationComponent::StopRotating()
{
	UE_LOG(LogTemp, Warning, TEXT("QuestRotationComponent::StopRotating:  Function called for %s!"), *GetOwner()->GetName());
	SetComponentTickEnabled(false);
	bShouldRotate = false;
	bHasStartingRotationPosition = false;
	
	// Commented in order to implement the tag removal in blueprint
	AQuestCharacterBase* QuestCharacterToRotate = Cast<AQuestCharacterBase>(GetOwner());
	if (IsValid(QuestCharacterToRotate))
	{
		QuestCharacterToRotate->RemoveGameplayTag(UQuestGlobalTags::Status_Spinning());
	}
}

void UQuestRotationComponent::OnCharacterDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("QuestRotationComponent::OnCharacterDeath:  Function called!"));
	StopRotating();
}

