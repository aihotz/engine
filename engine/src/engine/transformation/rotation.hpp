#ifndef ROTATION_HPP
#define ROTATION_HPP

#include <glm/gtc/quaternion.hpp>

namespace engine
{
    class Rotation
    {
        glm::quat m_rotation;

      public:
        static constexpr glm::vec3 GLOBAL_UP_VECTOR      = glm::vec3 { 0.0f, 1.0f, 0.0f };
        static constexpr glm::vec3 GLOBAL_FORWARD_VECTOR = glm::vec3 { 0.0f, 0.0f, -1.0f };
        static constexpr glm::vec3 GLOBAL_RIGHT_VECTOR   = glm::vec3 { 1.0f, 0.0f, 0.0f };

        Rotation();
        Rotation(const glm::quat& quaternion);
        Rotation& operator=(const glm::quat& quaternion);

        Rotation(glm::vec3 forward, glm::vec3 up = GLOBAL_UP_VECTOR);
        Rotation(glm::vec3 forward, glm::vec3 up, glm::vec3 right);

        // applied in ZYX order
        Rotation(float x_angle_radians, float y_angle_radians, float z_angle_radians); 

        glm::vec3 GetForwardVector() const;
        glm::vec3 GetUpVector() const;
        glm::vec3 GetRightVector() const;

        // in radians
        glm::vec3 GetEulerAngles() const;

        glm::quat GetQuaternion() const;

        Rotation  GetInverse() const;
        Rotation  operator*(const Rotation& other) const;
        Rotation& operator*=(const Rotation& other);

        bool operator==(const Rotation& other) const;
        bool operator!=(const Rotation& other) const;

        void RotateAroundAxis(float angle_radians, glm::vec3 axis);

        glm::mat4 GetMatrix() const;

        static Rotation Slerp(const Rotation& start, const Rotation& end, float t);
    };
} // namespace engine

#endif