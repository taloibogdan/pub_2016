#include <Engine/ComponentSystem/ComponentSystem.h>
#include <System/SystemImpl.h>
#include <Engine/Util/json/json-forwards.h>
namespace Engine
{
	Entity::Entity(SystemImpl* sys, Json::Value obj)
	{
		system = sys;
		name = obj["name"].asString();
		transform = new TransformComponent(this, obj["transform"]);

		for (auto comp : obj["components"])
		{
			IComponent* c = system->componentFactory->CreateComponent(comp["type"].asString(), this, comp["data"]);
			RegisterComponent(c);
		}
	}

	TransformComponent::TransformComponent(Entity* ent, Json::Value obj)
	{
		m_entity = ent;
		position = Vector2(obj["position"]);
		GetEntity()->RegisterReceiver("collision", this);
	}

	RenderComponent::RenderComponent(Entity* ent, Json::Value obj)
	{
		m_entity = ent;
		std::string path = "../";
		path += obj["texture_file"].asString();

		texture = Engine::Texture::LoadTexture(&path[0]);
		int offsetx = 0, offsety = 0;
		if (obj["offsetx"] != NULL) offsetx = obj["offsetx"].asInt();
		if (obj["offsety"] != NULL) offsety = obj["offsety"].asInt();

		int wid = obj["width"].asInt();
		int hei = obj["height"].asInt();
		sprite = new Sprite(texture, offsetx, offsety, offsetx + wid, offsety + hei);
		GetEntity()->RegisterReceiver("invulnerability", this);
	}
	Collider::Collider(Json::Value obj)
	{
		type = obj["type"].asInt();
		centerRelative = Vector2(obj["center"]);
		if (type == 0)
		{
			radiusx = obj["radius"].asFloat();
			radiusy = radiusx;
		}
	}
	PhysicsComponent::PhysicsComponent(Entity* ent, PhysicsSimulator* psim, Json::Value obj)
	{
		m_entity = ent;
		m_physicsSimulator = psim;
		psim->AddComponent(this);
		collisionBitmask = obj["collisionBitmask"].asInt();
		isImmovable = obj["immovable"].asBool();
		collider = Collider(obj["collider"]);
		GetEntity()->RegisterReceiver("entityDestroyed", this);
	}
	DamageDealerComponent::DamageDealerComponent(Entity* ent, Json::Value obj)
	{
		m_entity = ent;
		damageBitmask = obj["damageBitmask"].asInt();
		damage = obj["damage"].asFloat();
		GetEntity()->RegisterReceiver("collision", this);
		//GetEntity()->RegisterReceiver("damageRequest", this);
	}
	HealthComponent::HealthComponent(Entity* ent, Json::Value obj)
	{
		m_entity = ent;
		healthBitmask = obj["healthBitmask"].asInt();
		health = obj["health"].asFloat();
		damageCD = obj["damageCD"].asFloat();
		GetEntity()->RegisterReceiver("damage", this);
	}
	ScriptComponent::ScriptComponent(Entity* ent, ScriptContainer* sc, Json::Value obj)
	{
		m_entity = ent;
		scriptName = obj["scriptName"].asString();
		scriptContainer = sc;
		settings = obj["settings"];
	}
	SpawnerComponent::SpawnerComponent(Entity* ent, Json::Value obj)
	{
		m_entity = ent;
		offset = Vector2(obj["offset"]);
		area = Vector2(obj["area"]);
		spawnCD = obj["spawnCD"].asFloat();
		for (auto ent : obj["entitiesToSpawn"])
			entitiesToSpawn.push_back(ent);
	}
}