#include <Engine/Rendering/OpenGLRenderer.h>
#include <fstream>
#include <Engine/Util/Timer.h>
#include "System/SystemImpl.h"
#include <Engine/ComponentSystem/ComponentSystem.h>
#include <Engine/ComponentSystem/ScriptContainer.h>
namespace Engine
{


void SystemImpl::Init(SCreationSettings&& cs)
{
	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
	m_window = SDL_CreateWindow(cs.windowTitle, 100, 100, cs.windowWidth, cs.windowHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	m_context = SDL_GL_CreateContext(m_window);
	//if (cs.hideWindow)
	//	SDL_HideWindow(m_window);
	SDL_GL_SetSwapInterval(0);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);

	glEnable(GL_TEXTURE_2D);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);
	
	renderer = new Renderer();
	renderer->Init();
	renderer->OnResize(cs.windowWidth, cs.windowHeight);

	physicsSimulator = new PhysicsSimulator;
	componentFactory = new ComponentFactory;
	scriptContainer = new ScriptContainer;
	entityManager = new EntityManager(this);

	PhysicsSimulator* ps = physicsSimulator;
	ScriptContainer* sc = scriptContainer;
	componentFactory->RegisterComponentType("render", [](Entity* ent, Json::Value obj) {return new RenderComponent(ent, obj); });
	componentFactory->RegisterComponentType("physics", [ps](Entity* ent, Json::Value obj) {return new PhysicsComponent(ent, ps, obj); });
	componentFactory->RegisterComponentType("damageDealer", [](Entity* ent, Json::Value obj) {return new DamageDealerComponent(ent, obj); });
	componentFactory->RegisterComponentType("health", [](Entity* ent, Json::Value obj) {return new HealthComponent(ent, obj); });
	componentFactory->RegisterComponentType("script", [sc](Entity* ent, Json::Value obj) {return new ScriptComponent(ent, sc, obj); });
	componentFactory->RegisterComponentType("spawner", [](Entity* ent, Json::Value obj) {return new SpawnerComponent(ent, obj); });

}

void SystemImpl::AddScript(std::string name, std::function<void(Entity*, Json::Value, float)> scr)
{
	scriptContainer->AddScript(name, scr);
}

void SystemImpl::Start()
{
	Json::Value entitiesJSON;
	Json::Reader reader;

	std::ifstream entity_file("../testentities.json", std::ifstream::binary);

	bool parsingSuccessful = reader.parse(entity_file, entitiesJSON, false);
	if (!parsingSuccessful)
	{
		// report to the user the failure and their locations in the document.
		std::cout << reader.getFormattedErrorMessages() << "\n";
	}

	Entity* player = new Entity(this, entitiesJSON["player"]);
	for (auto ent : entitiesJSON["entities"])
	{
		entityManager->AddEntity(ent);
	}

	bool done = false;
	
	bool accs[]={ false, false, false, false };
	float acc = 32;
	float maxspeed = 256;
	float friction = 16;
	Vector2 speed;
	Timer timer;
	float dt = 0.033f;
	while (!done) {
		renderer->BeginFrame();

		
		if (player != nullptr)
		{
			if (player->toBeDestroyed)
			{
				EntityDestroyedEvent ev;
				player->OnEvent(&ev);
				delete player;
				player = nullptr;
				continue;
			}

			player->Update(dt);
			entityManager->Update(dt);
			player->LateUpdate(dt);
			entityManager->LateUpdate(dt);
			
			if (speed.Magnitude() < friction) speed = Vector2();
			else speed = speed / speed.Magnitude() * (speed.Magnitude() - friction);

			if (accs[0] && accs[1])speed.y = 0;
			else if (accs[0]) speed.y -= acc;
			else if (accs[1]) speed.y += acc;
			if (accs[2] && accs[3])speed.x = 0;
			else if (accs[2]) speed.x -= acc;
			else if (accs[3]) speed.x += acc;
			if (speed.Magnitude() > maxspeed)speed = speed / speed.Magnitude() * maxspeed;
			player->transform->position = player->transform->position + speed * dt;
		}
		else
		{
			entityManager->Update(dt);
			entityManager->LateUpdate(dt);
		}
		physicsSimulator->CheckCollisions();


		SDL_Event evt;
		while (SDL_PollEvent(&evt)) {
			switch (evt.type)
			{
			case SDL_KEYDOWN:
			{
				if (evt.key.keysym.sym == SDLK_UP)
					accs[0] = true;
				if (evt.key.keysym.sym == SDLK_DOWN)
					accs[1] = true;
				if (evt.key.keysym.sym == SDLK_LEFT)
					accs[2] = true;
				if (evt.key.keysym.sym == SDLK_RIGHT)
					accs[3] = true;
				break;
			}
			case SDL_KEYUP:
			{
				if (evt.key.keysym.sym == SDLK_UP)
					accs[0] = false;
				if (evt.key.keysym.sym == SDLK_DOWN)
					accs[1] = false;
				if (evt.key.keysym.sym == SDLK_LEFT)
					accs[2] = false;
				if (evt.key.keysym.sym == SDLK_RIGHT)
					accs[3] = false;
				if (evt.key.keysym.sym == SDLK_r && player == nullptr)
					player = new Entity(this, entitiesJSON["player"]);
				break;
			}
			case SDL_WINDOWEVENT:
				switch (evt.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
				{
					SDL_Window *window = SDL_GetWindowFromID(evt.window.windowID);
					int w, h;
					SDL_GetWindowSize(window, &w, &h);
					renderer->OnResize(w, h);
					break;
				}
				case SDL_WINDOWEVENT_CLOSE:
				{
					SDL_Window *window = SDL_GetWindowFromID(evt.window.windowID);
					if (window)
					{
						SDL_DestroyWindow(window);
					}
					break;
				}
				break;
				}
				break;
			}
		}
		renderer->EndFrame();
		SDL_GL_MakeCurrent(m_window, m_context);
		SDL_GL_SwapWindow(m_window);
		dt = (float)timer.elapsed();
		timer.reset();
	}
};

}