#ifndef COMPONENT_HPP
#define COMPONENT_HPP

namespace engine
{
    class GameObject;

    class Component
    {
      public:
        GameObject* m_owner;

        virtual void Create() { };
        void Initialize() { };
        void Shutdown() { };
        void ShutdownEvents() { };

        GameObject* GetOwner() const
        {
            return m_owner;
        }
    };

    // todo:
    class TransformationComponent : public Component
    {
      public:
        void SetParent(TransformationComponent*) { };
    };

} // namespace engine

#include "component.inl"

#endif