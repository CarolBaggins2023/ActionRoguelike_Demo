// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	this->bUseControllerRotationYaw = false;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
}

void ASCharacter::MoveForward(float Val) {
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Val);
}

void ASCharacter::MoveRight(float Val) {
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Val);
}

void ASCharacter::SprintStart() {
	ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop() {
	ActionComp->StopActionByName(this, "Sprint");
}

void ASCharacter::PrimaryAttack() {
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::DashAttack() {
	ActionComp->StartActionByName(this, "DashAttack");
}

void ASCharacter::PrimaryInteract() {
	if (InteractionComp) {
		InteractionComp->PrimaryInteract();
	}
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("DashAttack", IE_Pressed, this, &ASCharacter::DashAttack);
	
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
}

FVector ASCharacter::GetPawnViewLocation() const {
	return CameraComp->GetComponentLocation();
}

bool ASCharacter::HealSelf(float Amount /* = 100.0f */) {
	return AttributeComp->ApplyHealthChanged(this, Amount);
}

void ASCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta) {
	if (NewHealth <= 0.0f) {
		APlayerController *PC = Cast<APlayerController>(GetController());
		DisableInput(PC);

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		SetLifeSpan(5.0f);
	}
}
