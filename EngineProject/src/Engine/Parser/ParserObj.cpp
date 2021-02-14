#include "pch.h"
#include <fstream>
#include "ParserObj.h"
#include "Engine/Renderer/Model.h"

namespace Engine
{

	Model* ParserObj::ParseGetModel(std::string Path)
	{
		Model* FileModel = new Model();
		std::ifstream InFile(Path);
 
		std::string Line;

 		std::getline(InFile, Line);
 
 
 		while (Line.c_str()[0] != 'o')
 		{
 			std::getline(InFile, Line);
 		}

		char c;
		float x, y, z;
		InFile >> c;

		while (c == 'v')
		{
			InFile >> x >> y >> z >> c;
		}



		while (InFile >> c >> x >> y >> z)
		{



		}







		return FileModel;
	}
}