// EdrSantander Dev


#include "Character/AuraEnemy.h"

void AAuraEnemy::HighlightActor()
{
	const FString ActorName = GetNameSafe(this);
	UE_LOG(LogTemp, Warning, TEXT("Actor Highligthed %s"), *ActorName);
}

void AAuraEnemy::UnHighlightActor()
{
	const FString ActorName = GetNameSafe(this);
	UE_LOG(LogTemp, Warning, TEXT("Actor UnHighligthed %s"), *ActorName);
}
