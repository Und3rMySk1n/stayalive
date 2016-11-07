#pragma once
#include "lib.h"
#include "Lights.h"
#include "FunctionSurface.h"
#include <vector>
#include "MeshP3NT2.h"
#include "FlatSurface.h"

class CWindow : public CAbstractInputControlWindow
{
public:
    CWindow();

protected:
    // CAbstractWindow interface
    void OnWindowInit(const glm::ivec2 &size) override;
    void OnUpdateWindow(float deltaSeconds, const glm::ivec2 &size) override;
    void OnDrawWindow(const glm::ivec2 &size) override;

    // IInputEventAcceptor interface
    void OnKeyDown(const SDL_KeyboardEvent &) override;
    void OnKeyUp(const SDL_KeyboardEvent &) override;

private:
    void SetupView(const glm::ivec2 &size);

    CPhongModelMaterial m_material;
    CCamera m_camera;
    CDirectedLightSource m_sunlight;
	FlatSurface m_floor;
	FlatSurface m_roof;
};
