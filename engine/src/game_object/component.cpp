#include "component.hpp"

engine::Component::Component()
    : m_owner(nullptr)
{
}

void engine::Component::Create()
{
    AddToSystem();
    OnCreate();
}

void engine::Component::Shutdown()
{
    ShutdownEvents();
    OnShutdown();
    RemoveFromSystem();
}

engine::GameObject* engine::Component::GetOwner() const
{
    return m_owner;
}

// virtual interface methods
void engine::Component::AddToSystem() { }
void engine::Component::RemoveFromSystem() { }
void engine::Component::OnCreate() { }
void engine::Component::Initialize() { }
void engine::Component::Update() { }
void engine::Component::OnShutdown() { }
void engine::Component::ShutdownEvents() { }