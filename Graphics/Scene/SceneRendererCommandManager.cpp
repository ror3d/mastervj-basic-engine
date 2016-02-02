#include "SceneRendererCommandManager.h"
#include "SetDepthStencilStateSceneRendererCommand.h"
#include "SetPoolRenderableObjectsTechniqueSceneRendererCommand.h"
#include "GenerateShadowMapsSceneRendererCommand.h"
#include "SetRenderTargetSceneRendererCommand.h"
#include "SetMatricesSceneRendererCommand.h"
#include "RenderDebugLayerSceneRendererCommand.h"
#include "UnsetRenderTargetSceneRendererCommand.h"
#include "ClearSceneRendererCommand.h"
#include "DrawQuadSceneRendererCommand.h"
#include "DeferredShadingSceneRendererCommand.h"
#include "CaptureFrameBufferSceneRendererCommand.h"
#include "RenderDebugLightsSceneRendererCommand.h"
#include "RenderLayerSceneRendererCommand.h"
#include "RenderDebugGridSceneRendererCommand.h"
#include "EnableAlphaBlendSceneRendererCommand.h"
#include "DisableAlphaBlendSceneRendererCommand.h"
#include "RenderGUISceneRendererCommand.h"
#include "PresentSceneRendererCommand.h"

CSceneRendererCommandManager::CSceneRendererCommandManager(){

}

CSceneRendererCommandManager::~CSceneRendererCommandManager(){

}

std::string CSceneRendererCommandManager::GetNextName(){

	return nullptr;
}

bool CSceneRendererCommandManager::Load(const std::string &FileName){
	m_Filename = FileName;
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(FileName.c_str()))
	{
		CXMLTreeNode scene_rend_comm = l_XML["scene_renderer_commands"];
		if (scene_rend_comm.Exists())
		{
			for (int i = 0; i < scene_rend_comm.GetNumChildren(); ++i)
			{
				CXMLTreeNode scene_command = scene_rend_comm(i);

				if (scene_command.GetName() == std::string("set_depth_stencil_state")){
					add(scene_command.GetName(),new CSetDepthStencilStateSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("set_pool_renderable_objects_technique")){
					add(scene_command.GetName(), new CSetPoolRenderableObjectsTechniqueSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("generate_shadow_maps")){
					add(scene_command.GetName(), new CGenerateShadowMapsSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("set_render_target")){
					add(scene_command.GetName(), new CSetRenderTargetSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("set_matrices")){
					add(scene_command.GetName(), new CSetMatricesSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("render_debug_layer")){
					add(scene_command.GetName(), new CRenderDebugLayerSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("unset_render_target")){
					add(scene_command.GetName(), new CUnsetRenderTargetSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("clear")){
					add(scene_command.GetName(), new CClearSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("render_draw_quad")){
					add(scene_command.GetName(), new CDrawQuadSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("render_deferred_shading")){
					add(scene_command.GetName(), new CDeferredShadingSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("capture_frame_buffer")){
					add(scene_command.GetName(), new CCaptureFrameBufferSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("set_light_constants")){
					add(scene_command.GetName(), new CRenderDebugLightsSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("render_layer")){
					add(scene_command.GetName(), new CRenderLayerSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("render_debug_grid")){
					add(scene_command.GetName(), new CRenderDebugGridSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("enable_alpha_blend")){
					add(scene_command.GetName(), new CEnableAlphaBlendSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("disable_alpha_blend")){
					add(scene_command.GetName(), new CDisableAlphaBlendSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("render_debug_gui")){
					add(scene_command.GetName(), new CRenderGUISceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("present")){
					add(scene_command.GetName(), new CPresentSceneRendererCommand(scene_command));
				}
			}
			return true;
		}		
	}
	return false;
}

bool CSceneRendererCommandManager::Reload(){
	return Load(m_Filename);
}

void CSceneRendererCommandManager::Execute(CContextManager &_context){
	for (auto it = m_resources.begin(); it != m_resources.end(); it++){
		it->second->Execute(_context);
	}
}