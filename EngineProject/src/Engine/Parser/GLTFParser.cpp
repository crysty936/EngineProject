#include "pch.h"
#include <fstream>
#include "GLTFParser.h"
#include "Engine/Renderer/Model.h"
#include "Engine/Renderer/TextureUtils.h"
#include "Engine/Renderer/Mesh.h"
#include <filesystem>
#include "rapidjson/document.h"
#include "glm/ext/matrix_transform.inl"

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

		char* bufferData = nullptr;
		GetBufferData(bufferMember, bufferData, inPath);

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
		GetMeshes(meshesMember, accessors, bufferViews, bufferData, meshes);

		Value::MemberIterator& nodesMember = gltfDocument.FindMember("nodes");
		if (nodesMember == gltfDocument.MemberEnd())
			return nullptr;

		std::vector<Node> nodes;
		{
			Value::ConstObject::ValueType& nodesValue = nodesMember->value;
			Value::ConstArray& nodesArrayMember = nodesValue.GetArray();
			uint32_t nodesSize = nodesArrayMember.Size();
			nodes.reserve(nodesSize);


			for (Value::ConstArray::ValueType& node : nodesArrayMember)
			{
				//Nodes have connections only betweem them, they don't refer exactly to a mesh, but child Nodes act relatively as pointers towards the meshes.
				Node newNode;

				Value::ConstObject& obj = node.GetObject();
				GetMember(obj, "name", MemberType::String, &newNode.Name);

				Value::ConstMemberIterator& meshMember = node.FindMember("mesh");
				if (meshMember != node.MemberEnd()) //Mesh Node
				{
					Value::ConstObject::ValueType& meshValue = meshMember->value;
					newNode.MeshIndex = meshValue.GetInt();
					nodes.push_back(newNode);

					continue;
				}

				//Parent Node
				Value::ConstMemberIterator& childrenMember = node.FindMember("children");
				if (childrenMember == node.MemberEnd())
					continue;

				Value::ConstObject::ValueType& childrenValue = childrenMember->value;
				Value::ConstArray& childrenArrayMember = childrenValue.GetArray();
				uint32_t childrenSize = childrenArrayMember.Size();

				newNode.Children.reserve(childrenSize);

				for (Value::ConstArray::ValueType& child : childrenArrayMember)
				{
					newNode.Children.push_back(child.GetUint());
				}

				float m[16] = { 0.f };

				Value::ConstMemberIterator& matrixMember = node.FindMember("matrix");
				if (matrixMember != node.MemberEnd())
				{
					Value::ConstObject::ValueType& matrixArrayValue = matrixMember->value;
					Value::ConstArray& matrixArrayMember = matrixArrayValue.GetArray();

					int i = 0;
					for (Value::ConstArray::ValueType& matrixElement : matrixArrayMember)
					{
						m[i++] = matrixElement.GetFloat();
					}

					glm::mat4& nodeMatrix = newNode.Matrix;
					glm::vec4& firstCol = nodeMatrix[0];
					memcpy(&firstCol.x, &m, sizeof(m));
				}

				nodes.push_back(newNode);
			}
		}









		//Position is made glm::vec4 with 1 at the end and multiplied with the resulting matrix then replaced.
		//Normal is made into glm::vec4 with 0 at the end and multiplied with the resulting matrix then replaced.


		//Mesh (indices to Accessors) ->
		//Accessors(Type, count and indices to BufferViews) ->
		//BufferViews ( Buffer(Nearly always only one so 0), ByteLength(Length in that buffer in bytes starting from byteOffset), byteOfsset(offset of start position)



		std::unique_ptr<Model> fileModel = std::make_unique<Model>(meshes);
		fileModel->SetDirectory(inPath);

		return fileModel;
	}

	void GLTFParser::RecursiveTraverseNodes(const Node& inCurrentNode, glm::mat4 inCurrentMatrix, const std::vector<Node>& InNodes, std::vector<Mesh>& inMeshes)
	{
		static int asd = 0;
		// 		  		if (asd > 10)
		// 		  			return;

		glm::mat4 newMatrix =  inCurrentNode.Matrix * inCurrentMatrix;

// 		glm::vec3 translation(0.f, 0.f, 0.f);
// 		glm::vec3 rotation(1.f, 0.f, 0.f);
// 		glm::vec3 scale(1.f, 1.f, 1.f);
// 
// 		glm::mat4 mat4Translation = glm::translate(glm::mat4(1.f), translation);
// 		glm::mat4 mat4Rotation = glm::rotate(glm::mat4(1.f), 1.f, rotation);
// 		glm::mat4 mat4Scale = glm::scale(glm::mat4(1.f), scale);
// 
// 
// 		glm::mat4 parentRotation = glm::rotate(glm::mat4(1.f), 1.f, rotation);
// 		glm::mat4 parentResult = mat4Translation * parentRotation * mat4Scale;
// 		glm::mat4 result = mat4Translation * mat4Rotation * mat4Scale;
// 		result = result * parentResult;
		glm::mat4 result = newMatrix;


		if (inCurrentNode.MeshIndex != -1) //Mesh node
		{
			Mesh& currentMesh = inMeshes[inCurrentNode.MeshIndex];

			std::vector<Vertex>& meshVertices = currentMesh.AccessVertices();

			for (Vertex& vert : meshVertices)
			{
				glm::vec3 vertexPos = vert.Position;
				glm::vec3 vector;


				glm::vec4 col0 = result[0];
				glm::vec3 col0norm = glm::normalize(col0);

				glm::vec4 col1 = result[1];
				glm::vec3 col1norm = glm::normalize(col1);

				glm::vec4 col2 = result[2];
				glm::vec3 col2norm = glm::normalize(col2);

// 				vector.x = (((col0norm[0] * vertexPos.x) + (col0norm[1] * vertexPos.y)) + (col0norm[2] * vertexPos.z)) + col0[3];
// 				vector.y = (((col1norm[0] * vertexPos.x) + (col1norm[1] * vertexPos.y)) + (col1norm[2] * vertexPos.z)) + col1[3];
// 				vector.z = (((col2norm[0] * vertexPos.x) + (col2norm[1] * vertexPos.y)) + (col2norm[2] * vertexPos.z)) + col2[3];
// 
// 				glm::vec4 col3 = glm::normalize(result[3]);
// 				vector += glm::vec3(col3.x, col3.y, col3.z);
// 
// 				vert.Position = vector;



				// 				glm::vec4 vec4MatrixPosition = newMatrix[3];
				// 				glm::vec3 vec3MatrixPosition = glm::vec3(vec4MatrixPosition.x, vec4MatrixPosition.y, vec4MatrixPosition.z);
				// 				glm::vec3 resultVec3 = vec3MatrixPosition * vert.Position;
				// 				vert.Position = resultVec3;



								//Position
				//   				glm::vec3& meshPosition = vert.Position;
				//  
				//    				glm::vec4 meshPosition4(meshPosition.x, meshPosition.y, meshPosition.z, 1.0f);
				//    				glm::vec4 resultPosition = newMatrix * meshPosition4;
				//    				meshPosition = glm::vec3(resultPosition.x, resultPosition.y, resultPosition.z);

								//Normal
				//  				glm::vec3& meshNormal = vert.Normal;
				//  				glm::vec4 meshNormal4(meshNormal.x, meshNormal.y, meshNormal.z, 0.f);
				//  				glm::vec4 resultNormal = meshNormal4 * newMatrix;
				//  				meshNormal = glm::vec3(resultNormal.x, resultNormal.y, resultNormal.z);
			}
			++asd;
			return;
		}

		for (uint32_t i : inCurrentNode.Children)
		{
			const Node& childNode = InNodes[i];
			RecursiveTraverseNodes(childNode, newMatrix, InNodes, inMeshes);
		}
	}


	void GLTFParser::GetMeshes(rapidjson::Value::MemberIterator& inMeshesMember, const std::vector<Accessor>& inAccessors, const std::vector<BufferView>& inBufferViews, const char* inBufferData, OUT std::vector<class Mesh>& outMeshes)
	{
		using namespace rapidjson;

		Value::ConstObject::ValueType& meshesValue = inMeshesMember->value;
		Value::ConstArray& arrayMember = meshesValue.GetArray();

		uint32_t size = arrayMember.Size();
		outMeshes.reserve(arrayMember.Size());

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

			const Accessor& positionAccessor = inAccessors[positionIndex];
			const Accessor& normalAccessor = inAccessors[normalndex];
			const Accessor& texCoordAccessor = inAccessors[texCoordIndex];
			const Accessor& indicesAccessor = inAccessors[indicesIndex];

			//position
			std::vector<glm::vec3> positions;
			GetVectorFromData(inBufferViews[positionAccessor.BufferViewIndex], positionAccessor, inBufferData, positions);

			//normal
			std::vector<glm::vec3> normals;
			GetVectorFromData(inBufferViews[normalAccessor.BufferViewIndex], normalAccessor, inBufferData, normals);

			//texCoord
			std::vector<glm::vec2> texCoords;
			GetVectorFromData(inBufferViews[texCoordAccessor.BufferViewIndex], texCoordAccessor, inBufferData, texCoords);

			std::vector<uint32_t> indices;
			GetIndices(inBufferViews[indicesAccessor.BufferViewIndex], indicesAccessor, inBufferData, indices);

			std::vector<Vertex> vertices;
			vertices.reserve(positions.size());
			for (int i = 0; i < positions.size(); i++)
			{
				Vertex currentVertex{ positions[i], normals[i], texCoords[i] };
				vertices.push_back(currentVertex);
			}

			std::vector<Texture> asd; //placeholder

			Mesh currentMesh(vertices, indices, asd);
			outMeshes.push_back(currentMesh);
		}

	}

	void GLTFParser::GetIndices(const BufferView& inBufferView, const Accessor& inAccessor, const char* inData, std::vector<uint32_t>& outIndices) const
	{
		if (inAccessor.ComponentType == 5125u) //It's unsigned int, do it normally
		{
			GetVectorFromData(inBufferView, inAccessor, inData, outIndices);
		}
		else //It's unsigned short, need to convert
		{
			std::vector<unsigned short> shortIndices;
			GetVectorFromData(inBufferView, inAccessor, inData, shortIndices);
			outIndices.reserve(shortIndices.size());
			for (unsigned short shortIndice : shortIndices)
			{
				outIndices.push_back(shortIndice);
			}
		}
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

			GetMember(obj, "componentType", MemberType::UInt, &currentAccessor.ComponentType);
			GetMember(obj, "bufferView", MemberType::UInt, &currentAccessor.BufferViewIndex);
			GetMember(obj, "count", MemberType::UInt, &currentAccessor.Count);
			GetMember(obj, "byteOffset", MemberType::UInt, &currentAccessor.ViewByteOffset);

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

	void GLTFParser::GetBufferData(rapidjson::Value::MemberIterator& inbufferMember, OUT char*& outData, const std::string& inFilePath) const
	{
		using namespace rapidjson;

		Value::ConstObject::ValueType& bufferMemberValue = inbufferMember->value;
		Value::ConstArray& arrayMember = bufferMemberValue.GetArray();
		Value::ConstArray::ValueType& theBuffer = arrayMember[0];
		Value::ConstObject& bufferObj = theBuffer.GetObject();

		uint32_t byteLength = 0;
		GetMember(bufferObj, "byteLength", MemberType::UInt, &byteLength);
		outData = new char[byteLength];

		std::string bufferUri;
		GetMember(bufferObj, "uri", MemberType::String, &bufferUri);

		if (bufferUri.find("octet-stream") != std::string::npos) //It's base64, convert it
		{
			size_t lastCommaIndex = bufferUri.find_last_of(',');
			bufferUri.erase(0, lastCommaIndex + 1);
			const size_t stringSize = bufferUri.size();
			const char* data = bufferUri.c_str();
			ConvertFromBase64(data, stringSize, outData);
		}
		else
		{
			size_t lastSlashIndex = inFilePath.find_last_of('/');
			std::string bufferPath = inFilePath;
			bufferPath.erase(lastSlashIndex + 1, bufferPath.size());
			bufferPath.append(bufferUri);

			std::ifstream bufferFile(bufferPath, std::ios_base::binary);

			const uintmax_t size = std::filesystem::file_size(bufferPath);

			bufferFile.read(outData, size);
			if (bufferFile.rdstate() & std::ios_base::eofbit || bufferFile.rdstate() & std::ios_base::badbit)
			{
				__debugbreak();
			}
		}
	}

	void GLTFParser::ConvertFromBase64(const char* inData, const uint32_t inSize, OUT char*& outData) const
	{
		unsigned char bytes3[3] = { 0, 0, 0 };
		unsigned char bytes4[4] = { 0, 0, 0, 0 };

		uint32_t totalLen, convertedTotalIndex;
		totalLen = convertedTotalIndex = 0;

		for (; totalLen < inSize; totalLen += 4, convertedTotalIndex += 3)
		{

			for (uint32_t i = 0; i < 4; i++)
			{
				uint32_t totalIndex = i + totalLen;
				bytes4[i] = inData[totalIndex];
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
				Value::ConstMemberIterator& pbrMember = obj.FindMember("pbrMetallicRoughness");
				if (pbrMember != obj.MemberEnd())
				{
					Value::ConstObject::ValueType& PbrValue = pbrMember->value;
					Value::ConstObject& pbrObject = PbrValue.GetObject();
					Value::ConstArray& baseColorFactorArr = pbrObject.FindMember("baseColorFactor")->value.GetArray();

					metRoughness.BaseColorFactor.reserve(baseColorFactorArr.Size());
					for (Value::ConstArray::ValueType& ColorFactor : baseColorFactorArr)
						metRoughness.BaseColorFactor.push_back(ColorFactor.GetDouble());

					GetMember(pbrObject, "metallicFactor", MemberType::Double, &metRoughness.MetallicFactor);
					GetMember(pbrObject, "roughnessFactor", MemberType::Double, &metRoughness.RoughnessFactor);
				}
			}
			outMaterials.push_back(mat);
		}
	}

	char GLTFParser::GetDecimalFromBase64(const char inByte) const
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
			//__debugbreak();
		}
	}






}