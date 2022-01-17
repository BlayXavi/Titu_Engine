#pragma once
#include "TituEngine/Core/Core.h"

namespace TituEngine
{
	enum class FramebufferTextureFormat
	{
		NONE = 0,

		//Color
		RBA8,

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

		virtual const uint32_t GetColorAttachment(uint32_t index = 0) const = 0;

	protected:
		uint32_t m_RendererID;
		FramebufferSpecs m_FramebufferSpecs;

		Framebuffer(const FramebufferSpecs& framebufferSpecs);
		virtual void DeleteAttachments() = 0;
		virtual void Rebuild() = 0;
	};
}
