#include <Engine/ComponentSystem/ComponentSystem.h>
#include <Engine/ComponentSystem/ScriptContainer.h>
#include <System/SystemImpl.h>
#include <Engine/Rendering/OpenGLRenderer.h>
namespace Engine
{
	void TransformComponent::OnEvent(Event* event)
	{
		if (event->GetName() == "collision")
		{
			CollisionEvent* cEvent = static_cast<CollisionEvent*>(event);
			position = position + cEvent->reaction;
		}
	}

	void RenderComponent::LateUpdate(float dt)
	{
		if (blinkingCD <= 0 && isBlinking)
		{
			isVisible = !isVisible;
			if (isVisible) blinkingCD = blinkingCD0Max;
			else blinkingCD = blinkingCD1Max;
		}
		if (isVisible) Render();
		if (isBlinking) blinkingCD -= dt;
	}
	void RenderComponent::OnEvent(Event* event)
	{
		if (event->GetName() == "invulnerability")
		{
			InvulnerabilityEvent* iEvent = static_cast<InvulnerabilityEvent*>(event);
			isBlinking = iEvent->isStart;
			isVisible = !isBlinking;
			blinkingCD0Max = iEvent->blinkingCD0;
			blinkingCD1Max = iEvent->blinkingCD1;
		}
	}
	void RenderComponent::Render()
	{
		if (sprite != nullptr)
		{
			GetEntity()->system->renderer->RenderSprite(*sprite, int(GetEntity()->transform->position.x), int(GetEntity()->transform->position.y));
		}
	}
	void PhysicsComponent::OnEvent(Event* event)
	{
		if (event->GetName() == "entityDestroyed")
		{
			m_physicsSimulator->RemoveComponent(this);
			std::cout << "Removed " << GetEntity()->name << " physics component" << std::endl;
		}
	}
	void PhysicsComponent::CheckCollision(PhysicsComponent* other)
	{
		if (other->GetEntity() == nullptr) return;
		if (collider.type == 0 && other->collider.type == 0)
		{
			Vector2 dif = other->GetEntity()->transform->position - GetEntity()->transform->position + other->collider.centerRelative - collider.centerRelative;
			float rad = collider.radiusx + other->collider.radiusx;
			if (dif.Magnitude() > rad) return;

			dif = dif / dif.Magnitude()*(rad - dif.Magnitude());
			Vector2 d1 = dif / (-2), d2 = dif / 2;
			if (isImmovable) { d1 = Vector2(0, 0); d2 = d2 * 2; }
			else if (other->isImmovable) { d2 = Vector2(0, 0); d1 = d1 * 2; }

			GetEntity()->OnEvent(new CollisionEvent(other->GetEntity(), d1));
			other->GetEntity()->OnEvent(new CollisionEvent(GetEntity(), d2));
		}
	}

	void DamageDealerComponent::OnEvent(Event* event)
	{/*
	 if (event->GetName() == "damageRequest")
	 {
	 GetEntity()->requestOutput.clear();
	 GetEntity()->requestOutput["damage"] = damage;
	 GetEntity()->requestOutput["damageBitmask"] = damageBitmask;
	 }*/
		if (event->GetName() == "collision")
		{
			CollisionEvent* cEvent = static_cast<CollisionEvent*>(event);
			cEvent->other->OnEvent(new DamageEvent(damage, damageBitmask));
		}
	}

	void HealthComponent::OnEvent(Event* event)
	{
		if (event->GetName() == "damage" && currentDamageCD <= 0)
		{
			DamageEvent* dEvent = static_cast<DamageEvent*>(event);
			if ((healthBitmask & dEvent->bitmask) != 0)
			{
				health -= dEvent->damage;
				std::cout << GetEntity()->name << " damaged! HP: " << (int)health << std::endl;
				currentDamageCD = damageCD;
				GetEntity()->OnEvent(new InvulnerabilityEvent(true, 0.1f, 0.1f));
			}
			if (health <= 0) GetEntity()->toBeDestroyed = true;
		}
	}
	void HealthComponent::LateUpdate(float dt)
	{
		if (GetEntity()->transform->position.x < 0) GetEntity()->toBeDestroyed = true;
		if (currentDamageCD <= 0)return;
		currentDamageCD -= dt;
		if (currentDamageCD <= 0)
			GetEntity()->OnEvent(new InvulnerabilityEvent(false, 0, 0));
	}
	void ScriptComponent::Update(float dt)
	{
		scriptContainer->RunScript(scriptName, GetEntity(), settings, dt);
	}
	
	void SpawnerComponent::Update(float dt)
	{
		currentSpawnCD -= dt;
		if (currentSpawnCD <= 0)
		{
			Vector2 pos(((std::rand() % 100) / 100.0f)*area.x, ((std::rand() % 100) / 100.0f)*area.y);
			pos = pos - (area / 2) + offset + GetEntity()->transform->position;
			Entity* ent = GetEntity()->system->entityManager->AddEntity(entitiesToSpawn[nextEntityID]);
			ent->transform->position = pos;
			currentSpawnCD = spawnCD;
			nextEntityID++;
			if (nextEntityID == std::size(entitiesToSpawn)) nextEntityID = 0;
		}
	}
	
}