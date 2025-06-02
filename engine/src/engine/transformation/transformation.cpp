#include "transformation.hpp"

#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>

engine::Transformation::Transformation(glm::vec3 position, Rotation rotation, glm::vec3 scale)
    : m_position { position }
    , m_rotation { rotation }
    , m_scale { scale }
{
}

glm::mat4 engine::Transformation::GetMatrix() const
{
    glm::mat4 translation_matrix = glm::translate(glm::mat4 { 1.0f }, m_position);
    glm::mat4 rotation_matrix    = m_rotation.GetMatrix();
    glm::mat4 scale_matrix       = glm::scale(glm::mat4 { 1.0f }, m_scale);

    return translation_matrix * rotation_matrix * scale_matrix;
}

engine::Transformation engine::Transformation::GetInverse() const
{
    if (glm::any(glm::epsilonEqual(m_scale, glm::vec3 { 0.0f }, glm::epsilon<float>())))
    {
        throw std::runtime_error("Division by zero when calculating inverse transformation");
    }

    glm::vec3 inverse_scale = 1.0f / m_scale;

    Rotation inverse_rotation = m_rotation.GetInverse();

    glm::vec3 inverse_position = -(inverse_rotation.GetQuaternion() * (inverse_scale * m_position));

    return Transformation(inverse_position, inverse_rotation, inverse_scale);
}

engine::Transformation engine::Transformation::Concatenate(const engine::Transformation& child_local) const
{
    return *this * child_local;
}

engine::Transformation engine::Transformation::operator*(const engine::Transformation& child_local) const
{
    // scale the child position according to parent scale
    glm::vec3 scaled_position = m_scale * child_local.m_position;

    // rotate child around parent space
    glm::vec3 rotated_position = m_rotation.GetQuaternion() * scaled_position;

    // add the parent position to compute the final position
    glm::vec3 new_position = m_position + rotated_position;

    // scale and rotation are simply concatenated
    Rotation  new_rotation = m_rotation * child_local.m_rotation;
    glm::vec3 new_scale    = m_scale * child_local.m_scale;

    return Transformation(new_position, new_rotation, new_scale);
}

engine::Transformation engine::Transformation::InverseConcatenate(const engine::Transformation& child_world) const
{
    return GetInverse() * child_world;
}

bool engine::Transformation::operator==(const engine::Transformation& other) const
{
    return glm::all(glm::epsilonEqual(m_position, other.m_position, glm::epsilon<float>())) 
        && m_rotation == other.m_rotation 
        && glm::all(glm::epsilonEqual(m_scale, other.m_scale, glm::epsilon<float>()));
}

bool engine::Transformation::operator!=(const engine::Transformation& other) const
{
    return !(*this == other);
}

void engine::Transformation::LookAt(glm::vec3 target)
{
    glm::vec3 direction = glm::normalize(target - m_position);
    m_rotation          = Rotation(direction);
}

void engine::Transformation::RotateAroundAxis(float angle_radians, glm::vec3 axis)
{
    glm::quat q = glm::angleAxis(angle_radians, glm::normalize(axis));
    m_rotation  = Rotation { q * m_rotation.GetQuaternion() };
    m_position  = q * m_position;
}

engine::Transformation engine::Transformation::Lerp(const engine::Transformation& start, const engine::Transformation& end, float t)
{
    glm::vec3 lerped_position = glm::mix(start.m_position, end.m_position, t);
    Rotation  lerped_rotation = Rotation::Slerp(start.m_rotation, end.m_rotation, t);
    glm::vec3 lerped_scale    = glm::mix(start.m_scale, end.m_scale, t);

    return Transformation { lerped_position, lerped_rotation, lerped_scale };
}