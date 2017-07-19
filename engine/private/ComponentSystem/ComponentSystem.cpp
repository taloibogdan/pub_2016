#pragma once
#include <Engine/ComponentSystem/ComponentSystem.h>
namespace Engine
{
	void PhysicsSimulator::AddComponent(PhysicsComponent* pc)
	{
		components.push_back(pc);
	}
	void PhysicsSimulator::RemoveComponent(PhysicsComponent* pc)
	{
		int l = std::size(components), i;
		bool ok = false;
		for (i = 0; i < l; ++i)
		{
			if (components[i] == pc)
			{
				ok = true;
				break;
			}
		}
		if (ok)
		{
			components[i] = components[l - 1];
			components.pop_back();
		}
	}
	void PhysicsSimulator::CheckCollisions()
	{
		int l = std::size(components), i, j;
		for (i = 0; i < l - 1; ++i)
			for (j = i + 1; j < l; ++j)
			{
				if ((components[i]->collisionBitmask & components[j]->collisionBitmask) == 0) continue;
				if (!components[i]->isImmovable || !components[j]->isImmovable)
				{
					components[i]->CheckCollision(components[j]);
				}
			}
	}
	void ComponentFactory::RegisterComponentType(std::string name, std::function<IComponent*(Entity*, Json::Value) > fct)
	{
		m_constructors.emplace_back(name, fct);
	}
	IComponent* ComponentFactory::CreateComponent(std::string name, Entity* ent, Json::Value data)
	{
		for (auto& entry : m_constructors)
		{
			if (entry.first == name)
			{
				return entry.second(ent, data);
			}
		}
		return nullptr;
	}
	void EntityManager::Update(float dt)
	{
		int l = std::size(entities);
		for (int i = 0; i < l; ++i)
		{
			if (entities[i]->toBeDestroyed)
			{
				entities[i]->OnEvent(new EntityDestroyedEvent);
				delete entities[i];
				entities[i] = entities[l - 1];
				entities.pop_back();
				l--; i--;
			}
		}
		for (int i = 0; i < l; ++i)
		{
			entities[i]->Update(dt);
		}

	}
	void EntityManager::LateUpdate(float dt)
	{
		for (auto ent : entities) { ent->LateUpdate(dt); }
	}
}
