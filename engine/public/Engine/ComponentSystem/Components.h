#include<Engine/ComponentSystem/EventInterfaces.h>
#include <Engine/Rendering/Sprite.h>
#include <Engine/Util/Vector2.h>
namespace Engine {

	class IComponent : public IEventReceiver
	{
	public:
		Entity* GetEntity() { return m_entity; }
		virtual void Update(float dt) {}
		virtual void LateUpdate(float dt) {}
		virtual void OnEvent(Event* event) = 0;
	protected:
		Entity* m_entity;
	};

	class TransformComponent : public IComponent
	{
	public:
		Vector2 position;
		TransformComponent(Entity* ent, Json::Value obj);
		void OnEvent(Event* event);
	};

	class RenderComponent : public IComponent
	{
	public:
		Texture* texture;
		Sprite* sprite;

		float blinkingCD0Max = 0;
		float blinkingCD1Max = 0;
		float blinkingCD = 0;
		bool isVisible = true;
		bool isBlinking = false;

		RenderComponent(Entity* ent, Json::Value obj);
		virtual void LateUpdate(float dt)override;
		virtual void OnEvent(Event* event);

		~RenderComponent()
		{
			delete sprite;
			delete texture;
		}
	private:
		void Render();
	};
	
	class Collider
	{
	public:
		int type; // 0 = circle
		Vector2 centerRelative;
		float radiusx, radiusy;
		Collider() {}
		Collider(Json::Value obj);
	};

	class PhysicsSimulator;

	class PhysicsComponent : public IComponent
	{
	public:
		Collider collider;
		int collisionBitmask;
		bool isImmovable;
		PhysicsComponent(Entity* ent, PhysicsSimulator* psim, Json::Value obj);
		void CheckCollision(PhysicsComponent* other);
		void OnEvent(Event* event);
	private:
		PhysicsSimulator* m_physicsSimulator;
	};


	class DamageDealerComponent : public IComponent
	{
	public:
		int damageBitmask;
		float damage;
		DamageDealerComponent(Entity* ent, Json::Value obj);
		void OnEvent(Event* event);
	};

	class HealthComponent : public IComponent
	{
	public:
		int healthBitmask;
		float health;
		float damageCD;
		float currentDamageCD = 0;
		HealthComponent(Entity* ent, Json::Value obj);
		void OnEvent(Event* event);
		void LateUpdate(float dt)override;
	};

	class ScriptContainer;
	class ScriptComponent : public IComponent
	{
	public:
		std::string scriptName;
		ScriptContainer* scriptContainer;
		Json::Value settings;
		ScriptComponent(Entity* ent, ScriptContainer* sc, Json::Value obj);
		void OnEvent(Event* event) {}
		void Update(float dt)override;
	};

	class SpawnerComponent : public IComponent
	{
	public:
		Vector2 offset;
		Vector2 area;
		float spawnCD;
		std::vector<Json::Value> entitiesToSpawn;
		SpawnerComponent(Entity* ent, Json::Value obj);
		void OnEvent(Event* event) {}
		void Update(float dt)override;
	private:
		float currentSpawnCD;
		int nextEntityID = 0;
	};
}