#pragma once

class cPawn;

// tolua_begin
class cEntityEffect
{
public:
	
	/** All types of entity effects (numbers correspond to IDs) */
	enum eType
	{
		effNoEffect       = 0,
		effSpeed          = 1,
		effSlowness       = 2,
		effHaste          = 3,
		effMiningFatigue  = 4,
		effStrength       = 5,
		effInstantHealth  = 6,
		effInstantDamage  = 7,
		effJumpBoost      = 8,
		effNausea         = 9,
		effRegeneration   = 10,
		effResistance     = 11,
		effFireResistance = 12,
		effWaterBreathing = 13,
		effInvisibility   = 14,
		effBlindness      = 15,
		effNightVision    = 16,
		effHunger         = 17,
		effWeakness       = 18,
		effPoison         = 19,
		effWither         = 20,
		effHealthBoost    = 21,
		effAbsorption     = 22,
		effSaturation     = 23,
	} ;
	
	/** How many ticks this effect has been active for */
	int m_Ticks;
	
	/** Returns the duration of the effect */
	int GetDuration() { return m_Duration; }
	
	/** Returns how strong the effect will be applied */
	short GetIntensity() { return m_Intensity; }
	
	/** Returns the pawn that produced this entity effect */
	cPawn *GetCreator() { return m_Creator; }
	
	/** Returns the distance modifier for affecting potency */
	double GetDistanceModifier() { return m_DistanceModifier; }
	
	void SetDuration(int a_Duration)                    { m_Duration         = a_Duration;         }
	void SetIntensity(short a_Intensity)                { m_Intensity        = a_Intensity;        }
	void SetCreator(cPawn * a_Creator)                  { m_Creator          = a_Creator;          }
	void SetDistanceModifier(double a_DistanceModifier) { m_DistanceModifier = a_DistanceModifier; }
	
	/** Creates an empty entity effect */
	cEntityEffect(void);
	
	/** Creates an entity effect of the specified type
	@param a_Duration         How long this effect will last, in ticks
	@param a_Intensity        How strong the effect will be applied
	@param a_Creator          The pawn that produced this entity effect
	@param a_DistanceModifier The distance modifier for affecting potency, defaults to 1 */
	cEntityEffect(int a_Duration, short a_Intensity, cPawn * a_Creator, double a_DistanceModifier = 1);
	
private:
	/** How long this effect will last, in ticks */
	int m_Duration;
	
	/** How strong the effect will be applied */
	short m_Intensity;
	
	/** The pawn that produced this entity effect (threw the potion, etc) */
	cPawn *m_Creator;
	
	/** The distance modifier for affecting potency */
	double m_DistanceModifier;
};
// tolua_end
