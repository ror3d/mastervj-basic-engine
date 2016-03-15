#include <Windows.h>
#include <Windowsx.h>

#include <d3d11.h>

// TODO: Activar AntTeakBar
//#include <AntTweakBar.h>

#include "Math/Matrix44.h"
#include "Math/Vector4.h"

#include "Application.h"

#include "Input/InputManagerImplementation.h"
#include "Debug/DebugHelperImplementation.h"

#include <Engine/Engine.h>
#include <Graphics/Context/ContextManager.h>
#include <Graphics/Debug/DebugRender.h>
#include <Graphics/Camera/Camera.h>

#include <AntTweakBar.h>

#pragma comment(lib, "Winmm.lib")

#define APPLICATION_NAME	"VIDEOGAME"

void ToggleFullscreen(HWND Window, WINDOWPLACEMENT &WindowPosition)
{
	// This follows Raymond Chen's prescription
	// for fullscreen toggling, see:
	// http://blogs.msdn.com/b/oldnewthing/archive/2010/04/12/9994016.aspx

	DWORD Style = GetWindowLongW(Window, GWL_STYLE);
	if (Style & WS_OVERLAPPEDWINDOW)
	{
		MONITORINFO MonitorInfo = { sizeof(MonitorInfo) };
		if (GetWindowPlacement(Window, &WindowPosition) &&
			GetMonitorInfoW(MonitorFromWindow(Window, MONITOR_DEFAULTTOPRIMARY), &MonitorInfo))
		{
			SetWindowLongW(Window, GWL_STYLE, Style & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(Window, HWND_TOP,
						 MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top,
						 MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left,
						 MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top,
						 SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	}
	else
	{
		SetWindowLongW(Window, GWL_STYLE, Style | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(Window, &WindowPosition);
		SetWindowPos(Window, 0, 0, 0, 0, 0,
					 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
					 SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CContextManager& context = *(CEngine::GetSingleton().getContextManager());
	CInputManagerImplementation *inputManager = dynamic_cast<CInputManagerImplementation*>(CInputManager::GetInputManager());

	bool WasDown = false, IsDown = false, Alt = false;

	switch (msg)
	{
		case WM_SETFOCUS:
			if (inputManager) {
				inputManager->SetFocus(true);
			}
			return 0;
		case  WM_KILLFOCUS:
			if (inputManager) {
				inputManager->SetFocus(false);
			}
			return 0;
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
			WasDown = ((lParam & (1 << 30)) != 0);
			IsDown = ((lParam & (1 << 31)) == 0);
			Alt = ((lParam & (1 << 29)) != 0);

			if (IsDown && !WasDown)
			{
				bool consumed = false;
				switch (wParam)
				{
					case VK_RETURN:
						if (Alt)
						{
							WINDOWPLACEMENT windowPosition = { sizeof(WINDOWPLACEMENT) };
							GetWindowPlacement(hWnd, &windowPosition);

							ToggleFullscreen(hWnd, windowPosition);
							consumed = true;
						}
						break;
					case VK_ESCAPE:
						PostQuitMessage(0);
						consumed = true;
						break;
					case VK_F4:
						if (Alt)
						{
							PostQuitMessage(0);
							consumed = true;
						}
						break;
				}
				if (consumed)
				{
					return 0;
				}
			}
			if (inputManager && inputManager->HasFocus())
			{
				if (inputManager->KeyEventReceived(wParam, lParam))
				{
					return 0;
				}
			}
			break;
		case WM_MOUSEMOVE:
			if (inputManager && inputManager->HasFocus())
			{
				int xPosAbsolute = GET_X_LPARAM(lParam);
				int yPosAbsolute = GET_Y_LPARAM(lParam);

				inputManager->UpdateCursor(xPosAbsolute, yPosAbsolute);

				return 0;
			}
			break;
		case WM_SIZE:
			if (wParam != SIZE_MINIMIZED)
			{
				// TODO: Resetear el AntTeakBar
				TwWindowSize(0, 0);

				context.Resize(hWnd, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
				// TODO: Resetear el AntTeakBar
				TwWindowSize((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
			}
			return 0;
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
	}//end switch( msg )
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


//-----------------------------------------------------------------------
// WinMain
//-----------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(77928);

	new CEngine();
	CEngine& engine = CEngine::GetSingleton();
	//*/
	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };

	RegisterClassEx(&wc);


	// Calcular el tamano de nuestra ventana
	RECT rc = {
		0, 0, 800, 600
	};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the application's window
	HWND hWnd = CreateWindow(APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 100, 100, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, wc.hInstance, NULL);

	// Añadir aquí el Init de la applicacioón

	engine.Init();
	engine.getPhysXManager()->InitPhysx();

	CContextManager& context = *(CEngine::GetSingleton().getContextManager());
	context.CreateContext(hWnd, 800, 600);

	ShowWindow(hWnd, SW_SHOWDEFAULT);

	engine.getDebugRender()->Init();
	engine.getEffectsManager()->load("Data\\effects.xml");
	engine.getRenderableObjectTechniqueManager()->Load("Data\\pool_renderable_objects.xml");
	engine.getMaterialManager()->load("Data\\materials.xml");
	engine.getCookedMeshManager()->SetCookedMeshPath("Cache\\Cooked\\");
	engine.getStaticMeshManager()->Load("Data\\static_meshes.xml");
	engine.getAnimatedModelManager()->Load("Data\\animated_models.xml");
	engine.getParticleManager()->Load("Data\\particle_classes.xml");
	engine.getLayerManager()->Load("Data\\renderable_objects.xml");
	engine.getLightManager()->Load("Data\\lights.xml");
	engine.getSceneRendererCommandManager()->Load("Data\\scene_renderer_commands.xml");
	engine.getIAManager()->Create();
	engine.getScriptManager()->Initialize();
	engine.getCharacterControllerManager()->Create("main", "__fps", "models", "main");


	context.CreateBackBuffer(hWnd, 800, 600);
	{
		CInputManagerImplementation inputManager(hWnd);
		CInputManager::SetCurrentInputManager(&inputManager);
		inputManager.LoadCommandsFromFile("Data\\input.xml");

		CDebugHelperImplementation debugHelper(context.GetDevice());
		CDebugHelper::SetCurrentDebugHelper(&debugHelper);

		CApplication application(&context);

		engine.getScriptManager()->RegisterLUAFunctions();


		UpdateWindow(hWnd);
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		// Añadir en el while la condición de salida del programa de la aplicación
		DWORD m_PreviousTime = timeGetTime();

		bool hasFocus = true;

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				if (!debugHelper.Update(msg.hwnd, msg.message, msg.wParam, msg.lParam))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				inputManager.BeginFrame();

				DWORD l_CurrentTime = timeGetTime();
				float l_ElapsedTime = (float)(l_CurrentTime - m_PreviousTime)*0.001f;
				CEngine::GetSingleton().getTimerManager()->m_elapsedTime = l_ElapsedTime;
				m_PreviousTime = l_CurrentTime;


				application.Update(l_ElapsedTime);
				engine.getIAManager()->Update(l_ElapsedTime);
				application.Render();
				inputManager.EndFrame();
			}
		}
	}

	CEngine::ReleaseSingleton();
	UnregisterClass(APPLICATION_NAME, wc.hInstance);
	return 0;
}


