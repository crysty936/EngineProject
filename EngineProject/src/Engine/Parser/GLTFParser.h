#pragma once

#include <string>
#include "rapidjson/document.h"
#include "glm/glm.hpp"

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
		uint32_t ComponentType = 0;
		uint32_t ViewByteOffset = 0;
	};

	struct Node
	{
		std::vector<uint32_t> Children;
		glm::mat4 Matrix = glm::mat4(1);
		std::string Name;
		int32_t MeshIndex = -1;
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
		void RecursiveTraverseNodes(const Node& inCurrentNode, glm::mat4 inCurrentMatrix, const std::vector<Node>& InNodes, std::vector<class Mesh>& inMeshes);
		void GetMeshes(class rapidjson::Value::MemberIterator& inMeshesMember, const std::vector<Accessor>& inAccessors, const std::vector<BufferView>& inBufferViews, const char* inBufferData, OUT std::vector<class Mesh>& outMeshes);
		void GetIndices(const BufferView& inBufferView, const Accessor& inAccessor, const char* inData, std::vector<uint32_t>& outIndices) const;
		void GetAccessors(class rapidjson::Value::MemberIterator& inAccessorMember, OUT std::vector<Accessor>& outAccessors) const;
		void GetBufferViews(class rapidjson::Value::MemberIterator& inbufferViewsMember, OUT std::vector<BufferView>& outBufferViews) const;
		void GetBufferData(rapidjson::Value::MemberIterator& inbufferMember, OUT char*& outData, const std::string& inFilePath) const;
		void ConvertFromBase64(const char* inData, const uint32_t inSize, OUT char*& outData) const;
		void GetMaterials(class rapidjson::Value::MemberIterator& inMaterialsMember, OUT std::vector<Material>& outMaterials) const;
		void GetMember(class rapidjson::Value::ConstObject& inContainer, const char* inMemberName, MemberType Type, OUT void* outMemberAdress) const;
		char GetDecimalFromBase64(const char inByte) const;

		template<typename T>
		void GetVectorFromData(const BufferView& inBufferView, const Accessor& inAccessor, const char* inData, OUT std::vector<T>& outVector) const
		{
			const uint32_t count = inAccessor.Count;
			const size_t accessorByteLength = count * sizeof(T);

			outVector.resize(count);
			memcpy(outVector.data(), inData + inBufferView.ByteOffset + inAccessor.ViewByteOffset, accessorByteLength);
		}

	};
}