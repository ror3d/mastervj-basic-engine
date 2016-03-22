#include "Scene/DeferredShadingSceneRendererCommand.h"
#include "Renderable/RenderableObjectTechnique.h"
#include <Core/Debug/DebugPerf.h>

#include <Core/Engine/Engine.h>
#include <Graphics/Context/ContextManager.h>
#include <Graphics/Texture/TextureManager.h>
#include <Graphics/Material/MaterialManager.h>
#include <Graphics/Effect/EffectManager.h>
#include <Graphics/Light/LightManager.h>
#include <Graphics/Material/Material.h>
#include "Light/Light.h"

CDeferredShadingSceneRendererCommand::CDeferredShadingSceneRendererCommand(CXMLTreeNode &TreeNode)
	: CStagedTexturedSceneRendererCommand(TreeNode)
{
	auto mm = CEngine::GetSingleton().getMaterialManager();
	m_Material = mm->get(TreeNode.GetPszProperty("material"));

	for (int i = 0; i < TreeNode.GetNumChildren(); i++)
	{
		CXMLTreeNode texChild = TreeNode(i);
		if ( texChild.GetName() != std::string( "texture" ) )
		{
			continue;
		}
		CTexture *tex = nullptr;
		tex = CEngine::GetSingleton().getTextureManager()->GetTexture(texChild.GetPszProperty("file"));
		AddStageTexture(texChild.GetIntProperty("stage_id"), tex);
	}

	D3D11_BLEND_DESC l_AlphablendDesc;
	ZeroMemory(&l_AlphablendDesc, sizeof(D3D11_BLEND_DESC));
	l_AlphablendDesc.RenderTarget[0].BlendEnable = true;
	l_AlphablendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	l_AlphablendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	l_AlphablendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	l_AlphablendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	l_AlphablendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	l_AlphablendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	l_AlphablendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HRESULT hr = CEngine::GetSingleton().getContextManager()->GetDevice()->CreateBlendState(&l_AlphablendDesc, &m_EnabledAlphaBlendState);
	DEBUG_ASSERT(!FAILED(hr));
}

CDeferredShadingSceneRendererCommand::~CDeferredShadingSceneRendererCommand()
{
	m_EnabledAlphaBlendState->Release();
}

void CDeferredShadingSceneRendererCommand::Execute(CContextManager &_context)
{
	SCOPED_DEBUG_PERF(L"Deferred Shading");

	CEngine::GetSingleton().getContextManager()->GetDeviceContext()->OMSetBlendState(m_EnabledAlphaBlendState, nullptr, 0xffffffff);

	auto lm = CEngine::GetSingleton().getLightManager();
	auto em = CEngine::GetSingleton().getEffectsManager();

	ActivateTextures();
	for (int i = 0; i < lm->count(); i++)
	{
		CLight& light = lm->iterate(i);
		// Set light to the buffer
		em->SetLightConstants(0, &light);
		m_Material->getRenderableObjectTechique()->GetEffectTechnique()->SetConstantBuffer(1, &CEffectManager::m_LightParameters);
		// Render scene with that light

		// TODO: Paint only enabled lights
		m_Material->apply();
		_context.DrawRelativeScreenQuad(m_Material->getRenderableObjectTechique()->GetEffectTechnique(),
								nullptr, 0, 0, 1, 1, CColor(1, 1, 1, 1));
	}

	CEngine::GetSingleton().getContextManager()->GetDeviceContext()->OMSetBlendState(NULL, NULL, 0xffffffff);
}
