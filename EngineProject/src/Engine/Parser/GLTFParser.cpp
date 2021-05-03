#include "pch.h"
#include <fstream>
#include "GLTFParser.h"
#include "Engine/Renderer/Model.h"
#include "Engine/Renderer/TextureUtils.h"
#include "Engine/Renderer/Mesh.h"
#include <filesystem>
#include "rapidjson/document.h"

namespace Engine
{
	std::unique_ptr<Model> GLTFParser::ParseGetModel(std::string inPath)
	{
		using namespace rapidjson;



		std::ifstream gltfFile(inPath);

		//Fast file read
		const unsigned long long size = std::filesystem::file_size(inPath);
		std::string buffer(size, '\0');
		gltfFile.read(buffer.data(), size);
		Document gltfDocument;
		gltfDocument.Parse(buffer.c_str());




		// 		auto a = j.find("images");
		// 		//if(JImages)
		// 		 //json& JImages = .value();
		// 		std::vector<Texture> Textures;
		// 
		// // 		for (const auto& ImagesIt : JImages.items())
		// // 		{
		// // 			json& Obj = ImagesIt.value();
		// // 			std::string RelPath = Obj["uri"].get<std::string>();
		// // 			RelPath.insert(0, Path);
		// // 			Texture NewTexture;
		// // 			TextureUtils::LoadTexture(RelPath.c_str(), NewTexture.Id);;
		// // 			Textures.push_back(NewTexture);
		// // 		}
		// 
		// 
		// 

		//Materials
		Value::MemberIterator& materialsMember = gltfDocument.FindMember("materials");
		if (materialsMember == gltfDocument.MemberEnd())
			return nullptr;

		std::vector<Material> materials;
		GetMaterials(materialsMember, materials);

		//Buffer
		Value::MemberIterator& bufferMember = gltfDocument.FindMember("buffers");
		if (bufferMember == gltfDocument.MemberEnd())
			return nullptr;

		unsigned char* convertedData = nullptr;
		//GetConvertedBase64(bufferMember, convertedData);

		//BufferViews
		Value::MemberIterator& bufferViewMember = gltfDocument.FindMember("bufferViews");
		if (bufferViewMember == gltfDocument.MemberEnd())
			return nullptr;

		std::vector<BufferView> bufferViews;
		GetBufferViews(bufferViewMember, bufferViews);


		//Accessors
		Value::MemberIterator& accessorsMember = gltfDocument.FindMember("accessors");
		if (accessorsMember == gltfDocument.MemberEnd())
			return nullptr;

		std::vector<Accessor> accessors;
		GetAccessors(accessorsMember, accessors);


		//Meshes
		Value::MemberIterator& meshesMember = gltfDocument.FindMember("meshes");
		if (meshesMember == gltfDocument.MemberEnd())
			return nullptr;

		std::vector<Mesh> meshes;
		{
			Value::ConstObject::ValueType& meshesValue = meshesMember->value;
			Value::ConstArray& arrayMember = meshesValue.GetArray();

			uint32_t size = arrayMember.Size();
			accessors.reserve(arrayMember.Size());

			for (Value::ConstArray::ValueType& mesh : arrayMember)
			{
				Value::ConstObject& obj = mesh.GetObject();

				std::string meshName;
				GetMember(obj, "name", MemberType::String, &meshName);

				Value::ConstMemberIterator& primitivesMembers = mesh.FindMember("primitives");
				Value::ConstObject::ValueType& primitivesValue = primitivesMembers->value;

				Value::ConstArray& primitivesArrayMember = primitivesValue.GetArray();
				uint32_t primitivesSize = primitivesArrayMember.Size();
				Value::ConstArray::ValueType& thePrimitive = primitivesArrayMember[0];


				//Attributes
				Value::ConstMemberIterator& attributesMember = thePrimitive.FindMember("attributes");
				Value::ConstObject::ValueType& attributesValue = attributesMember->value;
				Value::ConstObject& attributesObj = attributesValue.GetObject();

				uint32_t positionIndex = 0;
				uint32_t normalndex = 0;
				uint32_t texCoordIndex = 0;

				GetMember(attributesObj, "POSITION", MemberType::UInt, &positionIndex);
				GetMember(attributesObj, "NORMAL", MemberType::UInt, &normalndex);
				GetMember(attributesObj, "TEXCOORD_0", MemberType::UInt, &texCoordIndex);


				//primitives
				Value::ConstObject& primitiveObj = thePrimitive.GetObject();

				uint32_t indicesIndex = 0;
				uint32_t materialIndex = 0;

				GetMember(primitiveObj, "indices", MemberType::UInt, &indicesIndex);
				GetMember(primitiveObj, "material", MemberType::UInt, &materialIndex);



				const Accessor& positionAccessor = accessors[positionIndex];
				const Accessor& normalAccessor = accessors[normalndex];
				const Accessor& texCoordAccessor = accessors[texCoordIndex];
				const Accessor& indicesAccessor = accessors[indicesIndex];

				//position
				std::vector<glm::vec3> positions;
				GetVectorFromData(bufferViews[positionAccessor.BufferViewIndex], positionAccessor, convertedData, positions);

				//normal
				std::vector<glm::vec3> normals;
				GetVectorFromData(bufferViews[normalAccessor.BufferViewIndex], normalAccessor, convertedData, normals);

				//texCoord
				std::vector<glm::vec2> texCoords;
				GetVectorFromData(bufferViews[texCoordAccessor.BufferViewIndex], texCoordAccessor, convertedData, texCoords);

				//indices
				//std::vector<uint32_t> indices;
				std::vector<unsigned short> indices; //testModel Workaround
				GetVectorFromData(bufferViews[indicesAccessor.BufferViewIndex], indicesAccessor, convertedData, indices);
				std::vector<uint32_t> uIndices;
				for (int i = 0; i < indices.size(); i++)	
				{
					uIndices.push_back(indices[i]); //testModel Workaround
				}


				std::vector<Vertex> vertices;
				vertices.reserve(positions.size());
				for (int i = 0; i < positions.size(); i++)
				{
					Vertex currentVertex{ positions[i], normals[i], texCoords[i] };
					vertices.push_back(currentVertex);
				}
				std::vector<Texture> asd; //placeholder

				Mesh currentMesh(vertices, uIndices, asd);
				meshes.push_back(currentMesh);
			}




		}



		//Mesh (indices to Accessors) ->
		//Accessors(Type, count and indices to BufferViews) ->
		//BufferViews ( Buffer(Nearly always only one so 0), ByteLength(Length in that buffer in bytes starting from byteOffset), byteOfsset(offset of start position)



		std::unique_ptr<Model> fileModel = std::make_unique<Model>(meshes);
		fileModel->SetDirectory(inPath);

		return fileModel;
	}

