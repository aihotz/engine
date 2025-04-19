#include "logger.hpp"
#include "core.hpp"

struct ProofGame : public iGameProject
{
	virtual void RegisterTypes() override
	{
		shared::Log("Registering types!");
	}

	virtual void Initialize() override
	{
		shared::Log("Game initialization!");
	}

	virtual void Shutdown() override
	{
		shared::Log("Game shutdown!");
	}
};

#ifdef DLL_BUILD
extern "C" __declspec(dllexport) iGameProject* GetGameProject()
{
    shared::Log("DLL endpoint!");
	return new ProofGame;
}
#else
int main(void)
{
    shared::Log("Standalone executable endpoint!");
	Engine engine;
	engine.SetGame(new ProofGame());
	engine.Run();
}
#endif