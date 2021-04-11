#include "pch.h"
#include <fstream>
#include "ParserObj.h"
#include "Engine/Renderer/Model.h"
#include "Engine/Renderer/TextureUtils.h"
#include <filesystem>

namespace Engine
{
	struct MetallicRoughness
	{
		std::vector<double> BaseColorFactor;
		double MetallicFactor;
		double RoughnessFactor;
	};

	struct Material
	{
		bool DoubleSided = false;
		std::string Name;
		MetallicRoughness PbrMetallicRoughness;
	};



	std::unique_ptr<Model> ParserObj::ParseGetModel(std::string Path)
	{
		using namespace rapidjson;

		std::unique_ptr<Model> FileModel = std::make_unique<Model>();
		FileModel->SetDirectory(Path);

		std::ifstream InFile(Path);
		// 		json j = json::ParseStream(InFile);


		//Fast file read
		const unsigned long long size = std::filesystem::file_size(Path);
		std::string Buffer(size, '\0');
		InFile.read(Buffer.data(), size);
		Document GltfDocument;
		GltfDocument.Parse(Buffer.c_str());




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


		Value::ConstObject::ValueType& JMaterialsArray = GltfDocument.FindMember("materials")->value;
		assert(!JMaterialsArray.IsNull());
		std::vector<Material> Materials;
		Materials.reserve(JMaterialsArray.Size());

		//for (Value::ConstValueIterator MaterialsIt = JMaterialsArray.Begin(); MaterialsIt != JMaterialsArray.End(); ++MaterialsIt)
		for (Value::ConstArray::ValueType& MaterialMember : JMaterialsArray.GetArray())
		{
			Material M;
			Value::ConstObject& Obj = MaterialMember.GetObject();

			{
				GetMember(Obj, "doubleSided", MemberType::Bool, &M.DoubleSided);
				GetMember(Obj, "name", MemberType::String, &M.Name);
			}
			{
				MetallicRoughness& Mr = M.PbrMetallicRoughness;
				Value::ConstObject::ValueType& PbrValue = Obj.FindMember("pbrMetallicRoughness")->value;
				Value::ConstObject& PbrObject = PbrValue.GetObject();
				Value::ConstArray& BaseColorFactorArr = PbrObject.FindMember("baseColorFactor")->value.GetArray();

				Mr.BaseColorFactor.reserve(BaseColorFactorArr.Size());
				for (Value::ConstArray::ValueType& ColorFactor : BaseColorFactorArr)
					Mr.BaseColorFactor.push_back(ColorFactor.GetDouble());

				GetMember(PbrObject, "metallicFactor", MemberType::Double, &Mr.MetallicFactor);
				GetMember(PbrObject, "roughnessFactor", MemberType::Double, &Mr.RoughnessFactor);
			}
			Materials.push_back(M);
		}

		Value::MemberIterator& BufferMember = GltfDocument.FindMember("buffers");
		if (BufferMember != GltfDocument.MemberEnd())
		{
			Value::ConstObject::ValueType& BufferMemberValue = GltfDocument.FindMember("buffers")->value;

			Value::ConstArray& ArrayMember = BufferMemberValue.GetArray();
			Value::ConstArray::ValueType& TheBuffer = ArrayMember[0];
			Value::ConstObject& BufferObj = TheBuffer.GetObject();

			uint32_t ByteLength;
			GetMember(BufferObj, "byteLength", MemberType::UInt, &ByteLength);

			std::string BufferUri;
			GetMember(BufferObj, "uri", MemberType::String, &BufferUri);

			if (BufferUri.find("octet-stream") != std::string::npos)
			{
				size_t LastCommaIndex = BufferUri.find_last_of(',');
				BufferUri.erase(0, LastCommaIndex + 1);
				const char* Data = BufferUri.c_str();

				std::vector<float> FloatVector;
				FloatVector.resize(288 / sizeof(float)); // Count 24 -> 288 bytes = 72 floats = 24 Vec3s
				memcpy(FloatVector.data(), Data, 288);


			}




		}



		//Mesh (indices to Accessors) ->
		//Accessors(Type, count and indices to BufferViews) ->
		//BufferViews ( Buffer(Nearly always only one so 0), ByteLength(Length in that buffer in bytes starting from byteOffset), byteOfsset(offset of start position)






















		return FileModel;
	}




	void ParserObj::GetMember(rapidjson::Value::ConstObject& Container, const char* MemberName, MemberType Type, OUT void* MemberAdress)
	{
		using namespace rapidjson;
		Value::ConstMemberIterator& Member = Container.FindMember(MemberName);
		const bool ItemFound = Member != Container.MemberEnd();
		bool IsCorrectType = false;

		if (ItemFound)
		{
			switch (Type)
			{
			case MemberType::Bool:
			{
				(*(bool*)MemberAdress) = Member->value.GetBool();
				break;
			}
			case MemberType::Double:
			{
				(*(double*)MemberAdress) = Member->value.GetDouble();
				break;
			}
			case MemberType::Float:
			{
				(*(float*)MemberAdress) = Member->value.GetFloat();
				break;
			}
			case MemberType::String:
			{
				(*(std::string*)MemberAdress) = Member->value.GetString();
				break;
			}
			case MemberType::UInt:
			{
				(*(uint32_t*)MemberAdress) = Member->value.GetUint();
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