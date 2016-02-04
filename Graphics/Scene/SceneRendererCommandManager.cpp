
#include "SceneRendererCommand.h"
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
#include "RenderDebugGUISceneRendererCommand.h"

CSceneRendererCommandManager::CSceneRendererCommandManager(){

}

CSceneRendererCommandManager::~CSceneRendererCommandManager(){

}

std::string CSceneRendererCommandManager::GetNextName(){

	return nullptr;
}

void CSceneRendererCommandManager::add(CSceneRendererCommand * command){
	m_resources.push_back(command);
}

void CSceneRendererCommandManager::destroy(){
	for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
	{
		delete (*it);
	}
	m_resources.clear();
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
					add(new CSetDepthStencilStateSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("set_pool_renderable_objects_technique")){
					add(new CSetPoolRenderableObjectsTechniqueSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("generate_shadow_maps")){
					add(new CGenerateShadowMapsSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("set_render_target")){
					add(new CSetRenderTargetSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("set_matrices")){
					add(new CSetMatricesSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("render_debug_layer")){
					add(new CRenderDebugLayerSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("unset_render_target")){
					add(new CUnsetRenderTargetSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("clear")){
					add(new CClearSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("render_draw_quad")){
					add(new CDrawQuadSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("render_deferred_shading")){
					add(new CDeferredShadingSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("capture_frame_buffer")){
					add(new CCaptureFrameBufferSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("set_light_constants")){
					add(new CRenderDebugLightsSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("render_layer")){
					add(new CRenderLayerSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("render_debug_grid")){
					add(new CRenderDebugGridSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("enable_alpha_blend")){
					add(new CEnableAlphaBlendSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("disable_alpha_blend")){
					add(new CDisableAlphaBlendSceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("render_debug_gui")){
					add(new CRenderGUISceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("render_debug_antTweak")){
					add(new CRenderDebugGUISceneRendererCommand(scene_command));
				}
				else if (scene_command.GetName() == std::string("present")){
					add(new CPresentSceneRendererCommand(scene_command));
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
		(*it)->Execute(_context);
	}
}

