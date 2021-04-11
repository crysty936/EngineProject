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


		Value& JMaterialsArray = GltfDocument["materials"];
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












		return FileModel;
	}




	void ParserObj::GetMember(rapidjson::Value::ConstObject& Container, const char* MemberName, MemberType Type, void* MemberAdress)
	{
		using namespace rapidjson;
		Value::ConstMemberIterator& Member_Name = Container.FindMember(MemberName);
		const bool ItemFound = Member_Name != Container.MemberEnd();
		bool IsCorrectType = false;

		if (ItemFound)
		{
			switch (Type)
			{
			case MemberType::Bool:
			{
				(*(bool*)MemberAdress) = Member_Name->value.GetBool();
				break;
			}
			case MemberType::Double:
			{
				(*(double*)MemberAdress) = Member_Name->value.GetDouble();
				break;
			}
			case MemberType::Float:
			{
				(*(float*)MemberAdress) = Member_Name->value.GetFloat();
				break;
			}
			case MemberType::String:
			{
				(*(std::string*)MemberAdress) = std::string(Member_Name->value.GetString());
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