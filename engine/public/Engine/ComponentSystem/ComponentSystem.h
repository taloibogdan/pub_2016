#pragma once
#include <Engine/ComponentSystem/Components.h>
#include <functional>
namespace Engine
{
	class PhysicsSimulator
	{
	public:
		void AddComponent(PhysicsComponent* pc);
		void RemoveComponent(PhysicsComponent* pc);
		void CheckCollisions();
	private:
		std::vector<PhysicsComponent*> components;
	};

	class ComponentFactory
	{
	public:
		void RegisterComponentType(std::string name, std::function<IComponent*(Entity*, Json::Value) > fct);
		IComponent* CreateComponent(std::string name, Entity* ent, Json::Value data);
	protected:
		std::vector<std::pair<std::string, std::function<IComponent*(Entity*, Json::Value)>>> m_constructors;
	};

	class EntityManager
	{
	public:
		EntityManager(SystemImpl* sys){ system = sys; }
		Entity* AddEntity(Json::Value obj)
		{
			Entity* ent = new Entity(system, obj);
			entities.push_back(ent);
			return ent;
		}
		void Update(float dt);
		void LateUpdate(float dt);
	private:
		SystemImpl* system;
		std::vector<Entity*> entities;
	};

}
