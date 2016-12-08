#pragma once
#include "lib.h"
#include <vector>
#include "FunctionSurface.h"
#include "StayAliveProgramContext.h"
#include "MeshP3NT2.h"
#include "Labyrinth.h"
#include "SolidBlock.h"

class CWindowClient
        : public CAbstractWindowClient
{
public:
    CWindowClient(CWindow &window);

protected:
    // IWindowClient interface
    void OnUpdateWindow(float deltaSeconds) override;
    void OnDrawWindow() override;
    void OnKeyDown(const SDL_KeyboardEvent &) override;
    void OnKeyUp(const SDL_KeyboardEvent &) override;

private:
    void UpdateRotation(float deltaSeconds);
    void SetupView(const glm::ivec2 &size);
    void SetupLight0();

    // Данный VAO будет объектом по-умолчанию.
    // Его привязка должна произойти до первой привязки VBO.
    //  http://stackoverflow.com/questions/13403807/
    CArrayObject m_defaultVAO;

    //CIdentitySphere m_sphereObj;
    CCamera m_camera;
    CDirectedLightSource m_sunlight;
	CStayAliveProgramContext m_programContext;
	Labyrinth m_labyrinth;
};
