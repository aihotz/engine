#include "rotation.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp> // glm::toMat4

engine::Rotation::Rotation()
    : m_rotation { glm::quat { 1.0f, 0.0f, 0.0f, 0.0f } }
{
}

engine::Rotation::Rotation(const glm::quat& quaternion)
    : m_rotation { glm::normalize(quaternion) }
{
}

engine::Rotation& engine::Rotation::operator=(const glm::quat& quaternion)
{
    m_rotation = glm::normalize(quaternion);
    return *this;
}

engine::Rotation::Rotation(glm::vec3 forward, glm::vec3 up)
{
    // normalize
    glm::vec3 forward_normalized = glm::normalize(forward);
    glm::vec3 up_normalized      = glm::normalize(up);

    // compute right vector and a new up vector
    glm::vec3 right_normalized = glm::normalize(glm::cross(forward_normalized, up_normalized));
    glm::vec3 up_corrected     = glm::cross(right_normalized, forward_normalized);

    // build the matrix and convert to quaternion
    glm::mat3 rotation_matrix = glm::mat3(right_normalized, up_corrected, forward_normalized);

    // glm::quat_cast should return a unit quaternion
    m_rotation = glm::quat_cast(rotation_matrix);
}

engine::Rotation::Rotation(glm::vec3 forward, glm::vec3 up, glm::vec3 right)
{
    // normalize vectors
    glm::vec3 forward_normalized = glm::normalize(forward);
    glm::vec3 up_normalized      = glm::normalize(up);
    glm::vec3 right_normalized   = glm::normalize(right);

    // build the matrix and convert to quaternion
    glm::mat3 rotation_matrix = glm::mat3(right_normalized, up_normalized, forward_normalized);

    // glm::quat_cast should return a unit quaternion
    m_rotation = glm::quat_cast(rotation_matrix);
}

// applied in ZYX order
engine::Rotation::Rotation(float x_angle_radians, float y_angle_radians, float z_angle_radians)
{
    m_rotation = glm::quat { glm::vec3 { x_angle_radians, y_angle_radians, z_angle_radians } };
}

glm::vec3 engine::Rotation::GetForwardVector() const
{
    return m_rotation * GLOBAL_FORWARD_VECTOR;
}

glm::vec3 engine::Rotation::GetUpVector() const
{
    return m_rotation * GLOBAL_UP_VECTOR;
}

glm::vec3 engine::Rotation::GetRightVector() const
{
    return m_rotation * GLOBAL_RIGHT_VECTOR;
}

// in radians
glm::vec3 engine::Rotation::GetEulerAngles() const
{
    return glm::eulerAngles(m_rotation);
}

glm::quat engine::Rotation::GetQuaternion() const
{
    return m_rotation;
}

engine::Rotation engine::Rotation::GetInverse() const
{
    // since we keep our quaternion normalized, conjugate is a faster inverse method
    // conjugate would not work if the quaternion was not normalized
    return Rotation { glm::conjugate(m_rotation) };
}

engine::Rotation engine::Rotation::operator*(const engine::Rotation& other) const
{
    // constructor normalized input quaternion
    return Rotation { m_rotation * other.m_rotation };
}

engine::Rotation& engine::Rotation::operator*=(const engine::Rotation& other)
{
    m_rotation = glm::normalize(m_rotation * other.m_rotation);
    return *this;
}

bool engine::Rotation::operator==(const engine::Rotation& other) const
{
    return glm::all(glm::epsilonEqual(m_rotation, other.m_rotation, glm::epsilon<float>()));
}

bool engine::Rotation::operator!=(const engine::Rotation& other) const
{
    return !(*this == other);
}

void engine::Rotation::RotateAroundAxis(float angle_radians, glm::vec3 axis)
{
    glm::quat q = glm::angleAxis(angle_radians, glm::normalize(axis));
    m_rotation  = glm::normalize(q * m_rotation);
}

glm::mat4 engine::Rotation::GetMatrix() const
{
    return glm::toMat4(m_rotation);
}

engine::Rotation engine::Rotation::Slerp(const engine::Rotation& start, const engine::Rotation& end, float t)
{
    return Rotation { glm::slerp(start.m_rotation, end.m_rotation, t) };
}