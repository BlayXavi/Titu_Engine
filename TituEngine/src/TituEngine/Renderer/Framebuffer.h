#pragma once
#include "TituEngine/Core/Core.h"

#include "Signals.h"

namespace TituEngine
{
	enum class FramebufferTextureFormat
	{
		NONE = 0,

		//Color
		RGBA8,
		RGBA16,
		RED_INTEGER_32,

		//DEPTH/STENCIL
		DEPTH24STENCIL8,

		DEPTH = DEPTH24STENCIL8

	};

	struct FramebufferSpecs
	{
		uint32_t Width = 0, Height = 0;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;

		std::vector<FramebufferTextureFormat> Attachments;

		bool IsMultiSampled() const { return Samples > 1; }
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual const FramebufferSpecs& GetFramebufferSpecs() const { return m_FramebufferSpecs; };
		static Framebuffer* Create(const FramebufferSpecs& framebufferSpecs);

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual const void ClearAttachment(uint32_t index, int value) = 0;
		virtual const uint32_t GetColorAttachment(uint32_t index = 0) const = 0;
		virtual const uint32_t GetDepthAttachment() const = 0;
		virtual const uint32_t GetPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual const std::vector<uint32_t> GetAttachments() = 0;
		virtual const void BindColorAttachments() = 0;

		void SetDirty(const uint32_t& width, const uint32_t& height);
		void ProcessDirty();

		bool GetDirty() const { return m_Dirty; }
		
		Signal<uint32_t, uint32_t> OnViewportResized;

	protected:
		bool m_Dirty = false;
		uint32_t m_DirtySize[2];

		uint32_t m_RendererID;
		FramebufferSpecs m_FramebufferSpecs;

		Framebuffer(const FramebufferSpecs& framebufferSpecs);
		virtual void DeleteAttachments() = 0;
		virtual void Rebuild() = 0;
	};
}
