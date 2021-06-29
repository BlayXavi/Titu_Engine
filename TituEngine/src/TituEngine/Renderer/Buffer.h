#pragma once

#include "TituEngine/Core.h"

namespace TituEngine
{
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

	static uint ShaderDataTypeSize(const ShaderDataType type)
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

	static uint ShaderDataTypeElementCount(const ShaderDataType type)
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

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint Offset;
		uint Size;
		uint ElementCount;
		uint Normalized;

		BufferElement(const ShaderDataType type, const bool normalized, const std::string name)
			: Type(type), Normalized(normalized), Name(name), Size(ShaderDataTypeSize(type)), Offset(0), ElementCount(ShaderDataTypeElementCount(type))
		{

		}
	};

	class BufferLayout
	{
	public:

		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements), m_Stride(0)
		{
			CalculateOffsetsAndStride();
		}

		inline const std::vector<BufferElement>& GetElements() { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

		inline uint GetStride() const { return m_Stride; }

	private:
		std::vector<BufferElement> m_Elements;
		uint m_Stride = 0;

		void CalculateOffsetsAndStride()
		{
			m_Stride = 0;
			for (size_t i = 0; i < m_Elements.size(); i++)
			{
				BufferElement& element = m_Elements[i];
				element.Offset = m_Stride; //element offset if the current stride
				m_Stride += element.Size;
			}
		}
	};

	class Buffer
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		const BufferLayout& GetLayout() const { return m_Layout; }
		void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

	private:
		BufferLayout m_Layout;
	};

	class VertexBuffer : public Buffer
	{
	public:
		~VertexBuffer() = default;
		static VertexBuffer* Create(float* vertices, uint size);
	};

	class IndexBuffer : public Buffer
	{
	public:
		~IndexBuffer() = default;

		static IndexBuffer* Create(uint* vertices, uint size);
		virtual uint GetCount() const = 0;
	};

}