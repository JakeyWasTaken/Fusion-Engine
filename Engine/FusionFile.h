#pragma once
#include "FusionCore.h"

namespace Fusion
{
	struct File
	{
		std::string FilePath;
		std::string FileContents;
	};

	namespace FileInterface
	{
		struct ComInit
		{
			ComInit() { CoInitialize(nullptr); }
			~ComInit() { CoUninitialize(); }
		};

		ComInit com;

		char* SaveSceneFile()
		{
			CComPtr<IFileSaveDialog> pFolderDlg;
			pFolderDlg.CoCreateInstance(CLSID_FileSaveDialog);

			// Extensions
			COMDLG_FILTERSPEC rgSpec[] =
			{
				{ L"Fusion Scene File", L"*.fsf" }
			};

			pFolderDlg->SetFileTypes(ARRAYSIZE(rgSpec), rgSpec);
			pFolderDlg->SetDefaultExtension(L"fsf");

			// Set options for a filesystem folder picker dialog.
			FILEOPENDIALOGOPTIONS opt{};
			pFolderDlg->GetOptions(&opt);
			pFolderDlg->SetOptions(opt | FOS_CREATEPROMPT | FOS_OVERWRITEPROMPT | FOS_FORCEFILESYSTEM);

			// Show the dialog modally.
			if (SUCCEEDED(pFolderDlg->Show(nullptr)))
			{
				// Get the path of the selected folder and output it to the console.

				CComPtr<IShellItem> pSelectedItem;
				pFolderDlg->GetResult(&pSelectedItem);

				CComHeapPtr<wchar_t> pPath;
				pSelectedItem->GetDisplayName(SIGDN_FILESYSPATH, &pPath);

				return wchar_to_char(pPath.m_pData);
			}
		}

		char* OpenSceneFile()
		{
			CComPtr<IFileOpenDialog> pFolderDlg;
			pFolderDlg.CoCreateInstance(CLSID_FileOpenDialog);

			// Extensions
			COMDLG_FILTERSPEC rgSpec[] =
			{
				{ L"Fusion Scene File", L"*.fsf" },
			};

			pFolderDlg->SetFileTypes(ARRAYSIZE(rgSpec), rgSpec);

			// Set options for a filesystem folder picker dialog.
			FILEOPENDIALOGOPTIONS opt{};
			pFolderDlg->GetOptions(&opt);
			pFolderDlg->SetOptions(opt | FOS_STRICTFILETYPES | FOS_PATHMUSTEXIST | FOS_FORCEFILESYSTEM);

			// Show the dialog modally.
			if (SUCCEEDED(pFolderDlg->Show(nullptr)))
			{
				// Get the path of the selected folder and output it to the console.

				CComPtr<IShellItem> pSelectedItem;
				pFolderDlg->GetResult(&pSelectedItem);

				CComHeapPtr<wchar_t> pPath;
				pSelectedItem->GetDisplayName(SIGDN_FILESYSPATH, &pPath);
				
				return wchar_to_char(pPath.m_pData);
			}
		}
	}

	namespace FileLoader
	{
		std::string GetFullFilePath(std::string filePath)
		{
			std::string toReplace = "res:";

			std::size_t pos = filePath.find(toReplace);
			if (pos == std::string::npos) return filePath;
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
			stream.close();

			if (fileContents.empty())
			{
				printf("[FILE LOADER][ERROR] Failed to load file from filepath: (%s)\n", FilePath.c_str());
			}

			File file;
			file.FilePath = FilePath;
			file.FileContents = fileContents;

			return file;
		}
	};

}