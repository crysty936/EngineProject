#pragma once
#include "glm/glm.hpp"

namespace Engine
{
	class RenderObject
	{
	public:
		RenderObject(glm::vec3 _Transform);
		RenderObject();

		~RenderObject();

		FORCEINLINE glm::vec3*	GetTransform() { return &Transform; }
		FORCEINLINE void		SetTransform(glm::vec3 _Transform) { Transform = _Transform; }




	private:
		glm::vec3 Transform;
	};







}