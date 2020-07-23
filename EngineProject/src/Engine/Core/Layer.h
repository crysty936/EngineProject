#pragma once
#include "pch.h"


namespace Engine
{
	class Layer
	{
	public:
		Layer(const std::string& name)
			:m_Name(name) {}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}


		inline const std::string& GetName() const { return m_Name; }
	private:
		std::string m_Name;
	};
}




