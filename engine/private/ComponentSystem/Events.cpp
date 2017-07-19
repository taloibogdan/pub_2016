#include<Engine/ComponentSystem/Events.h>
namespace Engine
{
	CollisionEvent::CollisionEvent(Entity* o, Vector2 r)
	{
		other = o;
		reaction = r;
	}

	DamageEvent::DamageEvent(float dmg, int btm)
	{
		damage = dmg;
		bitmask = btm;
	}
	InvulnerabilityEvent::InvulnerabilityEvent(bool start, float cd0, float cd1)
	{
		isStart = start;
		blinkingCD0 = cd0;
		blinkingCD1 = cd1;
	}
}