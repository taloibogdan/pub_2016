#include <Engine/ComponentSystem/Components.h>
namespace Engine
{
	void IEventDispatcher::OnEvent(Event* event)
	{
		for (auto& rec : m_receivers)
		{
			if (rec.first == event->GetName())
				rec.second->OnEvent(event);
		}
		delete event;
	}
	void IEventDispatcher::RegisterReceiver(std::string name, IEventReceiver* rec)
	{
		m_receivers.emplace_back(name, rec);
	}
	void Entity::RegisterComponent(IComponent* comp)
	{
		m_components.emplace_back(comp);
	}
	Entity::~Entity()
	{
		delete transform;
	}
	void Entity::Update(float dt)
	{
		for (auto const& i : m_components)
		{
			(*i).Update(dt);
		}
	}
	void Entity::LateUpdate(float dt)
	{
		for (auto const& i : m_components)
		{
			(*i).LateUpdate(dt);
		}
	}
}