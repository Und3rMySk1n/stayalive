#pragma once

#include "lib.h"
#include <functional>
#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include "MeshP3NT2.h"

// Вершина с трёхмерной позицией и нормалью.
struct SVertexP3N
{
    glm::vec3 position;
    glm::vec3 normal;

    SVertexP3N() = default;
    SVertexP3N(const glm::vec3 &position)
        : position(position)
    {
    }
};

class CSolidFunctionSurface final : public ISceneObject
{
public:
    CSolidFunctionSurface();

    // Инициализирует индексированную сетку треугольников
    void Tesselate(unsigned slices, unsigned stacks);

    // IBody interface.
    void Update(float) final {}
    void Draw() const final;

private:
    std::vector<SVertexP3NT2> m_vertices;
    std::vector<uint32_t> m_indicies;
};
