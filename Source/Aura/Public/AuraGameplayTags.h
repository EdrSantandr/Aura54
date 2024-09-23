// EdrSantander Dev

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Aura gameplayTags - Singleton
 */
struct FAuraGameplayTags
{

public:
	static const FAuraGameplayTags Get() { return GameplayTags;}
	static void InitializeNativeGameplayTags();
	
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;
	
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	
	FGameplayTag Attributes_Meta_IncomingXp;

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_5;
	FGameplayTag InputTag_6;
	FGameplayTag InputTag_Q;
	FGameplayTag InputTag_W;
	FGameplayTag InputTag_E;
	FGameplayTag InputTag_R;
	FGameplayTag InputTag_A;
	FGameplayTag InputTag_S;
	FGameplayTag InputTag_ESC;
	FGameplayTag InputTag_Confirm;
	FGameplayTag InputTag_Cancel;
	FGameplayTag InputTag_F1;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;

	FGameplayTag Damage;
	
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Poison;
	FGameplayTag Damage_Wind;

	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;
	FGameplayTag Attributes_Resistance_Poison;
	FGameplayTag Attributes_Resistance_Wind;

	FGameplayTag DeBuff_Burn;
	FGameplayTag DeBuff_Stun;
	FGameplayTag DeBuff_Arcane;
	FGameplayTag DeBuff_Physical;
	FGameplayTag DeBuff_Slow;
	FGameplayTag DeBuff_Disarm;

	FGameplayTag DeBuff_Chance;
	FGameplayTag DeBuff_Damage;
	FGameplayTag DeBuff_Duration;
	FGameplayTag DeBuff_Frequency;

	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	FGameplayTag Abilities_HitReact;
	FGameplayTag Abilities_None;

	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;

	FGameplayTag Abilities_Type_Enchant;
	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;

	FGameplayTag Abilities_Fire_Firebolt;
	FGameplayTag Abilities_Fire_Fireblast;
	FGameplayTag Abilities_Fire_Meteor;
	FGameplayTag Abilities_Lightning_Electrocute;
	FGameplayTag Abilities_Arcane_ArcaneShards;
	FGameplayTag Abilities_Arcane_BlackHoleBolt;
	FGameplayTag Abilities_Wind_Pushback;
	FGameplayTag Abilities_Wind_WindWave;
	FGameplayTag Abilities_Poison_InstaKill;

	FGameplayTag Abilities_Enchant_Fire;
	FGameplayTag Abilities_Enchant_Lightning;
	FGameplayTag Abilities_Enchant_Arcane;
	FGameplayTag Abilities_Enchant_Physical;
	FGameplayTag Abilities_Enchant_Poison;
	FGameplayTag Abilities_Enchant_Wind;
	FGameplayTag Abilities_Spawn;
	
	FGameplayTag Abilities_Passive_HaloOfProtection;
	FGameplayTag Abilities_Passive_LifeSiphon;
	FGameplayTag Abilities_Passive_ManaSiphon;
	FGameplayTag Abilities_Passive_ListenForEvent;
	
	FGameplayTag Cooldown_Fire_Firebolt;
	FGameplayTag Cooldown_Fire_Fireblast;
	FGameplayTag Cooldown_Fire_Meteor;
	FGameplayTag Cooldown_Lightning_Electrocute;
	FGameplayTag Cooldown_Arcane_ArcaneShards;
	FGameplayTag Cooldown_Arcane_BlackHoleBolt;
	FGameplayTag Cooldown_Wind_Pushback;
	FGameplayTag Cooldown_Wind_WindWave;
	FGameplayTag Cooldown_Poison_InstaKill;
	FGameplayTag Cooldown_Enchant_Fire;
	FGameplayTag Cooldown_Enchant_Lightning;
	FGameplayTag Cooldown_Enchant_Arcane;
	FGameplayTag Cooldown_Enchant_Physical;
	FGameplayTag Cooldown_Enchant_Poison;
	FGameplayTag Cooldown_Enchant_Wind;
	FGameplayTag Cooldown_Spawn;
	FGameplayTag Cooldown_Passive_ManaSiphon;
	
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;

	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	FGameplayTag Tree_Enchanted_None;
	FGameplayTag Tree_Enchanted_Fire;
	FGameplayTag Tree_Enchanted_Lightning;
	FGameplayTag Tree_Enchanted_Arcane;
	FGameplayTag Tree_Enchanted_Physical;

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDeBuffs;
	
	FGameplayTag Effects_HitReact;

	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;
	FGameplayTag Player_Block_CursorTrace;
	
	FGameplayTag GameplayCue_FireBlast;
	
private:
	static FAuraGameplayTags GameplayTags;
};
