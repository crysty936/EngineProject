#pragma once

namespace Engine
{
	class Buffer
	{
	public:
		Buffer(unsigned int _BufferType);
	
	public:
		void Bind() const;
		void SetData(const float* Vertices, const int count, const uint32_t DrawType);
		void SetData(const uint32_t* Indices, const int count, const uint32_t DrawType);

	
	public:
		inline int GetHandle() const { return Handle; }
	
	private:
	
	
	
	private:
		uint32_t Handle;
		uint32_t Type;
		uint32_t Count;
	};
	
}
