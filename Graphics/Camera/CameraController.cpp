#include "CameraController.h"

#include <cmath>

#include "Utils/Utils.h"

ICameraController::ICameraController()
	: m_Position(0,0,0)
{
}

ICameraController::ICameraController(const Vect3f& pos)
	: m_Position(pos)
{
}

ICameraController::~ICameraController()
{
}

