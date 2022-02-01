#pragma once

#include "TituEngine/Core/Core.h"

namespace TituEngine
{
	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;
		virtual void SetData(const void* data, const uint32_t& size, const uint32_t& offset = 0) = 0;

		static UniformBuffer* Create(const uint32_t& size, const uint32_t& binding);
	};

}