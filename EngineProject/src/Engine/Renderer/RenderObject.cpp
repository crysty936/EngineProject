#include "pch.h"
#include "RenderObject.h"
#include "VertexArray.h"
#include "Shader.h"

namespace Engine
{
	RenderObject::RenderObject(glm::vec3 _Transform, VertexArray* _VAO, Shader* _Shader)
		:Transform(_Transform), VAO(_VAO), ObjectShader(_Shader)
	{

	}

	RenderObject::RenderObject()
	{
	}

	RenderObject::~RenderObject()
	{
	}

}