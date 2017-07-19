#include<iostream>
#include<Engine/Util/Vector2.h>
namespace Engine {
	class Event
	{
	public:
		virtual std::string GetName() = 0;
	};
	class Entity;

	class CollisionEvent : public Event
	{
	public:
		std::string GetName() { return "collision"; }
		Vector2 reaction;
		Entity* other;
		CollisionEvent(Entity* o, Vector2 r);
	};

	class DamageEvent : public Event
	{
	public:
		std::string GetName() { return "damage"; }
		float damage;
		int bitmask;
		DamageEvent(float dmg, int btm);
	};
	class InvulnerabilityEvent : public Event
	{
	public:
		std::string GetName() { return "invulnerability"; }
		bool isStart;
		float blinkingCD0, blinkingCD1;
		InvulnerabilityEvent(bool start, float cd0, float cd1);
	};
	class EntityDestroyedEvent : public Event
	{
	public:
		std::string GetName() { return "entityDestroyed"; }

	};

	/*	class RequestEvent : public Event
	{
	public:
	RequestEvent(std::string nam)
	{
	name = nam;
	}
	std::string GetName() { return name; }
	private:
	std::string name;
	};*/
}