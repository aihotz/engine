#include "core.hpp"
#include "logger.hpp"

int main(void)
{
    shared::Log("We are in the editor now!");

    Engine engine;
    engine.Initialize();
    engine.Update();
    engine.Shutdown();
}