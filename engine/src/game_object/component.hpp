#ifndef COMPONENT_HPP
#define COMPONENT_HPP

namespace engine
{
    class GameObject;

    class Component
    {
        friend class GameObject;

        GameObject* m_owner;

        Component();
        Component(const Component&)            = delete;
        Component& operator=(const Component&) = delete;

        void Create();
        void Shutdown();

    protected:

        virtual void AddToSystem();
        virtual void RemoveFromSystem();

        virtual void OnCreate();
        virtual void Initialize();
        virtual void Update();
        virtual void OnShutdown();
        virtual void ShutdownEvents();

      public:

        virtual ~Component() = 0;
        GameObject* GetOwner() const;
    };
} // namespace engine

#endif