#pragma once
#include "glm/glm.hpp"

namespace Engine
{
	class RenderObject
	{
	public:
		RenderObject(glm::vec3 _Transform, class VertexArray* _VAO, class Shader* _ObjectShader);
		RenderObject();

		~RenderObject();

		FORCEINLINE glm::vec3&			GetTransform() { return Transform; }
		FORCEINLINE class VertexArray*	GetVAO() const { return VAO; }
		FORCEINLINE class Shader*		GetShader() const { return ObjectShader; }
		
		FORCEINLINE void				SetTransform(glm::vec3 _Transform) { Transform = _Transform; }
		FORCEINLINE void				SetVAO(class VertexArray* _VAO) { VAO = _VAO; }
		FORCEINLINE void				SetShader(class Shader* _ObjectShader) { ObjectShader = _ObjectShader; }

	private:
		glm::vec3 Transform;
		class VertexArray* VAO;
		class Shader* ObjectShader;


	};







}