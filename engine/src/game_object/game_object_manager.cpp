#include "game_object_manager.hpp"

#include <stack>

#include "game_object.hpp"

engine::GameObjectManager& engine::GameObjectManager::GetInstance()
{
    static GameObjectManager instance {};
    return instance;
}

engine::GameObject* engine::GameObjectManager::AddGameObject(engine::GameObject* object)
{
    if (std::find(m_rootGameObjects.begin(), m_rootGameObjects.end(), object) != m_rootGameObjects.end())
    {
        throw std::runtime_error("Trying to add an object to the game object manager twice");
    }

    m_rootGameObjects.push_back(object);
    return object;
}

void engine::GameObjectManager::RemoveRootGameObject(engine::GameObject* object)
{
    m_rootGameObjects.remove(object);
}

void engine::GameObjectManager::Update()
{
    for (GameObject* object : m_gameObjectsMarkedAsDead)
    {
        GameObject* parent = object->GetParent();
        if (parent == nullptr)
        {
            m_rootGameObjects.remove(object);
        }
        else
        {
            parent->m_children.remove(object);
        }

        object->DestroyInternal();
    }

    m_gameObjectsMarkedAsDead.clear();
}

void engine::GameObjectManager::Shutdown()
{
    for (GameObject* object : m_rootGameObjects)
    {
        object->ShutdownEvents();
    }

    for (GameObject* object : m_rootGameObjects)
    {
        object->DestroyInternal();

        // todo: memory manager!
        delete object;
    }

    m_rootGameObjects.clear();
    m_gameObjectsMarkedAsDead.clear();
}

void engine::GameObjectManager::DestroyGameObject(engine::GameObject* object)
{
    if (object == nullptr)
    {
        throw std::runtime_error("Tried to destroy null game object");
    }

    // do not add an object twice
    if (std::find(m_gameObjectsMarkedAsDead.begin(), m_gameObjectsMarkedAsDead.end(), object) == m_gameObjectsMarkedAsDead.end())
    {
        m_gameObjectsMarkedAsDead.push_back(object);
    }
}

engine::GameObject* engine::GameObjectManager::CreateGameObject(const char* name)
{
    if (name == nullptr)
    {
        throw std::runtime_error("Tried to create a game object with a null name");
    }

    // todo: memory manager
    GameObject* newObject = new GameObject { name };
    m_rootGameObjects.push_back(newObject);
    return newObject;
}

engine::GameObject* engine::GameObjectManager::FindGameObjectByName(const char* name) const
{
    if (name == nullptr)
    {
        throw std::runtime_error("Tried to search game object with a null name");
    }
    
    GameObject* foundObject = nullptr;

    TraverseGameObjectsPreOrder(
        [ & ](GameObject* object)
        {
            if (object->GetName() == name)
            {
                foundObject = object;
                return false; // stop searching
            }

            return true;
        });

    return foundObject;
}

std::list<engine::GameObject*> engine::GameObjectManager::FindAllGameObjectsWithName(const char* name) const
{
    if (name == nullptr)
    {
        throw std::runtime_error("Tried to search game object with a null name");
    }
    
    std::list<engine::GameObject*> foundObjects;

    TraverseGameObjectsPreOrder(
        [ & ](GameObject* object)
        {
            if (object->GetName() == name)
            {
                foundObjects.push_back(object);
            }

            return true;
        });

    return foundObjects;
}

void engine::GameObjectManager::ForEachRootGameObject(const std::function<void(GameObject*)>& func) const
{
    for (GameObject* gameObject : m_rootGameObjects)
    {
        func(gameObject);
    }
}

void engine::GameObjectManager::TraverseGameObjectsPreOrder(const std::function<bool(GameObject*)>& func) const
{
    std::stack<GameObject*> stack;

    // add root objects in reverse order
    for (std::list<GameObject*>::const_reverse_iterator it = m_rootGameObjects.rbegin(); it != m_rootGameObjects.rend(); it++)
    {
        stack.push(*it);
    }

    while (stack.empty() == false)
    {
        GameObject* current = stack.top();
        stack.pop();

        // call the function
        bool shouldKeepIterating = func(current);
        if (shouldKeepIterating == false)
        {
            return;
        }

        // add children in reverse order
        const std::list<GameObject*>& children = current->GetChildren();
        for (std::list<GameObject*>::const_reverse_iterator it = children.rbegin(); it != children.rend(); it++)
        {
            stack.push(*it);
        }
    }
}