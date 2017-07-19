#include <Engine/ComponentSystem/Events.h>
#include <memory>
namespace Engine {
	class IEventReceiver
	{
	public:
		virtual void OnEvent(Event* event) = 0;
	};

	class IEventDispatcher
	{
	public:
		void OnEvent(Event* event);
		void RegisterReceiver(std::string name, IEventReceiver* rec);
	private:
		std::vector<std::pair<std::string, IEventReceiver*>> m_receivers;
	};

	class TransformComponent;
	class IComponent;
	class SystemImpl;
	class Entity : public IEventDispatcher
	{
	public:
		bool toBeDestroyed = false;
		std::string name;
		TransformComponent* transform;
		SystemImpl* system;
		Entity(SystemImpl* sys, Json::Value obj);
		void Update(float dt);
		void LateUpdate(float dt);
		void RegisterComponent(IComponent* comp);
		~Entity();
	protected:
		std::vector<std::unique_ptr<IComponent>> m_components;
	};
}