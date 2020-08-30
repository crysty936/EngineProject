#pragma once

namespace Engine
{
	class Buffer
	{
	public:
		Buffer(unsigned int _BufferType);
	
	public:
		void Bind() const;
		void SetData(const float* Vertices, const int size, const unsigned int DrawType);
		void SetData(const unsigned int* Indices, const int size, const unsigned int DrawType);

	
	public:
		inline int GetHandle() const { return Handle; }
	
	private:
	
	
	
	private:
		unsigned int Handle;
		unsigned int Type;
	};
	
}
