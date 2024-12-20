// © 2025 Itihāsa Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Setting equal to zero means this is a pure virtual class and we will not provide a definition in this class. Abstract class, to be derived from in enemy class.
	// Any class implementing this interface will be required to override this pure virtual function and provide a definition for it.
	virtual void HighlightActor() = 0;	
	virtual void UnHighlightActor() = 0;
};
