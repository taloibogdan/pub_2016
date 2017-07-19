#include <SDL.h>
#include <iostream>
#include <functional>
#include <Engine/Util/json/json.h>
#include <Engine/Util/json/json-forwards.h>
#include <SDL_opengl.h>
#include <Engine/Config.h>

namespace Engine
{
	class Entity;
	class ComponentFactory;
	class PhysicsSimulator;
	class ScriptContainer;
	class EntityManager;
	class Renderer;
	struct SCreationSettings;

	class ENGINE_API SystemImpl
	{
	public:
		Renderer* renderer;
		ComponentFactory* componentFactory;
		PhysicsSimulator* physicsSimulator;
		ScriptContainer* scriptContainer;
		EntityManager* entityManager;

		void Init(SCreationSettings&& cs);
		void Start();
		void AddScript(std::string name, std::function<void(Entity*, Json::Value, float)> scr);

	protected:
		SDL_Window* m_window;
		SDL_GLContext m_context;
	};
}