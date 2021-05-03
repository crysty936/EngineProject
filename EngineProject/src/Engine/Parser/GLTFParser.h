#pragma once

#include <string>
#include "rapidjson/document.h"

namespace Engine
{
	struct MetallicRoughness
	{
		std::vector<double> BaseColorFactor;
		double MetallicFactor = 0;
		double RoughnessFactor = 0;
	};

	struct Material
	{
		bool DoubleSided = false;
		std::string Name;
		MetallicRoughness PbrMetallicRoughness;
	};


	struct BufferView
	{
		//uint32_t BufferIndex = 0;
		uint32_t ByteLength = 0;
		uint32_t ByteOffset = 0;
	};

	struct Accessor
	{
		uint32_t BufferViewIndex = 0;
		uint32_t Count = 0;
	};

	enum class MemberType
	{
		String,
		Bool,
		Float,
		Double,
		UInt
	};



	class GLTFParser
	{
	public:
		static GLTFParser& GetInstance()
		{
			static GLTFParser s_Instance; //lazy thread safe instantiation
			return s_Instance;
		}

	public:
		std::unique_ptr<class Model> ParseGetModel(std::string inPath);

	private:
		void GetAccessors(class rapidjson::Value::MemberIterator& inAccessorMember, OUT std::vector<Accessor>& outAccessors) const;
		void GetBufferViews(class rapidjson::Value::MemberIterator& inbufferViewsMember, OUT std::vector<BufferView>& outBufferViews) const;
		void GetConvertedBase64(class rapidjson::Value::MemberIterator& inbufferMember, OUT unsigned char*& outData) const;
		void GetMaterials(class rapidjson::Value::MemberIterator& inMaterialsMember, OUT std::vector<Material>& outMaterials) const;
		void GetMember(class rapidjson::Value::ConstObject& inContainer, const char* inMemberName, MemberType Type, OUT void* outMemberAdress) const;
		unsigned char GetDecimalFromBase64(const char inByte) const;

		template<typename T>
		void GetVectorFromData(const BufferView& inBufferView, const Accessor& inAccessor, const unsigned char* inData, OUT std::vector<T>& outVector) const
		{
			const uint32_t count = inAccessor.Count;

			const uint32_t accessorByteLength = count * sizeof(T);
			if (accessorByteLength != inBufferView.ByteLength)
			{
				__debugbreak();
			}

			outVector.resize(count);
			memcpy(outVector.data(), inData + inBufferView.ByteOffset, inBufferView.ByteLength);
		}

	};
}