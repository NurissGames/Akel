// This file is a part of Akel
// Authors : @kbz_8
// Created : 25/03/2022
// Updated : 30/05/2022

#include "render_core.h"

namespace Ak
{
	namespace RCore
	{
		std::string verbaliseResultVk(VkResult result)
		{
			switch(result)
			{
				case VK_SUCCESS: return "Success";
				case VK_NOT_READY: return "A fence or query has not yet completed";
				case VK_TIMEOUT: return "A wait operation has not completed in the specified time";
				case VK_EVENT_SET: return "An event is signaled";
				case VK_EVENT_RESET: return "An event is unsignaled";
				case VK_INCOMPLETE: return "A return array was too small for the result";
				case VK_ERROR_OUT_OF_HOST_MEMORY: return "A host memory allocation has failed";
				case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "A device memory allocation has failed";
				case VK_ERROR_INITIALIZATION_FAILED: return "Initialization of an object could not be completed for implementation-specific reasons";
				case VK_ERROR_DEVICE_LOST: return "The logical or physical device has been lost";
				case VK_ERROR_MEMORY_MAP_FAILED: return "Mapping of a memory object has failed";
				case VK_ERROR_LAYER_NOT_PRESENT: return "A requested layer is not present or could not be loaded";
				case VK_ERROR_EXTENSION_NOT_PRESENT: return "A requested extension is not supported";
				case VK_ERROR_FEATURE_NOT_PRESENT: return "A requested feature is not supported";
				case VK_ERROR_INCOMPATIBLE_DRIVER: return "The requested version of Vulkan is not supported by the driver or is otherwise incompatible";
				case VK_ERROR_TOO_MANY_OBJECTS: return "Too many objects of the type have already been created";
				case VK_ERROR_FORMAT_NOT_SUPPORTED: return "A requested format is not supported on this device";
				case VK_ERROR_SURFACE_LOST_KHR: return "A surface is no longer available";
				case VK_SUBOPTIMAL_KHR: return "A swapchain no longer matches the surface properties exactly, but can still be used";
				case VK_ERROR_OUT_OF_DATE_KHR: return "A surface has changed in such a way that it is no longer compatible with the swapchain";
				case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "The display used by a swapchain does not use the same presentable image layout";
				case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "The requested window is already connected to a VkSurfaceKHR, or to some other non-Vulkan API";
				case VK_ERROR_VALIDATION_FAILED_EXT: return "A validation layer found an error";

				default: return "Unknown Vulkan error";
			}
		}

		void checkVk(VkResult result)
		{
			if(result < 0)
				Core::log::report(FATAL_ERROR, "Vulkan error : %s", verbaliseResultVk(result));
		}
	}

	Render_Core::Render_Core() : _device(), _queues(), _surface(),
								 _cmd_pool(), _swapchain(), _instance(),
								 _allocator(), _layers()
	{}

	void Render_Core::init()
	{
		_device.init();
		_allocator.init(_device.getPhysicalDevice(), _device.get(), nullptr, 4096);
		_surface.create();
		_cmd_pool.init();
		_instance.init();
		_swapchain.init();
		_pass.init();
		_layers.init();
		
		_cmd_buffers.resize(MAX_FRAMES_IN_FLIGHT);
		for(auto cmd : _cmd_buffers)
			cmd.init();

		_semaphore.init();
	}

	void Render_Core::update()
	{
		vkWaitForFences(_device(), 1, &semaphore.getInFlightFence(_active_image_index), VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(_device(), _swapchain(), UINT64_MAX, semaphore.getImageSemaphore(_active_image_index), VK_NULL_HANDLE, &imageIndex);

		if(result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			_swapchain.recreate();
			return;
		}
		else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			Core::log::report(FATAL_ERROR, "Vulkan error : failed to acquire swap chain image");

		vkResetFences(device, 1, &semaphore.getInFlightFence(_active_image_index));

		vkResetCommandBuffer(_cmd_buffers[_active_image_index], 0);

		_cmd_buffers[_active_image_index].beginRecord();
		
		

		_cmd_buffers[_active_image_index].endRecord();

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[_active_image_index]};
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &_cmd_buffers[_active_image_index];

		VkSemaphore signalSemaphores[] = { _semaphore.getRenderImageSemaphore(_active_image_index) };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if(vkQueueSubmit(_queues.getGraphic(), 1, &submitInfo, semaphore.getInFlightFence(_active_image_index)) != VK_SUCCESS)
			Core::log::report(FATAL_ERROR, "Vulkan error : failed to submit draw command buffer");

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { _swapchain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = _swapchain;

		presentInfo.pImageIndices = &imageIndex;

		result = vkQueuePresentKHR(_queues.getPresent(), &presentInfo);

		if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _framebufferResized)
		{
			_framebufferResized = false;
			_swapchain.recreate();
		}
		else if(result != VK_SUCCESS)
			Core::log::report(FATAL_ERROR, "Vulkan error : failed to present swap chain image");

		_active_image_index = (_active_image_index + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void Render_Core::destroy()
	{
		_semaphore.destroy();
		_pass.destroy();
		_swapchain.destroy();
		_cmd_pool.destroy();
		_device.destroy();
		_layers.destroy();
		_surface.destroy();
		_instance.destroy();
	}
}
