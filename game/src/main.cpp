#undef main
#include <Engine/System/System.h>
#include <Engine/ComponentSystem/Components.h>

void AutoMove(Engine::Entity* entity, Json::Value settings, float dt)
{
	entity->transform->position = entity->transform->position + (Vector2(settings["speed"]) * dt);
}
int main(int argc, char **argv)
{
	auto system = new Engine::System();	
	system->Init({ 1024, 768, "Test" });

	system->AddScript("autoMove", AutoMove);

	system->Start();
	return 0;
}