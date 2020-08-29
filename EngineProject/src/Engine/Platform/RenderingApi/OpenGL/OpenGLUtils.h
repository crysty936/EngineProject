#pragma once

namespace Engine {
	class OpenGLUtils
	{
		struct GLenum;
	public:
		static GLenum GlCheckError_(const char* file, int line);
	};
#define glCheckError() OpenGLUtils::GlCheckError_(__FILE__,__LINE__)

}