	void GLTFParser::GetAccessors(rapidjson::Value::MemberIterator& inAccessorsMember, OUT std::vector<Accessor>& outAccessors) const
	{
		using namespace rapidjson;

		Value::ConstObject::ValueType& accessorValue = inAccessorsMember->value;
		Value::ConstArray& arrayMember = accessorValue.GetArray();
		uint32_t size = arrayMember.Size();
		outAccessors.reserve(arrayMember.Size());

		for (Value::ConstArray::ValueType& accessor : arrayMember)
		{
			Value::ConstObject& obj = accessor.GetObject();
			Accessor currentAccessor;

			uint32_t bufferViewIndex = 0;
			uint32_t count = 0;
			std::string elementType;
			GetMember(obj, "bufferView", MemberType::UInt, &currentAccessor.BufferViewIndex);
			GetMember(obj, "count", MemberType::UInt, &currentAccessor.Count);

			outAccessors.push_back(currentAccessor);
		}
	}

	void GLTFParser::GetBufferViews(rapidjson::Value::MemberIterator& inbufferViewsMember, OUT std::vector<BufferView>& outBufferViews) const
	{
		using namespace rapidjson;

		Value::ConstObject::ValueType& bufferViewemberValue = inbufferViewsMember->value;
		Value::ConstArray& arrayMember = bufferViewemberValue.GetArray();
		uint32_t size = arrayMember.Size();
		outBufferViews.reserve(arrayMember.Size());

		for (Value::ConstArray::ValueType& bufferView : arrayMember)
		{
			Value::ConstObject& obj = bufferView.GetObject();

			BufferView view;
			//GetMember(obj, "buffer", MemberType::UInt, &view.BufferIndex);
			GetMember(obj, "byteLength", MemberType::UInt, &view.ByteLength);
			GetMember(obj, "byteOffset", MemberType::UInt, &view.ByteOffset);
			outBufferViews.push_back(view);
		}
	}

