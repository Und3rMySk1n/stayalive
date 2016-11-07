#include "stdafx.h"
#include "FunctionSurface.h"
#include <algorithm>

namespace
{
/// Привязывает вершины к состоянию OpenGL,
/// затем вызывает 'callback'.
template <class T>
void DoWithBindedArrays(const std::vector<SVertexP3NT2> &vertices, T && callback)
{
	// Включаем режим vertex array и normal array.
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	// Выполняем привязку vertex array и normal array
	const size_t stride = sizeof(SVertexP3NT2);
	glNormalPointer(GL_FLOAT, stride, glm::value_ptr(vertices[0].normal));
	glVertexPointer(3, GL_FLOAT, stride, glm::value_ptr(vertices[0].position));

	// Выполняем внешнюю функцию.
	callback();

	// Выключаем режим vertex array и normal array.
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void CalculateTriangleStripIndicies(std::vector<uint32_t> &indicies,
                                    unsigned columnCount, unsigned rowCount)
{
    indicies.clear();
    indicies.reserve((columnCount - 1) * rowCount * 2);
    // вычисляем индексы вершин.
    for (unsigned ci = 0; ci < columnCount - 1; ++ci)
    {
        if (ci % 2 == 0)
        {
            for (unsigned ri = 0; ri < rowCount; ++ri)
            {
                unsigned index = ci * rowCount + ri;
                indicies.push_back(index + rowCount);
                indicies.push_back(index);
            }
        }
        else
        {
            for (unsigned ri = rowCount - 1; ri < rowCount; --ri)
            {
                unsigned index = ci * rowCount + ri;
                indicies.push_back(index);
                indicies.push_back(index + rowCount);
            }
        }
    }
}

glm::vec3 GetPositionOnSphere(float u, float v)
{
	const float radius = 1.f;
	const float latitude = float(M_PI) * 2 * v;
	const float longitude = u * 2 - 1; 
	const float latitudeRadius = radius * sinf(latitude);

	return{ cosh(longitude) * cosf(latitude),
		    cosh(longitude) * sinf(latitude),
		    longitude };
}
}

CSolidFunctionSurface::CSolidFunctionSurface()
{
}

void CSolidFunctionSurface::Tesselate(unsigned slices, unsigned stacks)
{
    m_vertices.clear();
    m_vertices.reserve(slices * stacks);

	for (unsigned ci = 0; ci < slices; ++ci)
	{
		const float u = float(ci) / float(slices - 1);
		for (unsigned ri = 0; ri < stacks; ++ri)
		{
			const float v = float(ri) / float(stacks - 1);

			SVertexP3NT2 vertex;
			vertex.position = GetPositionOnSphere(u, v);	

			glm::vec3 dir1 = GetPositionOnSphere(u + 0.1f, v) - vertex.position;
			glm::vec3 dir2 = GetPositionOnSphere(u, v + 0.1f) - vertex.position;
			vertex.normal = glm::normalize(glm::cross(dir1, dir2));

			m_vertices.push_back(vertex);
		}
	}

    CalculateTriangleStripIndicies(m_indicies, slices, stacks);
}

void CSolidFunctionSurface::Draw() const
{
    DoWithBindedArrays(m_vertices, [this] {
        glDrawElements(GL_TRIANGLE_STRIP, GLsizei(m_indicies.size()),
                       GL_UNSIGNED_INT, m_indicies.data());
    });
}
