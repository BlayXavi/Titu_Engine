#pragma once
#include "TituEngine/Core/Core.h"

#include "Signals.h"

namespace 
{

	Signal<uint32_t, uint32_t> OnViewportResized;

	enum class ShaderDataType
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	//Return # of bytes of type
	static uint32_t ShaderDataTypeSize(const ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 4 * 2;
		case ShaderDataType::Float3:	return 4 * 3;
		case ShaderDataType::Float4:	return 4 * 4;
		case ShaderDataType::Mat3:		return 4 * 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4 * 4;
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 4 * 2;
		case ShaderDataType::Int3:		return 4 * 3;
		case ShaderDataType::Int4:		return 4 * 4;
		case ShaderDataType::Bool:		return 1;
		}

		TE_ASSERT(false, "Undefined SHader Data Type");
		return 0;
	}

	static uint32_t ShaderDataTypeElementCount(const ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Int:
		case ShaderDataType::Bool:
			return 1;
		case ShaderDataType::Float2:
		case ShaderDataType::Int2:
			return 2;
		case ShaderDataType::Float3:
		case ShaderDataType::Int3:
			return 3;
		case ShaderDataType::Float4:
		case ShaderDataType::Int4:
			return 4;
		case ShaderDataType::Mat3:
			return 9;
		case ShaderDataType::Mat4:
			return 16;
		}

		TE_ASSERT(false, "Undefined SHader Data Type");
		return 0;
	}
}
