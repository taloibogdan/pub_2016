#include <Engine/Util/json/json.h>
#include <functional>
namespace Engine
{
	class Entity;
	class ScriptContainer
	{
	public:
		void AddScript(std::string name, std::function<void(Entity*, Json::Value, float)> func);
		void RunScript(std::string name, Entity* ent, Json::Value settings, float dt);
	private:
		std::vector<std::pair<std::string, std::function<void(Entity*, Json::Value, float)>>> scripts;
	};
}