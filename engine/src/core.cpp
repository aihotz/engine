#include "core.hpp"
#include "logger.hpp"

void Engine::Initialize()
{
    shared::Log("Engine initialize!");
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
    shared::Log("Engine shutdown!");
}