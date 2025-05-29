#ifndef CORE_HPP
#define CORE_HPP

struct iGameProject
{
    virtual void RegisterTypes() = 0;
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;
};

class Engine
{
	iGameProject* m_gameProject;

    void Initialize();
    void Update();
    void Shutdown();

  public:
    void Run();
    void SetGame(iGameProject* gameProject);
};

#endif