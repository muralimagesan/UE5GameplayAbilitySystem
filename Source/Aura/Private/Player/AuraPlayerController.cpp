// © 2025 Itihāsa Studios


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"


AAuraPlayerController::AAuraPlayerController()
{
    bReplicates = true; // In multiplayer, when an entity changes on the server, it is replicated at each of the clients connected to the server.
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
    CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
    Super::BeginPlay();
    check(AuraContext);

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    check(Subsystem);
    Subsystem->AddMappingContext(AuraContext, 0);

    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;

    FInputModeGameAndUI InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputModeData.SetHideCursorDuringCapture(false);
    SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);

}

void AAuraPlayerController::Move(const FInputActionValue &InputActionValue)
{
    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
    const FRotator Rotation = GetControlRotation();
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
 
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
        ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
    }
}

void AAuraPlayerController::CursorTrace()
{
    FHitResult CursorHit;    
    GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);
    if (!CursorHit.bBlockingHit) return;

    LastActor = ThisActor;
    ThisActor = CursorHit.GetActor();

    /** 
     * Line trace from cursor. There are several scenatiors:
     * 1) LastActor is null && ThisActor is null
     *      - Do Nothing
     * 2) LastActor is null && ThisActor is valid
     *      - Highlight ThisActor
     * 3) LastActor is valid && ThisActor is null
     *      - Unhighlight LastActor
     * 4) Both actors are valid, but LastActor != ThisActor
     *      - Unhighlight LastActor, and Highlight ThisActor
     * 5) Both actors are valid, and are the same actor
     *      - Do nothing
     */

    if (LastActor == nullptr)
    {
        if (ThisActor != nullptr)
        {
            // Case 2
            ThisActor->HighlightActor();
        }
        else
        {
            // Case 1 - Do Nothing

        }
    }
    else    // LastActor is valid
    {
        if (ThisActor == nullptr)
        {
            // Case 3
            LastActor->UnHighlightActor();
        }
        else // both actors are valid
        {
            if (ThisActor != LastActor)
            {
                // Case 4
                LastActor->UnHighlightActor();
                ThisActor->HighlightActor();
            }
            else
            {
                // Case 5 - Do Nothing
            }
        }
    }
}
