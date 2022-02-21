#pragma once

#include "Buffer.h"

namespace TituEngine
{
	class Mesh
	{
	public:

		static Mesh* Create(const std::string& path);
	
	private:

		Mesh();
		virtual ~Mesh() = default;

	private:
		VertexBuffer* m_VertexBuffer = nullptr;

	protected:
	};
}