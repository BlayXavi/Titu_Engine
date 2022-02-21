#pragma once

#include "TituEngine/Core/Core.h"
#include "TituEngine/Renderer/RendererUtilities.h"

namespace TituEngine
{
	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Offset;
		uint32_t Size;
		uint32_t ElementCount;
		uint32_t Normalized;

		BufferElement(const ShaderDataType type, const bool normalized, const std::string name)
			: Type(type), Normalized(normalized), Name(name), Size(ShaderDataTypeSize(type)), Offset(0), ElementCount(ShaderDataTypeElementCount(type))
		{

		}
	};

	class BufferLayout
	{
	public:

		BufferLayout() = default;
		virtual ~BufferLayout() = default;

		BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements), m_Stride(0)
		{
			CalculateOffsetsAndStride();
		}

		const std::vector<BufferElement> GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

		inline uint32_t GetStride() const { return m_Stride; }

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;

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
		virtual ~Buffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

	private:
		BufferLayout m_Layout;
	};

	class VertexBuffer : public Buffer
	{
	public:
		virtual ~VertexBuffer() = default;
		static VertexBuffer* Create(float* vertices, uint32_t size, bool isStatic = true);
		virtual void SetData(const void* data, uint32_t size) = 0;
	};

	class IndexBuffer : public Buffer
	{
	public:
		virtual ~IndexBuffer() = default;
		static IndexBuffer* Create(uint32_t* vertices, uint32_t count, bool isStatic = true);

		virtual uint32_t GetCount() const = 0;
	};

}