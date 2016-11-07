#include "lib_private.h"
#include "Camera.h"
#include <glm/gtx/rotate_vector.hpp>


namespace
{
const float ROTATION_SPEED_RADIANS = 1.f;
const float LINEAR_MOVE_SPEED = 5.f;
const float MIN_DISTANCE = 1.5f;
const float MAX_DISTANCE = 30.f;

const float MOVE_SPEED = 0.05;
const float VIEW_SPEED = 1.f;
const float MOUSE_SENSITIVITY = 10.f;

bool ShouldTrackKeyPressed(const SDL_Keysym &key)
{
    switch (key.sym)
    {
    case SDLK_LEFT:
    case SDLK_RIGHT:
    case SDLK_UP:
    case SDLK_DOWN:
    case SDLK_w:
    case SDLK_a:
    case SDLK_s:
    case SDLK_d:
        return true;
    }
    return false;
}

float GetRotationSpeedRadians(std::set<unsigned> & keysPressed)
{
    if (keysPressed.count(SDLK_RIGHT) || keysPressed.count(SDLK_d))
    {
        return ROTATION_SPEED_RADIANS;
    }
    if (keysPressed.count(SDLK_LEFT) || keysPressed.count(SDLK_a))
    {
        return -ROTATION_SPEED_RADIANS;
    }
    return 0;
}

float GetXOffset(std::set<unsigned> & keysPressed, float angleX)
{
	float offset = 0;

    if (keysPressed.count(SDLK_UP) || keysPressed.count(SDLK_w))
    {
		offset -= (float)sinf(angleX / 180.f * (float)M_PI) * MOVE_SPEED;
    }
    if (keysPressed.count(SDLK_DOWN) || keysPressed.count(SDLK_s))
    {
		offset += (float)sinf(angleX / 180.f * (float)M_PI) * MOVE_SPEED;
    }
	if (keysPressed.count(SDLK_LEFT) || keysPressed.count(SDLK_a))
	{
		offset -= (float)sinf((angleX + 90.f) / 180.f * (float)M_PI) * MOVE_SPEED;
	}
	if (keysPressed.count(SDLK_RIGHT) || keysPressed.count(SDLK_d))
	{
		offset += (float)sinf((angleX + 90.f) / 180.f * (float)M_PI) * MOVE_SPEED;
	}

    return offset;
}

float GetZOffset(std::set<unsigned> & keysPressed, float angleX)
{
	float offset = 0;

	if (keysPressed.count(SDLK_UP) || keysPressed.count(SDLK_w))
	{
		offset -= (float)cosf(angleX / 180.f * (float)M_PI) * MOVE_SPEED;
	}
	if (keysPressed.count(SDLK_DOWN) || keysPressed.count(SDLK_s))
	{
		offset += (float)cosf(angleX / 180.f * (float)M_PI) * MOVE_SPEED;
	}
	if (keysPressed.count(SDLK_LEFT) || keysPressed.count(SDLK_a))
	{
		offset -= (float)cosf((angleX + 90.f) / 180.f * (float)M_PI) * MOVE_SPEED;
	}
	if (keysPressed.count(SDLK_RIGHT) || keysPressed.count(SDLK_d))
	{
		offset += (float)cosf((angleX + 90.f) / 180.f * (float)M_PI) * MOVE_SPEED;
	}

	return offset;
}
}


CCamera::CCamera(float rotationRadians, float distance)
    : m_rotationRadians(rotationRadians)
    , m_distance(distance)
{
    SetDirection({0.f, 0.5f, 1.f});
}

void CCamera::Update(float deltaSec, const glm::ivec2 &windowSize)
{
	m_posX += GetXOffset(m_keysPressed, m_angleX);
	m_posZ += GetZOffset(m_keysPressed, m_angleX);

	GetCursorPos(&m_mousexy);
	m_angleX += (windowSize.x / 2.f - m_mousexy.x) / MOUSE_SENSITIVITY;
	m_angleY += (windowSize.y / 2.f - m_mousexy.y) / MOUSE_SENSITIVITY;
	if (m_angleY < -89.0) { m_angleY = -89.0; }
	if (m_angleY > 89.0) { m_angleY = 89.0; }
	SetCursorPos(windowSize.x / 2.f, windowSize.y / 2.f);
}

bool CCamera::OnKeyDown(const SDL_KeyboardEvent &event)
{
    if (ShouldTrackKeyPressed(event.keysym))
    {
        m_keysPressed.insert(unsigned(event.keysym.sym));
        return true;
    }
    return false;
}

bool CCamera::OnKeyUp(const SDL_KeyboardEvent &event)
{
    if (ShouldTrackKeyPressed(event.keysym))
    {
        m_keysPressed.erase(unsigned(event.keysym.sym));
        return true;
    }
    return false;
}

void CCamera::SetDirection(const glm::vec3 &direction)
{
    // Дополнительно нормализуем вектор (приводим к единичной длине).
    m_direction = glm::normalize(direction);
}

glm::mat4 CCamera::GetViewTransform() const
{
    // Поворачиваем вектор направления камеры вокруг оси Y.
    // см. http://glm.g-truc.net/0.9.3/api/a00199.html
    //glm::vec3 direction = glm::rotateY(m_direction, m_rotationRadians);

	const glm::vec3 direction = { 
		m_posX - sin(m_angleX / 180 * M_PI), 
		m_height + (tan(m_angleY / 180 * M_PI)), 
		m_posZ - cosf(m_angleX / 180 * M_PI) };

	const glm::vec3 eye = {m_posX, m_height, m_posZ};
    //const glm::vec3 center = {0, 0, 0};
    const glm::vec3 up = {0, 1, 0};

    // Матрица моделирования-вида вычисляется функцией glm::lookAt.
    // Она даёт матрицу, действующую так, как будто камера смотрит
    // с позиции eye на точку center, а направление "вверх" камеры равно up.
    return glm::lookAt(eye, direction, up);
}
