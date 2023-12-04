// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"

#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"

USAction_ProjectileAttack::USAction_ProjectileAttack() {
	HandSocketName = "Muzzle_01";

	AttackAnimDelay = 0.2f;

	CoolDownTime = 0.001f;

	bIsActive = true;
}

bool USAction_ProjectileAttack::CanStart_Implementation(AActor* InstigatorActor) {
	bool bCanStart = Super::CanStart_Implementation(InstigatorActor);

	if (bIsActive && bCanStart) {
		return true;
	}
	
	APlayerController *PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	PC->ClientStartCameraShake(CoolDownCameraShake);
	
	return false;
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* NewInstigator) {
	Super::StartAction_Implementation(NewInstigator);

	ASCharacter *Character = Cast<ASCharacter>(Instigator);
	if (!Character) {
		return;
	}

	// Character turn to attack direction. only change yaw.
	FRotator ControlRotator = Character->GetControlRotation();
	ControlRotator.Pitch = Character->GetActorRotation().Pitch;
	ControlRotator.Roll = Character->GetActorRotation().Roll;
	FRotator NewRotation = FMath::RInterpTo(Character->GetActorRotation(), ControlRotator, GetWorld()->GetDeltaSeconds(), 100.0f);
	Character->SetActorRotation(NewRotation);
	
	Character->PlayAnimMontage(AttackAnim);

	UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), FName(HandSocketName));

	FTimerHandle TimerHandle_AttackDelay;
	FTimerDelegate Delegate_AttackDelay;
	Delegate_AttackDelay.BindUFunction(this, "AttackDelay_TimeElapsed", Character);
	// GetWorld() is override in SAction
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate_AttackDelay, AttackAnimDelay, false);
}

void USAction_ProjectileAttack::AttackDelay_TimeElapsed(ASCharacter* InstigatorCharacter) {
	if (!ensureAlways(ProjectileClass)) {
		StopAction(InstigatorCharacter);
		return;
	}
	
	// get Start and End vector to construct the rotator of the projectile

	// get Start Vector
	FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(FName(HandSocketName));
	FVector Start = HandLocation;

	// get End Vector
	// do line trace detection
	FHitResult Hit;
	FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
	FVector TraceEnd = TraceStart + InstigatorCharacter->GetControlRotation().Vector() * 5000.0f;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionShape Shape;
	Shape.SetSphere(20.0f);

	// ignore the player
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(InstigatorCharacter);

	// GetWorld() is override in USAction
	bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape, QueryParams);
	FVector End = bBlockingHit ? Hit.ImpactPoint : TraceEnd;

	// get the transformation of the spawned projectile
	// since the spawned actor will move straight ahead, namely along X axis, so we use MakeFromX()
	// MakeFromX() make the spawned actor's X axis in the same direction as (End - Start)
	// SpawnTW contains both the rotation and location information of the spawned actor
	FRotator ProjectileRotation = FRotationMatrix::MakeFromX(End - Start).Rotator();
	FTransform SpawnTW = FTransform(ProjectileRotation, HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = InstigatorCharacter;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTW, SpawnParams);

	StopAction(InstigatorCharacter);
}

void USAction_ProjectileAttack::StopAction_Implementation(AActor* NewInstigator) {
	Super::StopAction_Implementation(NewInstigator);

	// CoolDown
	bIsActive = false;

	FTimerHandle TimerHandle_CoolDown;
	FTimerDelegate Delegate_CoolDown;
	Delegate_CoolDown.BindUFunction(this, "CoolDown_TimeElapsed");
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CoolDown, Delegate_CoolDown, CoolDownTime, false);
}

void USAction_ProjectileAttack::CoolDown_TimeElapsed() {
	bIsActive = true;
}
