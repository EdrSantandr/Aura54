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

	/* Meta attributes*/
	GameplayTags.Attributes_Meta_IncomingXp = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingXP"),
		FString("Tag for IncomingXP meta attribute."));
	
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

	GameplayTags.InputTag_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.5"),
		FString("Input tag for 5 key button"));

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.6"),
		FString("Input tag for 6 key button"));

	GameplayTags.InputTag_Q = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Q"),
		FString("Input tag for Q key button"));

	GameplayTags.InputTag_W = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.W"),
		FString("Input tag for W key button"));

	GameplayTags.InputTag_E = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.E"),
		FString("Input tag for E key button"));
	
	GameplayTags.InputTag_R = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.R"),
		FString("Input tag for R key button"));

	GameplayTags.InputTag_A = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.A"),
		FString("Input tag for A key button"));

	GameplayTags.InputTag_S = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.S"),
		FString("Input tag for S key button"));

	GameplayTags.InputTag_ESC = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.ESC"),
		FString("Input tag for ESC key button"));

	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Passive.1"),
		FString("Input tag for passive 1"));

	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Passive.2"),
		FString("Input tag for passive 2"));
	
	GameplayTags.InputTag_Confirm = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Confirm"),
		FString("Input tag for Confirm"));

	GameplayTags.InputTag_Cancel = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Cancel"),
		FString("Input tag for Cancel"));

	GameplayTags.InputTag_F1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.F1"),
		FString("Input tag for F1"));
	
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

	GameplayTags.Damage_Poison = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Poison"),
		FString("Tag for damage Poison type"));

	GameplayTags.Damage_Wind = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Wind"),
		FString("Tag for damage Wind type"));

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

	GameplayTags.Attributes_Resistance_Poison = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Poison"),
		FString("Tag for resistance to Poison"));
	
	GameplayTags.Attributes_Resistance_Wind = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Wind"),
		FString("Tag for resistance to Wind"));

	/* DeBuffs*/
	GameplayTags.DeBuff_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DeBuff.Arcane"),
		FString("Tag for DeBuff to Arcane"));

	GameplayTags.DeBuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DeBuff.Burn"),
		FString("Tag for DeBuff to Burn Fire"));

	GameplayTags.DeBuff_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DeBuff.Physical"),
		FString("Tag for DeBuff to Physical"));

	GameplayTags.DeBuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DeBuff.Stun"),
		FString("Tag for DeBuff to Stun Lightning"));

	GameplayTags.DeBuff_Slow = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DeBuff.Slow"),
		FString("Tag for DeBuff to Slow Poison"));
		
	GameplayTags.DeBuff_Disarm = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("DeBuff.Disarm"),
	FString("Tag for DeBuff to Disarm wind"));

	/* DeBuffs general info tags*/
	GameplayTags.DeBuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DeBuff.Chance"),
		FString("Tag for DeBuff Chance"));

	GameplayTags.DeBuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DeBuff.Damage"),
		FString("Tag for DeBuff Damage"));

	GameplayTags.DeBuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DeBuff.Duration"),
		FString("Tag for DeBuff Duration"));

	GameplayTags.DeBuff_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("DeBuff.Frequency"),
		FString("Tag for DeBuff Frequency"));
	
	/* Map of DamageTypes to DeBuffs*/
	GameplayTags.DamageTypesToDeBuffs.Add(GameplayTags.Damage_Arcane, GameplayTags.DeBuff_Arcane);
	GameplayTags.DamageTypesToDeBuffs.Add(GameplayTags.Damage_Fire, GameplayTags.DeBuff_Burn);
	GameplayTags.DamageTypesToDeBuffs.Add(GameplayTags.Damage_Lightning, GameplayTags.DeBuff_Stun);
	GameplayTags.DamageTypesToDeBuffs.Add(GameplayTags.Damage_Physical, GameplayTags.DeBuff_Physical);
	GameplayTags.DamageTypesToDeBuffs.Add(GameplayTags.Damage_Poison, GameplayTags.DeBuff_Slow);
	GameplayTags.DamageTypesToDeBuffs.Add(GameplayTags.Damage_Wind, GameplayTags.DeBuff_Disarm);
	
	/* Map of DamageTypes to Resistances*/
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Poison, GameplayTags.Attributes_Resistance_Poison);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Wind, GameplayTags.Attributes_Resistance_Wind);

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

	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Summon"),
		FString("Tag for Summon ability"));

	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.HitReact"),
			FString("Tag for HitReact ability"));

	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.None"),
			FString("Tag for None ability. Nullptr"));

	/* Offensive Abilities*/
	GameplayTags.Abilities_Fire_Firebolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.Firebolt"),
		FString("Tag for Firebolt ability"));

	GameplayTags.Abilities_Fire_Fireblast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.Fireblast"),
		FString("Tag for Fireblast ability"));

	GameplayTags.Abilities_Fire_Meteor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.Meteor"),
		FString("Tag for Meteor ability"));

	GameplayTags.Abilities_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Lightning.Electrocute"),
		FString("Tag for Lightning electrocute ability"));

	GameplayTags.Abilities_Arcane_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Arcane.ArcaneShards"),
		FString("Tag for Arcane ArcaneShards ability"));

	GameplayTags.Abilities_Arcane_BlackHoleBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Arcane.BlackHoleBolt"),
		FString("Tag for Arcane BlackHoleBolt ability"));

	GameplayTags.Abilities_Wind_Pushback = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Wind.Pushback"),
		FString("Tag for Wind Pushback ability"));

	GameplayTags.Abilities_Wind_WindWave = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Wind.WindWave"),
		FString("Tag for Wind WindWave ability"));

	GameplayTags.Abilities_Poison_InstaKill = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Poison.InstaKill"),
		FString("Tag for Poison InstaKill ability"));

	/* Passive Abilities*/
	GameplayTags.Abilities_Passive_LifeSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Passive.LifeSiphon"),
			FString("Tag for Passive LifeSiphon ability"));

	GameplayTags.Abilities_Passive_ManaSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Passive.ManaSiphon"),
			FString("Tag for Passive ManaSiphon ability"));
	
	GameplayTags.Abilities_Passive_HaloOfProtection = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Passive.HaloOfProtection"),
			FString("Tag for Passive HaloOfProtection ability"));
	
	GameplayTags.Abilities_Passive_ListenForEvent = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Passive.ListenForEvent"),
			FString("Tag for Passive ListenForEvent ability for dev settings"));
	
	/* Status Abilities*/
	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Status.Eligible"),
			FString("Tag for Status Eligible for ability"));

	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Status.Equipped"),
			FString("Tag for Status Equipped for ability"));

	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
    			FName("Abilities.Status.Locked"),
    			FString("Tag for Status Locked for ability"));

	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Status.Unlocked"),
			FString("Tag for Status Unlocked for ability"));

	/* Types Abilities*/
	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Type.None"),
			FString("Tag for Type None ability"));

	GameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Type.Offensive"),
			FString("Tag for Type Offensive ability"));

	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Type.Passive"),
			FString("Tag for Type Passive ability"));

	GameplayTags.Abilities_Type_Enchant = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Type.Enchant"),
			FString("Tag for Type Enchant ability"));

	/* Enchant Abilities*/
	GameplayTags.Abilities_Enchant_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Enchant.Arcane"),
			FString("Tag for Type Enchant Arcane ability"));

	GameplayTags.Abilities_Enchant_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Enchant.Fire"),
			FString("Tag for Type Enchant Fire ability"));

	GameplayTags.Abilities_Enchant_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Enchant.Lightning"),
			FString("Tag for Type Enchant Lightning ability"));

	GameplayTags.Abilities_Enchant_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Enchant.Physical"),
			FString("Tag for Type Enchant Physical ability"));

	GameplayTags.Abilities_Enchant_Poison = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Enchant.Poison"),
			FString("Tag for Type Enchant Poison ability"));

	GameplayTags.Abilities_Enchant_Wind = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Enchant.Wind"),
			FString("Tag for Type Enchant Wind ability"));

	/* Gate Abilities*/
	GameplayTags.Abilities_Spawn = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Spawn"),
			FString("Tag for Type Spawn ability"));
	
	/* Cooldowns*/
	GameplayTags.Cooldown_Fire_Firebolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Fire.Firebolt"),
		FString("Tag for firebolt cooldown"));

	GameplayTags.Cooldown_Fire_Fireblast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Fire.Fireblast"),
		FString("Tag for Fireblast cooldown"));

	GameplayTags.Cooldown_Fire_Fireblast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Fire.Meteor"),
		FString("Tag for Meteor cooldown"));

	GameplayTags.Cooldown_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Lightning.Electrocute"),
		FString("Tag for Lightning_Electrocute cooldown"));

	GameplayTags.Cooldown_Arcane_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Arcane.ArcaneShards"),
		FString("Tag for Arcane_ArcaneShards cooldown"));

	GameplayTags.Cooldown_Arcane_BlackHoleBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Arcane.BlackHoleBolt"),
		FString("Tag for Arcane_BlackHoleBolt cooldown"));

	GameplayTags.Cooldown_Wind_Pushback = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Wind.Pushback"),
		FString("Tag for Wind_Pushback cooldown"));

	GameplayTags.Cooldown_Wind_WindWave = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Wind.WindWave"),
		FString("Tag for WindWave cooldown"));

	GameplayTags.Cooldown_Poison_InstaKill = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Poison.InstaKill"),
		FString("Tag for Poison.InstaKill cooldown"));
	
	GameplayTags.Cooldown_Enchant_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Enchant.Arcane"),
		FString("Tag for Enchant Arcane cooldown"));

	GameplayTags.Cooldown_Enchant_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Enchant.Fire"),
		FString("Tag for Enchant Fire cooldown"));

	GameplayTags.Cooldown_Enchant_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Enchant.Lightning"),
		FString("Tag for Enchant Lightning cooldown"));

	GameplayTags.Cooldown_Enchant_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Enchant.Physical"),
		FString("Tag for Enchant Physical cooldown"));

	GameplayTags.Cooldown_Enchant_Poison = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Enchant.Poison"),
		FString("Tag for Enchant Poison cooldown"));

	GameplayTags.Cooldown_Enchant_Wind = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Enchant.Wind"),
		FString("Tag for Enchant Wind cooldown"));

	GameplayTags.Cooldown_Spawn = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Spawn"),
		FString("Tag for Spawn cooldown"));

	GameplayTags.Cooldown_Passive_ManaSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Passive.ManaSiphon"),
		FString("Tag for Cooldown.Passive.ManaSiphon"));
	
	/* Player*/
	GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.CursorTrace"),
		FString("Tag for Player Block CursorTrace"));

	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputHeld"),
		FString("Tag for Player Block InputHeld"));

	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputPressed"),
		FString("Tag for Player Block InputPressed"));

	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputReleased"),
		FString("Tag for Player Block InputReleased"));

	/* GameplayCues*/
	GameplayTags.GameplayCue_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.FireBlast"),
		FString("Tag for GameplayCue FireBlast"));

	/* Tree Enchanted Types */
	GameplayTags.Tree_Enchanted_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Tree.Enchanted.None"),
		FString("Tag for Tree Enchanted None"));

	GameplayTags.Tree_Enchanted_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Tree.Enchanted.Arcane"),
		FString("Tag for Tree Enchanted Arcane"));

	GameplayTags.Tree_Enchanted_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Tree.Enchanted.Fire"),
		FString("Tag for Tree Enchanted Fire"));
	
	GameplayTags.Tree_Enchanted_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Tree.Enchanted.Lightning"),
		FString("Tag for Tree Enchanted Lightning"));
	
	GameplayTags.Tree_Enchanted_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Tree.Enchanted.Physical"),
		FString("Tag for Tree Enchanted Physical"));
}
