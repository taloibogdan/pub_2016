#include <Engine/ComponentSystem/EventInterfaces.h>
#include <Engine/ComponentSystem/ScriptContainer.h>
namespace Engine
{
	void ScriptContainer::AddScript(std::string name, std::function<void(Entity*, Json::Value, float)> func)
	{
		scripts.emplace_back(name, func);
	}
	void ScriptContainer::RunScript(std::string name, Entity* ent, Json::Value settings, float dt)
	{
		for (auto s : scripts)
			if (s.first == name)
				s.second(ent, settings, dt);
	}
}