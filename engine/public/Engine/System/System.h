#pragma once
#include <iostream>
#include <memory>
#include <functional>
#include <Engine/Util/json/json.h>
#include <Engine/Config.h>
#include "CreationSettigns.h"
namespace Engine
{
	class Entity;
	class SystemImpl;
	class ENGINE_API System
	{
	public:
		System();
		~System();

		void Init(SCreationSettings&& cs);
		void Start();

		void AddScript(std::string name, std::function<void(Entity*, Json::Value, float)> scr);

	protected:
		std::unique_ptr<SystemImpl> m_impl;
	};

}