#include "pch.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"


namespace Engine
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &Handle);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &Handle);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(Handle);
	}

	void VertexArray::AddBuffer(const VertexBuffer& VBuffer, const VertexBufferLayout& Layout)
	{
		Bind();
		VBuffer.Bind();

		const std::vector<LayoutProperties>& Properties = Layout.GetProperties();
		uint32_t offset = 0;

		for (int i = 0; i < Properties.size(); i++)
		{
			const auto& Property = Properties[i];

			glVertexAttribPointer(i, Property.Count, Property.Type, Property.Normalized, Layout.GetStride(), (void*)offset);
			glEnableVertexAttribArray(i);

			offset += Property.Count * LayoutProperties::GetSizeOfType(Property.Type);
		}

	}
	void VertexArray::Unbind()
	{
		glBindVertexArray(0);
	}
}
