#pragma once
#include "FusionCore.h"

namespace Fusion
{
	struct File
	{
		std::string FilePath;
		std::string FileContents;
	};

	namespace FileLoader
	{
		std::string GetFullFilePath(std::string filePath)
		{
			std::string toReplace = "res:";

			std::size_t pos = filePath.find(toReplace);
			if (pos == std::string::npos) return (char*)("");
			filePath.replace(pos, toReplace.length(), FS_RESOURCE_LOCATION);

			return filePath;
		}

		// Calls GetFullFilePath automatically
		File GetFile(std::string filePath)
		{
			// Convert res:/ to resource location
			std::string FilePath = GetFullFilePath(filePath);

			std::ifstream stream(FilePath);
			std::stringstream strStream;
			strStream << stream.rdbuf();
			std::string fileContents = strStream.str();

			if (fileContents.empty())
			{
				printf("[FILE LOADER][ERROR] Failed to load file from filepath: (%s)\n", filePath.c_str());
			}

			File file;
			file.FilePath = FilePath;
			file.FileContents = fileContents;

			return file;
		}
	};

}