#include "stdafx.h"
#include "WindowClient.h"
#include "LabyrinthRenderer3D.h"

using glm::mat4;
using glm::vec3;

namespace
{
const float CAMERA_INITIAL_ROTATION = 0.f;
const float CAMERA_INITIAL_DISTANCE = 3;
const int SPHERE_PRECISION = 40;

const int PLAYER_ID = 2;
const int KEY_ID = 3;

const unsigned TILE_SIZE = 2;
const unsigned LABYRINTH_WIDTH = 6;
const unsigned LABYRINTH_LENGTH = 10;
const float LABYRINTH_HEIGHT = 3.f;
const float WALL_OFFSET = 0.2f;

void SetupOpenGLState()
{
    // включаем механизмы трёхмерного мира.
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
}
}

CWindowClient::CWindowClient(CWindow &window)
	: CAbstractWindowClient(window)
	, m_defaultVAO(CArrayObject::do_bind_tag())
	, m_camera(CAMERA_INITIAL_ROTATION, CAMERA_INITIAL_DISTANCE)
	, m_labirynthReader(new CLabyrinthReader("res/map.tmx"))
	, m_lamp(GL_LIGHT0)
	, m_labyrinth(m_labirynthReader->GetWidth(), m_labirynthReader->GetLength(), LABYRINTH_HEIGHT, TILE_SIZE)
{
    const glm::vec3 SUNLIGHT_DIRECTION = {1.f, 1.f, 1.f};
	const glm::vec4 WHITE_RGBA = { 1, 1, 1, 1 };
    const glm::vec4 LIGHT_BROWN_RGBA = {0.746f, 0.7f, 0.68f, 1};
    const glm::vec4 BLACK_RGBA = {0.01f, 0.02f, 0.03f, 1};

    window.SetBackgroundColor(BLACK_RGBA);
    SetupOpenGLState();	
		
	std::vector<std::vector<int>> blocksArray = m_labirynthReader->GetBlocks(m_labirynthReader->GetWidth(), m_labirynthReader->GetLength());
	m_labyrinth.AddWalls(blocksArray);
	m_labirynthReader.reset();	

	glm::vec2 keyPosition = m_labyrinth.GetObjectPosition(blocksArray, KEY_ID);
	m_key.SetPosition(keyPosition);

	glm::vec2 playerPosition = m_labyrinth.GetObjectPosition(blocksArray, PLAYER_ID);
	m_camera.SetPosition(playerPosition);

	m_lamp.SetDiffuse(LIGHT_BROWN_RGBA);
	m_lamp.SetSpecular(LIGHT_BROWN_RGBA);
	m_lamp.SetPosition({ playerPosition.x, 1.0f, playerPosition.y });
	m_lamp.SetSpotDirection({-1.0f, -1.0f, 0.0f});
	m_lamp.SetSpotCutoff(30);

	ShowCursor(false);
}

void CWindowClient::OnUpdateWindow(float deltaSeconds)
{
    //UpdateRotation(deltaSeconds);
	// TODO: make player actor separate from camera
	glm::vec2 pos = m_camera.GetPositionOnMap();
	glm::vec2 newPos = m_camera.GetNewPositionOnMap();
	glm::vec2 resultPos = m_labyrinth.CorrectActorMovement(pos, newPos, WALL_OFFSET);

	m_camera.Move(deltaSeconds, resultPos);
	m_camera.Update(deltaSeconds, GetWindow().GetWindowSize());

	if (m_key.Reached(resultPos))
	{		
		if (!m_player.HasKey())
		{
			m_player.PickUpKey();
			std::cout << "Picked up the key!" << std::endl;
		}		
	}

	m_camera.SetPosition(resultPos);
}

void CWindowClient::OnDrawWindow()
{
    SetupView(GetWindow().GetWindowSize());
    SetupLight0();

	CLabyrinthRenderer3D renderer(m_programContext);
	m_labyrinth.Draw(renderer);

	if (!m_player.HasKey())
	{
		m_key.Draw(renderer);
	}	
}

void CWindowClient::OnKeyDown(const SDL_KeyboardEvent &event)
{
	m_camera.OnKeyDown(event); 
}

void CWindowClient::OnKeyUp(const SDL_KeyboardEvent &event)
{
    m_camera.OnKeyUp(event);
}

void CWindowClient::UpdateRotation(float deltaSeconds)
{
    const float ROTATION_SPEED = 0.2f;
    const float deltaRotation = ROTATION_SPEED * deltaSeconds;
    const mat4 model = glm::rotate(m_programContext.GetModel(),
                                   deltaRotation,
                                   vec3(0, 1, 0));
    m_programContext.SetModel(model);
}

void CWindowClient::SetupView(const glm::ivec2 &size)
{
    const mat4 view = m_camera.GetViewTransform();

    // Матрица перспективного преобразования вычисляется функцией
    // glm::perspective, принимающей угол обзора, соотношение ширины
    // и высоты окна, расстояния до ближней и дальней плоскостей отсечения.
    const float fieldOfView = glm::radians(70.f);
    const float aspect = float(size.x) / float(size.y);
    const float zNear = 0.01f;
    const float zFar = 100.f;
    const mat4 proj = glm::perspective(fieldOfView, aspect, zNear, zFar);

    glViewport(0, 0, size.x, size.y);

    m_programContext.SetView(view);
    m_programContext.SetProjection(proj);
}

void CWindowClient::SetupLight0()
{
    CStayAliveProgramContext::SLightSource light0;
    light0.specular = m_lamp.GetSpecular();
    light0.diffuse = m_lamp.GetDiffuse();
    light0.position = m_lamp.GetUniformPosition();

	light0.direction = m_lamp.GetSpotDirection();
	light0.cutoff = m_lamp.GetSpotCutoff();

    m_programContext.SetLight0(light0);
}
