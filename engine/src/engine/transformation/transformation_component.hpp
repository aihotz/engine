#ifndef TRANSFORMATION_COMPONENT_HPP
#define TRASFORMATION_COMPONENT_HPP

#include <game_object/component.hpp>

namespace engine
{
    class TransformationComponent : public Component
    {
      public:
        void SetParent(TransformationComponent*) { };
    };
} // namespace engine

#endif