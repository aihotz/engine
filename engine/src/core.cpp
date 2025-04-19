#include "core.hpp"
#include "logger.hpp"

void Engine::Run()
{
    Initialize();
	Update();
	Shutdown();
}

void Engine::Initialize()
{
    shared::Log("Engine initialize!");
    m_gameProject->Initialize();
}

void Engine::Update()
{
#ifdef _DEBUG
    shared::Log("Engine update! We are in Debug!");
#else
    shared::Log("Engine update! We are in Release!");
#endif
}

void Engine::Shutdown()
{
    m_gameProject->Shutdown();
    shared::Log("Engine shutdown!");
}

void Engine::SetGame(iGameProject* gameProject)
{
	m_gameProject = gameProject;
    m_gameProject->RegisterTypes();
}