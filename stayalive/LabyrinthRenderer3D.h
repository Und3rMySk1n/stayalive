#pragma once
#include "lib.h"
#include "IRenderer3D.h"

class CStayAliveProgramContext;

class CLabyrinthRenderer3D : public IRenderer3D
{
public:
	CLabyrinthRenderer3D(CStayAliveProgramContext &context);
	~CLabyrinthRenderer3D();

	// IRenderer3D interface
	void SetTexCoord2DOffset(size_t offset, size_t stride) override;
	void SetPosition3DOffset(size_t offset, size_t stride) override;
	void SetNormalOffset(size_t offset, size_t stride) override;

	void SetColormapSlot(const int slotNumber) override;

private:
	CStayAliveProgramContext &m_context;
	CVertexAttribute m_vertexAttr;
	CVertexAttribute m_normalAttr;
	CVertexAttribute m_texCoordAttr;
};