	void GLTFParser::GetConvertedBase64(rapidjson::Value::MemberIterator& inbufferMember, OUT unsigned char*& outData) const
	{
		using namespace rapidjson;

		Value::ConstObject::ValueType& bufferMemberValue = inbufferMember->value;
		Value::ConstArray& arrayMember = bufferMemberValue.GetArray();
		Value::ConstArray::ValueType& theBuffer = arrayMember[0];
		Value::ConstObject& bufferObj = theBuffer.GetObject();

		uint32_t byteLength = 0;
		GetMember(bufferObj, "byteLength", MemberType::UInt, &byteLength);
		outData = new unsigned char[byteLength];

		std::string bufferUri;
		GetMember(bufferObj, "uri", MemberType::String, &bufferUri);

		if (bufferUri.find("octet-stream") != std::string::npos)
		{
			size_t LastCommaIndex = bufferUri.find_last_of(',');
			bufferUri.erase(0, LastCommaIndex + 1);
			const uint32_t stringSize = bufferUri.size();
			const char* data = bufferUri.c_str();

			unsigned char bytes3[3] = { 0, 0, 0 };
			unsigned char bytes4[4] = { 0, 0, 0, 0 };

			// 				bytes4[0] = 16; Q
			// 				bytes4[1] = 20;	U		//test data
			// 				bytes4[2] = 9;	J
			// 				bytes4[3] = 3;	D

			uint32_t totalLen, convertedTotalIndex;
			totalLen = convertedTotalIndex = 0;
			for (; totalLen < stringSize; totalLen += 4, convertedTotalIndex += 3)
			{

				for (uint32_t i = 0; i < 4; i++)
				{
					uint32_t totalIndex = i + totalLen;
					bytes4[i] = data[totalIndex];
				}

				for (int j = 0; j < 4; j++)
					bytes4[j] = GetDecimalFromBase64(bytes4[j]);

				bytes3[0] = (bytes4[0] << 2) | ((bytes4[1] & 0x30) >> 4);
				bytes3[1] = ((bytes4[1]) << 4) | ((bytes4[2] & 0x3C) >> 2);
				bytes3[2] = ((bytes4[2]) << 6) | bytes4[3];

				for (int j = 0; j < 3; j++)
				{
					uint32_t convertedIndex = j + convertedTotalIndex;
					outData[convertedIndex] = bytes3[j];
				}
			}

			// 			std::vector<float> floatVector;
			// 			floatVector.resize(288 / sizeof(float)); // Count 24 -> 288 bytes = 72 floats = 24 Vec3s
			// 			memcpy(floatVector.data(), outData, 288);
		}
	}

	void GLTFParser::GetMaterials(rapidjson::Value::MemberIterator& inMaterialsMember, OUT std::vector<Material>& outMaterials) const
	{
		using namespace rapidjson;

		Value::ConstObject::ValueType& materialsMemberValue = inMaterialsMember->value;
		//assert(!materialsMemberValue.IsNull());
		Value::ConstArray& arrayMember = materialsMemberValue.GetArray();
		outMaterials.reserve(arrayMember.Size());

		for (Value::ConstArray::ValueType& material : arrayMember)
		{
			Material mat;
			Value::ConstObject& obj = material.GetObject();

			{
				GetMember(obj, "doubleSided", MemberType::Bool, &mat.DoubleSided);
				GetMember(obj, "name", MemberType::String, &mat.Name);
			}
			{
				MetallicRoughness& metRoughness = mat.PbrMetallicRoughness;
				Value::ConstObject::ValueType& PbrValue = obj.FindMember("pbrMetallicRoughness")->value;
				Value::ConstObject& pbrObject = PbrValue.GetObject();
				Value::ConstArray& baseColorFactorArr = pbrObject.FindMember("baseColorFactor")->value.GetArray();

				metRoughness.BaseColorFactor.reserve(baseColorFactorArr.Size());
				for (Value::ConstArray::ValueType& ColorFactor : baseColorFactorArr)
					metRoughness.BaseColorFactor.push_back(ColorFactor.GetDouble());

				GetMember(pbrObject, "metallicFactor", MemberType::Double, &metRoughness.MetallicFactor);
				GetMember(pbrObject, "roughnessFactor", MemberType::Double, &metRoughness.RoughnessFactor);
			}
			outMaterials.push_back(mat);
		}
	}

	unsigned char GLTFParser::GetDecimalFromBase64(const char inByte) const
	{
		if (inByte == '/' || inByte == '+') //NonAlpha
		{
			if (inByte == '+')
				return 62;
			if (inByte == '/')
				return 63;
		}
		else if (inByte >= 'A' && inByte <= 'z') //Alpha
		{
			if (inByte >= 'A' && inByte <= 'Z')
			{
				return inByte - 65;
			}
			else
			{
				return inByte - 71;
			}
		}
		else // Decimal
		{
			return inByte + 4;
		}
		return 0;
	}


	void GLTFParser::GetMember(rapidjson::Value::ConstObject& inContainer, const char* inMemberName, MemberType Type, OUT void* outMemberAdress) const
	{
		using namespace rapidjson;
		Value::ConstMemberIterator& Member = inContainer.FindMember(inMemberName);
		const bool ItemFound = Member != inContainer.MemberEnd();
		bool IsCorrectType = false;

		if (ItemFound)
		{
			switch (Type)
			{
			case MemberType::Bool:
			{
				(*(bool*)outMemberAdress) = Member->value.GetBool();
				break;
			}
			case MemberType::Double:
			{
				(*(double*)outMemberAdress) = Member->value.GetDouble();
				break;
			}
			case MemberType::Float:
			{
				(*(float*)outMemberAdress) = Member->value.GetFloat();
				break;
			}
			case MemberType::String:
			{
				(*(std::string*)outMemberAdress) = Member->value.GetString();
				break;
			}
			case MemberType::UInt:
			{
				(*(uint32_t*)outMemberAdress) = Member->value.GetUint();
				break;
			}
			}
		}
		else
		{
			__debugbreak();
		}
	}






}