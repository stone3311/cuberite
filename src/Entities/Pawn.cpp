
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Pawn.h"
#include "../World.h"
#include "../Bindings/PluginManager.h"





cPawn::cPawn(eEntityType a_EntityType, double a_Width, double a_Height):
	super(a_EntityType, 0, 0, 0, a_Width, a_Height),
	m_EntityEffects(tEffectMap())
{
}





void cPawn::Tick(float a_Dt, cChunk & a_Chunk)
{
	// Iterate through this entity's applied effects
	for (tEffectMap::iterator iter = m_EntityEffects.begin(); iter != m_EntityEffects.end();)
	{
		// Copies values to prevent pesky wrong accesses and erasures
		cEntityEffect::eType EffectType = iter->first;
		cEntityEffect & EffectValues = iter->second;
		
		// Apply entity effect
		HandleEntityEffect(EffectType, EffectValues);
		
		// Reduce the effect's duration
		EffectValues.m_Ticks++;
		
		// Iterates (must be called before any possible erasure)
		++iter;
		
		// Remove effect if duration has elapsed
		if (EffectValues.GetDuration() - EffectValues.m_Ticks <= 0)
		{
			RemoveEntityEffect(EffectType);
		}
		
		// TODO: Check for discrepancies between client and server effect values
	}
	
	super::Tick(a_Dt, a_Chunk);
}





void cPawn::KilledBy(cEntity * a_Killer)
{
	ClearEntityEffects();
}





void cPawn::AddEntityEffect(cEntityEffect::eType a_EffectType, int a_EffectDurationTicks, short a_EffectIntensity, cPawn * a_Creator, double a_DistanceModifier)
{
	// Check if the plugins allow the addition:
	if (cPluginManager::Get()->CallHookEntityAddEffect(*this, a_EffectType, a_EffectDurationTicks, a_EffectIntensity, a_Creator, a_DistanceModifier))
	{
		// A plugin disallows the addition, bail out.
		return;
	}
	
	// No need to add empty effects:
	if (a_EffectType == cEntityEffect::effNoEffect)
	{
		return;
	}
	
	int EffectDuration = (int)(a_EffectDurationTicks * a_DistanceModifier);
	m_EntityEffects[a_EffectType] = cEntityEffect(EffectDuration, a_EffectIntensity, a_Creator, a_DistanceModifier);
	m_World->BroadcastEntityEffect(*this, a_EffectType, a_EffectIntensity, EffectDuration);
}





void cPawn::RemoveEntityEffect(cEntityEffect::eType a_EffectType)
{
	m_EntityEffects.erase(a_EffectType);
	m_World->BroadcastRemoveEntityEffect(*this, a_EffectType);
}





void cPawn::ClearEntityEffects()
{
	// Iterate through this entity's applied effects
	for (tEffectMap::iterator iter = m_EntityEffects.begin(); iter != m_EntityEffects.end();)
	{
		// Copy values to prevent pesky wrong erasures
		cEntityEffect::eType EffectType = iter->first;
		
		// Iterates (must be called before any possible erasure)
		++iter;
		
		// Remove effect
		RemoveEntityEffect(EffectType);
	}
}





void cPawn::HandleEntityEffect(cEntityEffect::eType a_EffectType, cEntityEffect a_Effect)
{
	switch (a_EffectType)
	{
		// Default effect behaviors
		case cEntityEffect::effInstantHealth:
		{
			// Base heal = 6, doubles for every increase in intensity
			Heal((int)(6 * std::pow(2.0, a_Effect.GetIntensity()) * a_Effect.GetDistanceModifier()));
			return;
		}
		case cEntityEffect::effInstantDamage:
		{
			// Base damage = 6, doubles for every increase in intensity
			int damage = (int)(6 * std::pow(2.0, a_Effect.GetIntensity()) * a_Effect.GetDistanceModifier());
			TakeDamage(dtPotionOfHarming, a_Effect.GetCreator(), damage, 0);
			return;
		}
		case cEntityEffect::effStrength:
		{
			// TODO: Implement me!
			return;
		}
		case cEntityEffect::effWeakness:
		{
			// Damage reduction = 0.5 damage, multiplied by potion level (Weakness II = 1 damage)
			// double dmg_reduc = 0.5 * (a_Effect.GetIntensity() + 1);
			
			// TODO: Implement me!
			// TODO: Weakened villager zombies can be turned back to villagers with the god apple
			return;
		}
		case cEntityEffect::effRegeneration:
		{
			// Regen frequency = 50 ticks, divided by potion level (Regen II = 25 ticks)
			int frequency = std::floor(50.0 / (double)(a_Effect.GetIntensity() + 1));
			
			if (a_Effect.m_Ticks % frequency == 0)
			{
				Heal(1);
			}
			
			return;
		}
		case cEntityEffect::effPoison:
		{
			// Poison frequency = 25 ticks, divided by potion level (Poison II = 12 ticks)
			int frequency = std::floor(25.0 / (double)(a_Effect.GetIntensity() + 1));
			
			if (a_Effect.m_Ticks % frequency == 0)
			{
				// Cannot take poison damage when health is at 1
				if (GetHealth() > 1)
				{
					TakeDamage(dtPoisoning, a_Effect.GetCreator(), 1, 0);
				}
			}
			
			return;
		}
		case cEntityEffect::effWither:
		{
			// Poison frequency = 40 ticks, divided by effect level (Wither II = 20 ticks)
			int frequency = std::floor(25.0 / (double)(a_Effect.GetIntensity() + 1));
			
			if (a_Effect.m_Ticks % frequency == 0)
			{
				TakeDamage(dtWither, a_Effect.GetCreator(), 1, 0);
			}
			//TODO: "<Player> withered away>
			return;
		}
		case cEntityEffect::effFireResistance:
		{
			// TODO: Implement me!
			return;
		}
		case cEntityEffect::effSpeed:
		{
			// TODO: Implement me!
			return;
		}
		case cEntityEffect::effSlowness:
		{
			// TODO: Implement me!
			return;
		}
	}
}
