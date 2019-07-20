// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestCharacterRotationActor.h"
#include "Math/Vector.h"
#include "GameFramework/Pawn.h"

// Sets default values
AQuestCharacterRotationActor::AQuestCharacterRotationActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AQuestCharacterRotationActor::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = CharacterToRotate->GetActorLocation();
	ClockwiseFloat = (bClockwise) ? 1 : -1;
}

// Called every frame
void AQuestCharacterRotationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

void AQuestCharacterRotationActor::RotateActorAroundPoint()
{
	bShouldRotate = true;
	if (ActorAtCenter && CharacterToRotate)
	{
		// Rotate the character around the center axis
		FVector CenterCharacterLocation = ActorAtCenter->GetActorLocation();
		CenterCharacterLocation.Z += RotationHeight;
		FVector RotatedVectorAroundAxis = FVector(Radius, 0, RotationHeight);
		CharacterToRotate->SetActorLocation(CenterCharacterLocation + RotatedVectorAroundAxis.RotateAngleAxis((StartingAngle + CurrentRotation), FVector(0, 0, 1)));

		// Rotate the character around its own axis
		FRotator RotatedCharacterRotator = CharacterToRotate->GetActorRotation();
		RotatedCharacterRotator.Yaw = (StartingAngle - 180 + CurrentRotation);
		CharacterToRotate->SetActorRotation(RotatedCharacterRotator);
	}
	return;
}

void AQuestCharacterRotationActor::MoveActorToStartingRotationPosition(float DeltaTime)
{	
	if (CharacterToRotate)

	{
		CurrentPosition = FMath::VInterpTo(CharacterToRotate->GetActorLocation(), StartingRotationPosition, DeltaTime, MovementSpeedToStartingRotationPosition);
		CharacterToRotate->SetActorLocation(CurrentPosition, true);
	}

	//  If the character is close enough to the starting rotation position, stop moving and start rotating
	FVector RemainingDistance = CharacterToRotate->GetActorLocation() - StartingRotationPosition;
	if (RemainingDistance.Size() < 30)
	{
		bShouldMoveToStartingPosition = false;
		bShouldRotate = true;
	}
}

void AQuestCharacterRotationActor::GetStartingRotationPosition()
{
	if (ActorAtCenter && CharacterToRotate)
	{
		// Adjust rotation height based on the fact that the character's Z position is different than the center's Z position
		RotationHeight = RotationHeight + (CharacterToRotate->GetActorLocation().Z - ActorAtCenter->GetActorLocation().Z);
		FVector CharacterLocation = CharacterToRotate->GetActorLocation();
		FVector CenterLocation = ActorAtCenter->GetActorLocation() + FVector(0, 0, RotationHeight);


		// Get the starting angle for rotation
		FVector NormalCharacter = CharacterLocation.GetSafeNormal();
		FVector NormalCenter = CenterLocation.GetSafeNormal();
		StartingAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(NormalCenter, NormalCharacter))) + 180;

		// Set the starting position based on a tangent to the radius of the center actor
		FVector RotatedVectorAroundAxis = FVector(Radius, 0, RotationHeight);
		StartingRotationPosition = CenterLocation + RotatedVectorAroundAxis.RotateAngleAxis(StartingAngle, FVector(0, 0, 1));

		bHasStartingRotationPosition = true;
		bShouldMoveToStartingPosition = true;
		CurrentPosition = InitialLocation;
	}
}

void AQuestCharacterRotationActor::EjectCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("Eject character called"))
	bShouldRotate = false;
	APawn* PawnToEject = Cast<APawn>(CharacterToRotate);
	FVector NormalCharacter = CharacterToRotate->GetActorLocation().GetSafeNormal();
	FVector NormalCenter = (ActorAtCenter->GetActorLocation() + FVector(0, 0, RotationHeight)).GetSafeNormal();
	FVector DirectionToEject = (NormalCenter - NormalCharacter).RotateAngleAxis(90, FVector(0, 0, 1));
	PawnToEject->AddMovementInput(DirectionToEject, 525, true);
	UE_LOG(LogTemp, Warning, TEXT("direction to eject is %f, %f, %f"), DirectionToEject.X, DirectionToEject.Y, DirectionToEject.Z)
	Destroy();
}

