#ifndef BAR_CALLS_H
#define BAR_CALLS_H

#include <string>
#include "AntTweakBar.h"

class CBarCalls
{
public:
	static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static void OpenParticleDialog(CParticleSystemClass * particle);
	static void TW_CALL RemoveBar(void* bar);
	static void TW_CALL OpenParticleParameters(void *part);
	static void TW_CALL CloneParticle(void * part);
	static void TW_CALL OpenParticlesBar(void *particlesMap);
	static void TW_CALL ReloadParticles(void* _app);
	static void TW_CALL EqualVarsFloat(void *rng);
	static void TW_CALL EqualVarsColor(void *rng);
	static void TW_CALL EqualVarsVect3f(void *rng);
	static void TW_CALL SaveToFile(void *_app);
	static void TW_CALL LoadTexture(void *part);
	static void TW_CALL DeleteParticle(void *part);
	static void TW_CALL ChangeColorSpace(void *part);
};

#endif