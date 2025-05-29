#ifndef GAME_OBJECT_MANAGER_HPP
#define GAME_OBJECT_MANAGER_HPP

#include <functional>
#include <list>

namespace engine
{
    class GameObject;

    class GameObjectManager
    {
        friend class GameObject;

        std::list<GameObject*> m_rootGameObjects;
        std::list<GameObject*> m_gameObjectsMarkedAsDead;

        GameObjectManager()                                    = default;
        ~GameObjectManager()                                   = default;
        GameObjectManager(const GameObjectManager&)            = delete;
        GameObjectManager& operator=(const GameObjectManager&) = delete;

        GameObject* AddGameObject(GameObject* object);
        void        RemoveRootGameObject(GameObject* object);

      public:
        static GameObjectManager& GetInstance();

        void Update();
        void Shutdown();

        void DestroyGameObject(GameObject* object);

        GameObject*            CreateGameObject(const char* name = "Game Object");
        GameObject*            FindGameObjectByName(const char* name) const;
        std::list<GameObject*> FindAllGameObjectsWithName(const char* name) const;

        void ForEachRootGameObject(const std::function<void(GameObject*)>& func) const;
        void TraverseGameObjectsPreOrder(const std::function<bool(GameObject*)>& func) const;
    };
} // namespace engine

#endif