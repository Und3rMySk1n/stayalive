#include "stdafx.h"
#include "StayAliveProgramContext.h"

namespace
{
	glm::mat4 GetNormalMatrix(const glm::mat4 &modelView)
	{
		return glm::transpose(glm::inverse(modelView));
	}
}

CStayAliveProgramContext::CStayAliveProgramContext()
{
	CTexture2DLoader loader;

	m_pGroundTexture = loader.Load("res/img/ground.jpg");
	m_pGroundNormal = loader.Load("res/img/NormalMap.png");

	const auto vertShader = CFilesystemUtils::LoadFileAsString("res/cloud_earth_robust.vert");
	const auto fragShader = CFilesystemUtils::LoadFileAsString("res/cloud_earth_robust.frag");
	m_programStayAlive.CompileShader(vertShader, ShaderType::Vertex);
	m_programStayAlive.CompileShader(fragShader, ShaderType::Fragment);
	m_programStayAlive.Link();
}

void CStayAliveProgramContext::Use()
{
	// переключаемся на текстурный слот #2
	//glActiveTexture(GL_TEXTURE2);
	//m_pNightTexture->Bind();
	// переключаемся на текстурный слот #1
	//glActiveTexture(GL_TEXTURE1);
	//m_pCloudTexture->Bind();
	// переключаемся обратно на текстурный слот #0
	// перед началом рендеринга активным будет именно этот слот.
	glActiveTexture(GL_TEXTURE1);
	m_pGroundNormal->Bind();
	glActiveTexture(GL_TEXTURE0);
	m_pGroundTexture->Bind();

	m_programStayAlive.Use();
	m_programStayAlive.FindUniform("normalmap") = 1; // GL_TEXTURE1
	m_programStayAlive.FindUniform("colormap") = 0; // GL_TEXTURE0

	const glm::mat4 mv = m_view * m_model;
	m_programStayAlive.FindUniform("view") = m_view;
	m_programStayAlive.FindUniform("modelView") = mv;
	m_programStayAlive.FindUniform("normalModelView") = GetNormalMatrix(mv);
	m_programStayAlive.FindUniform("projection") = m_projection;

	m_programStayAlive.FindUniform("light0.position") = m_light0.position;
	m_programStayAlive.FindUniform("light0.diffuse") = m_light0.diffuse;
	m_programStayAlive.FindUniform("light0.specular") = m_light0.specular;
}

CVertexAttribute CStayAliveProgramContext::GetPositionAttr() const
{
	return m_programStayAlive.FindAttribute("vertex");
}

CVertexAttribute CStayAliveProgramContext::GetNormalAttr() const
{
	return m_programStayAlive.FindAttribute("normal");
}

CVertexAttribute CStayAliveProgramContext::GetTexCoordAttr() const
{
	return m_programStayAlive.FindAttribute("textureUV");
}

const glm::mat4 &CStayAliveProgramContext::GetModel() const
{
	return m_model;
}

const glm::mat4 &CStayAliveProgramContext::GetView() const
{
	return m_view;
}

const glm::mat4 &CStayAliveProgramContext::GetProjection() const
{
	return m_projection;
}

const CStayAliveProgramContext::SLightSource &CStayAliveProgramContext::GetLight0() const
{
	return m_light0;
}

void CStayAliveProgramContext::SetModel(const glm::mat4 &value)
{
	m_model = value;
}

void CStayAliveProgramContext::SetView(const glm::mat4 &value)
{
	m_view = value;
}

void CStayAliveProgramContext::SetProjection(const glm::mat4 &value)
{
	m_projection = value;
}

void CStayAliveProgramContext::SetLight0(const CStayAliveProgramContext::SLightSource &source)
{
	m_light0 = source;
}
