#include "game_object.hpp"

#include <stdexcept>

#include "component.hpp"
#include "game_object_manager.hpp"

engine::GameObject::GameObject(const std::string& name)
    : m_name { name }
    , m_parent { nullptr }
{
}

void engine::GameObject::DestroyInternal()
{
    for (GameObject* child : m_children)
    {
        child->ShutdownEvents();
    }

    for (GameObject* child : m_children)
    {
        child->DestroyInternal();

        // todo: memory manager
        delete child;
    }

    for (Component* component : m_components)
    {
        component->Shutdown();

        // todo: memory manager
        delete component;
    }

    m_children.clear();
    m_components.clear();
}

void engine::GameObject::DetachChild(engine::GameObject* child)
{
    if (child == nullptr)
    {
        throw std::runtime_error("Tried to detach a null child");
    }

    if (std::find(m_children.begin(), m_children.end(), child) == m_children.end())
    {
        throw std::runtime_error("Tried to remove a child from a game object that does not own it");
    }

    m_children.remove(child);
    child->MakeParent();
}

void engine::GameObject::ShutdownEvents()
{
    for (Component* component : m_components)
    {
        component->ShutdownEvents();
    }
}

void engine::GameObject::Initialize()
{
    for (Component* component : m_components)
    {
        component->Initialize();
    }

    for (GameObject* child : m_children)
    {
        child->Initialize();
    }
}

void engine::GameObject::Shutdown()
{
    GameObjectManager::GetInstance().DestroyGameObject(this);
}

engine::GameObject* engine::GameObject::AddChild(engine::GameObject* child)
{
    if (child == nullptr)
    {
        throw std::runtime_error("Tried to add a null child");
    }

    if (child == this)
    {
        throw std::runtime_error("Tried to make a game object children of itself");
    }

    if (child->GetParent() == nullptr)
    {
        GameObjectManager::GetInstance().RemoveRootGameObject(child);
    }

    m_children.push_back(child);
    child->SetParent(this);
    return child;
}

engine::GameObject* engine::GameObject::CreateChild(const std::string& name)
{
    // todo: memory manager
    GameObject* newChild = new GameObject { name };
    AddChild(newChild);
    return newChild;
}

void engine::GameObject::RemoveChild(engine::GameObject* child)
{
    if (child == nullptr)
    {
        throw std::runtime_error("Tried to remove a null child");
    }

    if (std::find(m_children.begin(), m_children.end(), child) == m_children.end())
    {
        throw std::runtime_error("Tried to remove a child from a game object that does not own it");
    }

    child->Shutdown();
}

const std::list<engine::GameObject*>& engine::GameObject::GetChildren() const
{
    return m_children;
}

engine::GameObject* engine::GameObject::GetChild(size_t index) const
{
    if (index >= m_children.size())
    {
        throw std::out_of_range("Child index out of range");
    }

    std::list<GameObject*>::const_iterator it = m_children.begin();
    std::advance(it, index);

    return *it;
}

engine::GameObject* engine::GameObject::GetParent() const
{
    return m_parent;
}

void engine::GameObject::SetParent(engine::GameObject* parent)
{
    // detach from current parent
    if (m_parent != nullptr)
    {
        m_parent->DetachChild(this);
    }

    // if previously an orfan and now has a parent
    if (m_parent == nullptr && parent != nullptr)
    {
        GameObjectManager::GetInstance().RemoveRootGameObject(this);
    }
    else if (m_parent != nullptr && parent == nullptr)
    {
        // similarly, if the object becomes orfan

        GameObjectManager::GetInstance().AddGameObject(this);
    }

    m_parent = parent;

    // if the object has a transformation set the parent transformation accordingly
    if (TransformationComponent* transformation_component = GetComponent<TransformationComponent>())
    {
        if (m_parent == nullptr)
        {
            transformation_component->SetParent(nullptr);
        }
        else
        {
            transformation_component->SetParent(m_parent->GetComponent<TransformationComponent>());
        }
    }
}

void engine::GameObject::MakeParent()
{
    SetParent(nullptr);
}

void engine::GameObject::SetName(const std::string& name)
{
    m_name = name;
}

std::string engine::GameObject::GetName() const
{
    return m_name;
}

const std::list<engine::Component*>& engine::GameObject::GetAllComponents() const
{
    return m_components;
}

engine::Component* engine::GameObject::AddComponent(engine::Component* component)
{
    if (component == nullptr)
    {
        throw std::runtime_error("Tried to add a null component");
    }

    if (component->GetOwner() != nullptr)
    {
        throw std::runtime_error("Tried to add a component that is already owned by a game object");
    }

    component->m_owner = this;
    m_components.push_back(component);

    component->Create();

    return component;
}

void engine::GameObject::RemoveComponent(engine::Component* component)
{
    if (component == nullptr)
    {
        throw std::runtime_error("Tried to remove a null component");
    }

    if (std::find(m_components.begin(), m_components.end(), component) == m_components.end())
    {
        throw std::runtime_error("Tried to remove a component from a game object that does not own it");
    }

    component->Shutdown();

    m_components.remove(component);

    // todo: memory manager
    delete component;
}