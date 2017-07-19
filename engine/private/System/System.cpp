#include "System/SystemImpl.h"
#include <Engine/System/System.h>
#include <type_traits>
#include<Engine/ComponentSystem/ScriptContainer.h>

namespace Engine
{
System::System()
:	m_impl(new SystemImpl())
{
	
};

System::~System()
{

}

void System::AddScript(std::string name, std::function<void(Entity*, Json::Value, float)> scr)
{
	m_impl->AddScript(name, scr);
}

void System::Init(SCreationSettings&& cs)
{
	m_impl->Init(std::forward<SCreationSettings>(cs));
}
void System::Start()
{
	m_impl->Start();
}
}