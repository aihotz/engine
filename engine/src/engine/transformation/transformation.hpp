#ifndef TRANSFORMATION_HPP
#define TRANSFORMATION_HPP

#include <glm/glm.hpp>

#include "rotation.hpp"

namespace engine
{
    struct Transformation
    {
        glm::vec3 m_position;
        Rotation  m_rotation;
        glm::vec3 m_scale;

        Transformation(
            glm::vec3 position = glm::vec3 { 0.0f, 0.0f, 0.0f },
            Rotation  rotation = Rotation {},
            glm::vec3 scale    = glm::vec3 { 1.0f, 1.0f, 1.0f });

        glm::mat4 GetMatrix() const;

        Transformation GetInverse() const;
        Transformation Concatenate(const Transformation& child_local) const;
        Transformation operator*(const Transformation& child_local) const;
        Transformation InverseConcatenate(const Transformation& child_world) const;

        bool operator==(const Transformation& other) const;
        bool operator!=(const Transformation& other) const;

        void LookAt(glm::vec3 target);
        void RotateAroundAxis(float angle_radians, glm::vec3 axis);

        static Transformation Lerp(const Transformation& start, const Transformation& end, float t);
    };
} // namespace engine

#endif