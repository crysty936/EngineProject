#pragma once

namespace Engine
{
	class BaseBuffer
	{
	protected:
		BaseBuffer();
		virtual ~BaseBuffer();

	public:
		virtual void Bind() const;
		virtual void UnBind() const;
		virtual FORCEINLINE uint32_t GetHandle() const;


	protected:
		uint32_t Handle;
	};
}