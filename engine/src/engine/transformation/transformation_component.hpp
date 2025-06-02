#ifndef TRANSFORMATION_COMPONENT_HPP
#define TRASFORMATION_COMPONENT_HPP

#include <game_object/component.hpp>
#include "transformation.hpp"

namespace engine
{
    class TransformationComponent : public Component
    {
        Transformation m_transformation;

      public:
        void SetParent(TransformationComponent*) { };
    };
} // namespace engine

#endif