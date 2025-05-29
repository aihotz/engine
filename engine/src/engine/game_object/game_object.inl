#include "game_object.hpp"

template <typename T>
T* engine::GameObject::GetComponent() const
{
    for (Component* component : m_components)
    {
        if (T* castedComponent = dynamic_cast<T*>(component))
        {
            return castedComponent;
        }
    }

    return nullptr;
}

template <typename T>
std::list<T*> engine::GameObject::GetAllComponents() const
{
    std::list<T*> components;

    for (Component* component : m_components)
    {
        if (T* castedComponent = dynamic_cast<T*>(component))
        {
            components.push_back(castedComponent);
        }
    }

    return components;
}

template <typename T>
T* engine::GameObject::AddComponent()
{
    // todo: memory manager
    T* component = new T {};

    component->m_owner = this;
    m_components.push_back(component);

    component->Create();

    return component;
}

template <typename T>
void engine::GameObject::RemoveComponent()
{
    T* component = GetComponent<T>();

    if (component == nullptr)
    {
        return;
    }

    component->Shutdown();

    m_components.remove(component);

    // todo: memory manager
    delete component;
}

template <typename T>
void engine::GameObject::RemoveAllComponents()
{
    for (T* component : m_components)
    {
        component->Shutdown();

        // todo: memory manager
        delete component;
    }

    m_components.clear();
}