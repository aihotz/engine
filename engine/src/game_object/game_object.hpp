#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <list>
#include <string>

namespace engine
{
    class Component;
    class GameObjectManager;

    class GameObject
    {
        friend class GameObjectManager;

        std::string            m_name;
        GameObject*            m_parent;
        std::list<GameObject*> m_children;
        std::list<Component*>  m_components;

        GameObject(const std::string& name = "Game Object");
        GameObject(const GameObject& other)            = delete;
        GameObject& operator=(const GameObject& other) = delete;

        void DestroyInternal();
        void DetachChild(GameObject* child);

        void ShutdownEvents();

      public:
        void Initialize();
        void Shutdown();

        GameObject*                   AddChild(GameObject* child);
        GameObject*                   CreateChild(const std::string& name = "Game Object");
        void                          RemoveChild(GameObject* child);
        const std::list<GameObject*>& GetChildren() const;
        GameObject*                   GetChild(size_t index) const;

        GameObject* GetParent() const;
        void        SetParent(GameObject* parent);
        void        MakeParent();

        void        SetName(const std::string& name);
        std::string GetName() const;

        template <typename T>
        T* GetComponent() const;

        template <typename T>
        std::list<T*> GetAllComponents() const;

        const std::list<Component*>& GetAllComponents() const;

        template <typename T>
        T* AddComponent();

        Component* AddComponent(Component* component);

        template <typename T>
        void RemoveComponent();

        template <typename T>
        void RemoveAllComponents();

        void RemoveComponent(Component* component);
    };
} // namespace engine

#include "game_object.inl"

#endif