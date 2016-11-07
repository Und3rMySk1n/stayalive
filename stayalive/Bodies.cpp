#include "stdafx.h"
#include "Bodies.h"
#include <stdint.h>

namespace
{
typedef glm::vec3 Vertex;

struct STriangleFace
{
    uint16_t vertexIndex1;
    uint16_t vertexIndex2;
    uint16_t vertexIndex3;
    uint16_t colorIndex;
};

// Вершины куба служат материалом для формирования треугольников,
// составляющих грани куба.
const Vertex CUBE_VERTICIES[] = {
    {-1, +1, -1},
    {+1, +1, -1},
    {+1, -1, -1},
    {-1, -1, -1},
    {-1, +1, +1},
    {+1, +1, +1},
    {+1, -1, +1},
    {-1, -1, +1},
};

// Сторона тетраэдра равна √3,
// расстояние от центра грани до вершины равно 1.
const Vertex TETRAHEDRON_VERTICES[] = {
    {0.f, 0.f, -1.0f},
    {sqrtf(1.5f), 0.f, 0.5f},
    {-sqrtf(1.5f), 0.f, 0.5f},
    {0.f, sqrtf(2.f), 0.f},
};

// Привыкаем использовать 16-битный unsigned short,
// чтобы экономить память на фигурах с тысячами вершин.
const STriangleFace CUBE_FACES[] = {
    {0, 1, 2, static_cast<uint16_t>(CubeFace::Back)},
    {0, 2, 3, static_cast<uint16_t>(CubeFace::Back)},
    {2, 1, 5, static_cast<uint16_t>(CubeFace::Right)},
    {2, 5, 6, static_cast<uint16_t>(CubeFace::Right)},
    {3, 2, 6, static_cast<uint16_t>(CubeFace::Bottom)},
    {3, 6, 7, static_cast<uint16_t>(CubeFace::Bottom)},
    {0, 3, 7, static_cast<uint16_t>(CubeFace::Left)},
    {0, 7, 4, static_cast<uint16_t>(CubeFace::Left)},
    {1, 0, 4, static_cast<uint16_t>(CubeFace::Top)},
    {1, 4, 5, static_cast<uint16_t>(CubeFace::Top)},
    {6, 5, 4, static_cast<uint16_t>(CubeFace::Front)},
    {6, 4, 7, static_cast<uint16_t>(CubeFace::Front)},
};

const STriangleFace TETRAHEDRON_FACES[] = {
    {0, 1, 2, 0},
    {0, 3, 1, 0},
    {2, 1, 3, 0},
    {0, 2, 3, 0},
};
}

CIdentityCube::CIdentityCube()
    : m_alpha(1)
{
    // Используем белый цвет по умолчанию.
    for (glm::vec3 &color : m_colors)
    {
        color.x = 1;
        color.y = 1;
        color.z = 1;
    }
}

void CIdentityCube::Draw() const
{
    if (m_alpha < 0.99f)
    {
        glFrontFace(GL_CW);
        OutputFaces();
        glFrontFace(GL_CCW);
    }
    OutputFaces();
}

void CIdentityCube::SetFaceColor(CubeFace face, const glm::vec3 &color)
{
    const size_t index = static_cast<size_t>(face);
    assert(index < COLORS_COUNT);
    m_colors[index] = color;
}

void CIdentityCube::SetAlpha(float alpha)
{
    m_alpha = alpha;
}

void CIdentityCube::OutputFaces() const
{
    // менее оптимальный способ рисования: прямая отправка данных
    // могла бы работать быстрее, чем множество вызовов glColor/glVertex.
    glBegin(GL_TRIANGLES);

    for (const STriangleFace &face : CUBE_FACES)
    {
        const Vertex &v1 = CUBE_VERTICIES[face.vertexIndex1];
        const Vertex &v2 = CUBE_VERTICIES[face.vertexIndex2];
        const Vertex &v3 = CUBE_VERTICIES[face.vertexIndex3];
        glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
        glm::vec3 color = m_colors[face.colorIndex];

        glColor4f(color.x, color.y, color.z, m_alpha);
        glNormal3fv(glm::value_ptr(normal));
        glVertex3fv(glm::value_ptr(v1));
        glVertex3fv(glm::value_ptr(v2));
        glVertex3fv(glm::value_ptr(v3));
    }
    glEnd();
}

void CIdentityTetrahedron::Draw() const
{
    if (m_color.a < 0.99f)
    {
        glFrontFace(GL_CW);
        OutputFaces();
        glFrontFace(GL_CCW);
    }
    OutputFaces();
}

void CIdentityTetrahedron::SetColor(const glm::vec4 &color)
{
    m_color = color;
}

void CIdentityTetrahedron::OutputFaces() const
{
    // менее оптимальный способ рисования: прямая отправка данных
    // могла бы работать быстрее, чем множество вызовов glColor/glVertex.
    glBegin(GL_TRIANGLES);

    for (const STriangleFace &face : TETRAHEDRON_FACES)
    {
        const Vertex &v1 = TETRAHEDRON_VERTICES[face.vertexIndex1];
        const Vertex &v2 = TETRAHEDRON_VERTICES[face.vertexIndex2];
        const Vertex &v3 = TETRAHEDRON_VERTICES[face.vertexIndex3];
        glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

        glColor4fv(glm::value_ptr(m_color));
        glNormal3fv(glm::value_ptr(normal));
        glVertex3fv(glm::value_ptr(v1));
        glVertex3fv(glm::value_ptr(v2));
        glVertex3fv(glm::value_ptr(v3));
    }
    glEnd();
}

CSphereQuadric::CSphereQuadric()
    : m_quadric(gluNewQuadric())
    , m_color({1, 1, 1})
{
}

CSphereQuadric::~CSphereQuadric()
{
    gluDeleteQuadric(m_quadric);
}

void CSphereQuadric::Draw() const
{
    const double radius = 1;
    const int slices = 20;
    const int stacks = 20;
    glColor3fv(glm::value_ptr(m_color));
    gluSphere(m_quadric, radius, slices, stacks);
}

void CSphereQuadric::SetColor(const glm::vec3 &color)
{
    m_color = color;
}

CConoidQuadric::CConoidQuadric()
    : m_quadric(gluNewQuadric())
    , m_color({1, 1, 1})
{
}

CConoidQuadric::~CConoidQuadric()
{
    gluDeleteQuadric(m_quadric);
}

// Рисует усечённый конус высотой 2,
// с радиусом основания 1 и радиусом верхнего торца m_topRadius.
void CConoidQuadric::Draw() const
{
    const double baseRadius = 1;
    const double height = 2;
    const int slices = 20;
    const int stacks = 1;
    glColor3fv(glm::value_ptr(m_color));
    glTranslatef(0, 0, 1);
    gluCylinder(m_quadric, baseRadius, m_topRadius, height, slices, stacks);
    glFrontFace(GL_CW);
    gluDisk(m_quadric, 0, baseRadius, slices, stacks);
    glFrontFace(GL_CCW);
    glTranslatef(0, 0, 2);
    gluDisk(m_quadric, 0, baseRadius, slices, stacks);
    glTranslatef(0, 0, -1);
}

void CConoidQuadric::SetTopRadius(double value)
{
    m_topRadius = glm::clamp(value, 0.0, 1.0);
}

void CConoidQuadric::SetColor(const glm::vec3 &color)
{
    m_color = color;
}
