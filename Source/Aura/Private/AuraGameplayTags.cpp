// EdrSantander Dev


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	/* Primary attributes*/
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attributes.Primary.Strength"),
			FString("Increases physical damage"));

	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
				FName("Attributes.Primary.Intelligence"),
				FString("Increases magical damage"));

	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attributes.Primary.Resilience"),
			FString("Increases armor and armor penetration"));

	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attributes.Primary.Vigor"),
			FString("Increases health"));

	/* Secondary attributes*/
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces damage taken."));

	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"),
		FString("Ignored percentage of armor and increases critical hit chance."));

	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString("Chance to cut incoming in half"));

	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString("Chance to double damage plus critical hit bonus."));

	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("Bonus damage added when a critical hit is scored."));

	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"),
		FString("Reduces critical hit chance of attacking enemies."));

	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString("Amount of health regenerated every 1 second."));

	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString("Amount of mana regenerated every 1 second."));

	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum amount of health obtainable."));

	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("Maximum amount of mana obtainable."));

	/* Input Tags*/
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input tag for left mouse button"));

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input tag for right mouse button"));

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input tag for 1 key button"));

	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input tag for 2 key button"));

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input tag for 3 key button"));

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input tag for 4 key button"));

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Tag for damage"));

	/* DamageTypes*/	
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"),
		FString("Tag for damage fire type"));
	
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Lightning"),
		FString("Tag for damage lightning type"));

	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Arcane"),
		FString("Tag for damage arcane type"));

	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Tag for damage Physical type"));

	/* Resistances*/
	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Arcane"),
		FString("Tag for resistance to arcane"));

	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attributes.Resistance.Fire"),
			FString("Tag for resistance to Fire"));

	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Lightning"),
		FString("Tag for resistance to Lightning"));

	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Physical"),
		FString("Tag for resistance to Physical"));

	/* Map of DamageTypes to Resistances*/
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);

	/* CombatSockets*/
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("Tag for montage HitReact"));

	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Weapon"),
		FString("Tag for CombatSocket attack weapon"));

	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.LeftHand"),
		FString("Tag for CombatSocket left hand attack"));

	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.RightHand"),
		FString("Tag for CombatSocket right hand attack"));

	GameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Tail"),
		FString("Tag for CombatSocket Tail"));

	/* Montage tags*/
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.1"),
		FString("Tag for montage Attack 1"));

	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.2"),
		FString("Tag for montage Attack 2"));

	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.3"),
		FString("Tag for montage Attack 3"));

	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.4"),
		FString("Tag for montage Attack 4"));
	
	/* Abilities*/
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("Tag for attack ability"));
}
