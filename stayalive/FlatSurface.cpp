#include "stdafx.h"
#include "FlatSurface.h"

namespace
{
	template <class T>
	void DoWithBindedArraysOfVerticies(const std::vector<glm::vec3> &vertices, T && callback)
	{
		// Включаем режим vertex array.
		glEnableClientState(GL_VERTEX_ARRAY);

		// Выполняем привязку vertex array
		const size_t stride = sizeof(glm::vec3);
		glVertexPointer(3, GL_FLOAT, stride, glm::value_ptr(vertices[0]));

		// Выполняем внешнюю функцию
		callback();

		// Выключаем режим vertex array
		glDisableClientState(GL_VERTEX_ARRAY);
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
}

void FlatSurface::Tesselate(unsigned slices, unsigned stacks)
{
	m_vertices.clear();
	m_vertices.reserve(slices * stacks);
	float sliceStep = 20.f / slices;
	float stackStep = 20.f / slices;

	for (unsigned ci = 0; ci < slices; ++ci)
	{
		float x = -10.f + sliceStep * ci;
		for (unsigned ri = 0; ri < stacks; ++ri)
		{
			float y = -10.f + stackStep * ri;

			glm::vec3 vertex = {x, m_height, y};
			m_vertices.push_back(vertex);
		}
	}

	CalculateTriangleStripIndicies(m_indicies, slices, stacks);
}

FlatSurface::FlatSurface()
{
}

void FlatSurface::SetHeight(float height)
{
	m_height = height;
}

void FlatSurface::Draw()
{
	DoWithBindedArraysOfVerticies(m_vertices, [this] {
		glDrawElements(GL_LINE_STRIP, GLsizei(m_indicies.size()),
			GL_UNSIGNED_INT, m_indicies.data());
	});
}

FlatSurface::~FlatSurface()
{
}
