#pragma once


namespace Engine {

	class IRenderingContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}
