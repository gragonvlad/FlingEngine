#pragma once

#include "Shader.h"
#include "NonCopyable.hpp"

#include <entt/entity/registry.hpp>
#include <entt/entity/helper.hpp>

namespace Fling
{
	class CommandBuffer;
	class LogicalDevice;
	class FrameBuffer;
	class Swapchain;
	class GraphicsPipeline;

	/**
	* @breif	A subpass represents one part of a RenderPipeline. Each subpass should 
	*			can add attachments to the frame buffer, build it's own command buffers, 
	*			and create its own descriptors. When overriding this class, add any additional
	*			uniform buffers or bindings you may need into the child class. 
	* 
	* @see GeometrySubpass for an example
	*/
	class Subpass : public NonCopyable
	{
	public:
		Subpass(const LogicalDevice* t_Dev, const Swapchain* t_Swap, std::shared_ptr<Fling::Shader> t_Vert, std::shared_ptr<Fling::Shader> t_Frag);
		
		virtual ~Subpass();

		/** Add any attachments to a frame buffer that this subpass may need */
		virtual void PrepareAttachments() {}

		virtual void CreateGraphicsPipeline() = 0;

		virtual void Draw(CommandBuffer& t_CmdBuf, VkFramebuffer t_PresentFrameBuf, UINT32 t_ActiveFrameInFlight, entt::registry& t_reg, float DeltaTime) = 0;

		/** Cleanup any allocated resources that you may need a registry for */
		virtual void CleanUp(entt::registry& t_reg) {}

		/**
		* @brief	Given the frame buffers and the registry, create any descriptor sets that we may need
		*			Assumes that the frame buffer has been prepared with it's attachments already.
		* @param t_FrameBuffer	The swap chain frame buffer
		*/		
		virtual void CreateDescriptorSets(VkDescriptorPool t_Pool, entt::registry& t_reg) = 0;
		
		/**
		 * @brief	If a subpass has a command buffer that the final swap chain presentation is dependent on, 
		 *			then add it this vector. The Deferred offscreen GBuffer is an example of this
		 */
		virtual void GatherPresentDependencies(std::vector<CommandBuffer*>& t_CmdBuffs, std::vector<VkSemaphore>& t_Deps, UINT32 t_ActiveFrameIndex, UINT32 t_CurrentFrameInFlight) {}
		
		/**
		* @brief	If a subpass has an additional command buffer to add to the final swap chain draw submission
		*			but it is not dependent on it, then add it here. ImGUI is an example of this
		*/
		virtual void GatherPresentBuffers(std::vector<CommandBuffer*>& t_CmdBuffs, UINT32 t_ActiveFrameIndex) {}


		inline GraphicsPipeline* GetGraphicsPipeline() const noexcept { return m_GraphicsPipeline; }
		inline const std::vector<VkClearValue>& GetClearValues() const { return m_ClearValues; }

	protected:

		// Get default graphics Pipeline
		const LogicalDevice* m_Device;
		const Swapchain* m_SwapChain;

		std::shared_ptr<Fling::Shader> m_VertexShader;
		
		std::shared_ptr<Fling::Shader> m_FragShader;

		/** The clear values that will be used when building the command buffer to run this subpass */
		std::vector<VkClearValue> m_ClearValues = std::vector<VkClearValue>(2);

		/** Layouts created in the constructor via shader reflection */
		GraphicsPipeline* m_GraphicsPipeline = nullptr;
	};
}