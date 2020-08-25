#pragma once

namespace Engine
{
	class VertexBuffer
	{
	public:
		VertexBuffer();
	
	public:
		void Bind() const;
		void SetData(const float* Vertices, const int size, const unsigned int DrawType);
	
	public:
		inline int GetHandle() const { return Handle; }
	
	private:
	
	
	
	private:
		unsigned int Handle;
	};
	
}
