#pragma once
#include "FusionCore.h"

#include "tinyxml2.h"

std::string Vec3toString(glm::vec3 vec)
{
	return std::to_string(vec.x) + "," + std::to_string(vec.y) + "," + std::to_string(vec.z);
}

glm::vec3 StringtoVec3(const char* str)
{
	float components[3];

	std::istringstream f(str);
	std::string s;

	unsigned int i = 0;

	while (std::getline(f, s, ','))
	{
		components[i] = std::stof(s);
		i++;
	}

	return glm::vec3(components[0], components[1], components[2]);
}

namespace Fusion
{
	namespace Serializer
	{

		std::string LastSerializedScene;

		tinyxml2::XMLDocument* SerializeSceneToDoc(Fusion::Scene* p_scene)
		{
			// gather all the unique shaders
			// gather all the unique models
			// gather all objects

			std::vector<Shader*> shaders;
			std::vector<Model*> models;
			std::vector<Object*>* objects = p_scene->GetObjects();

			p_scene->GetUniqueDescendants(&shaders, &models);

			tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument;

			// Add xml header
			tinyxml2::XMLDeclaration* header = doc->NewDeclaration();
			tinyxml2::XMLElement* Scene = doc->NewElement("Scene");

			Scene->SetAttribute("version", FS_FSF_VERSION); // Fusion Scene File Version

			doc->InsertFirstChild(header);
			doc->InsertAfterChild(header, Scene);

			for (unsigned int i = 0; i < objects->size(); i++)
			{
				Fusion::Object* object = (*objects)[i];

				tinyxml2::XMLElement* Object = doc->NewElement("Object");

				tinyxml2::XMLElement* name = doc->NewElement("name"); name->SetText(object->GetName());
				tinyxml2::XMLElement* position = doc->NewElement("position"); position->SetText(Vec3toString(object->position).c_str());
				tinyxml2::XMLElement* scale = doc->NewElement("scale"); scale->SetText(Vec3toString(object->scale).c_str());
				tinyxml2::XMLElement* rotation = doc->NewElement("rotation"); rotation->SetText(Vec3toString(object->rotation).c_str());
				tinyxml2::XMLElement* model = doc->NewElement("model"); model->SetText(std::to_string(object->GetModel()->Id).c_str());

				Object->InsertEndChild(name);
				Object->InsertEndChild(position);
				Object->InsertEndChild(scale);
				Object->InsertEndChild(rotation);
				Object->InsertEndChild(model);

				Scene->InsertEndChild(Object);
			}

			for (unsigned int i = 0; i < models.size(); i++)
			{
				Fusion::Model* model = models[i];

				tinyxml2::XMLElement* Model = doc->NewElement("Model");

				tinyxml2::XMLElement* name = doc->NewElement("name"); name->SetText(model->GetName());
				tinyxml2::XMLElement* file = doc->NewElement("file"); file->SetText(model->GetPath());
				tinyxml2::XMLElement* shader = doc->NewElement("shader"); shader->SetText(std::to_string(model->GetShader()->getProgId()).c_str());
				tinyxml2::XMLElement* id = doc->NewElement("id"); id->SetText(std::to_string(model->Id).c_str());

				Model->InsertEndChild(name);
				Model->InsertEndChild(file);
				Model->InsertEndChild(shader);
				Model->InsertEndChild(id);

				Scene->InsertEndChild(Model);
			}

			for (unsigned int i = 0; i < shaders.size(); i++)
			{
				Fusion::Shader* shader = shaders[i];

				tinyxml2::XMLElement* Shader = doc->NewElement("Shader");

				tinyxml2::XMLElement* id = doc->NewElement("id"); id->SetText(std::to_string(shader->getProgId()).c_str());
				tinyxml2::XMLElement* vert = doc->NewElement("vert"); vert->SetText(shader->getVertPath());
				tinyxml2::XMLElement* frag = doc->NewElement("frag"); frag->SetText(shader->getFragPath());

				Shader->InsertEndChild(id);
				Shader->InsertEndChild(vert);
				Shader->InsertEndChild(frag);

				Scene->InsertEndChild(Shader);
			}

			return doc;
		}

		// Return value must be deleted
		const char* SerializeSceneToConstCharP(Scene* p_scene)
		{
			tinyxml2::XMLDocument* doc = SerializeSceneToDoc(p_scene);
			tinyxml2::XMLPrinter* printer = new tinyxml2::XMLPrinter;

			doc->Print(printer);

			const char* out = printer->CStr();

			std::string cout(out);
			char* cstr = new char[cout.length() + 1];
			std::strcpy(cstr, cout.c_str());

			delete doc;

			return cstr;
		}

		void SerializeSceneToXml(Scene* p_scene, const char* filePath)
		{
			tinyxml2::XMLDocument* doc = SerializeSceneToDoc(p_scene);

			LastSerializedScene = SerializeSceneToConstCharP(p_scene);

			doc->SaveFile(filePath);

			delete doc;
		}
	
		void SceneFromXml(Scene* p_scene, const char* filePath)
		{
			File xmlFile = FileLoader::GetFile(filePath);
			std::string xmlSource = xmlFile.FileContents;
			
			tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument;
			doc->Parse(xmlSource.c_str());

			if (doc->Error())
			{
				printf("[FUSION SCENE SERIALIZATION][ERROR] Failed to deserialize scene file at location (%s)\n", filePath);
				printf("%s\n", doc->ErrorStr());
				return;
			}

			tinyxml2::XMLElement* Scene = doc->FirstChildElement();

			if (!Scene)
			{
				printf("[FUSION SCENE SERIALIZATION][ERROR] Fusion Scene File doesnt contain a scene (%s)\n", filePath);
				return;
			}

			unsigned int fileVersion = std::stoi(Scene->Attribute("version"));

			if (fileVersion < FS_SUPPORTED_FSF_VERSION)
			{
				printf("[FUSION SCENE SERIALIZATION][ERROR] This file version (%d) is no longer supported. Supported version: >= %d\n", fileVersion, FS_SUPPORTED_FSF_VERSION);
				return;
			}

			// Clear out scene
			Fusion::SceneReady = false;

			printf("[FUSION SCENE SERIALIZATION] Clearing scene\n");
			p_scene->ClearScene();

			printf("[FUSION SCENE SERIALIZATION] Loading scene file\n");
			std::map<unsigned int, Fusion::Shader*> shaderMap;
			std::map<unsigned int, Fusion::Model*> modelMap;

			printf("[FUSION SCENE SERIALIZATION] Loading shaders\n");
			for (const tinyxml2::XMLElement* shader = Scene->FirstChildElement("Shader"); shader; shader = shader->NextSiblingElement("Shader"))
			{
				unsigned int id = std::stoi(shader->FirstChildElement("id")->GetText());
				const char* vert = shader->FirstChildElement("vert")->GetText();
				const char* frag = shader->FirstChildElement("frag")->GetText();

				std::string Vert(vert);
				char* vstr = new char[Vert.length() + 1];
				std::strcpy(vstr, Vert.c_str());

				std::string Frag(frag);
				char* fstr = new char[Frag.length() + 1];
				std::strcpy(fstr, Frag.c_str());

				Fusion::Shader* fShader = new Fusion::Shader(vstr, fstr);

				printf("[FUSION SCENE SERIALIZATION] Loaded shader (id: %d) (vert: %s) (frag: %s)\n", id, vert, frag);

				shaderMap[id] = fShader;
			}

			printf("[FUSION SCENE SERIALIZATION] Loading models\n");
			for (const tinyxml2::XMLElement* model = Scene->FirstChildElement("Model"); model; model = model->NextSiblingElement("Model"))
			{
				unsigned int id = std::stoi(model->FirstChildElement("id")->GetText());
				unsigned int shaderId = std::stoi(model->FirstChildElement("shader")->GetText());
				const char* name = model->FirstChildElement("name")->GetText();
				const char* file = model->FirstChildElement("file")->GetText();

				Fusion::Shader* fShader = shaderMap[shaderId];

				assert(fShader, "Either shader.id is wrong or model.shader is wrong");

				std::string Name(name);
				char* nstr = new char[Name.length() + 1];
				std::strcpy(nstr, Name.c_str());

				std::string File(file);
				char* fstr = new char[File.length() + 1];
				std::strcpy(fstr, File.c_str());

				Fusion::Model* fModel = new Fusion::Model(fstr, nstr, fShader);

				printf("[FUSION SCENE SERIALIZATION] Loaded model (name: %s) (file: %s) (id: %d) (shader: %d)\n", name, file, id, shaderId);

				modelMap[id] = fModel;
			}

			printf("[FUSION SCENE SERIALIZATION] Loading objects\n");
			for (const tinyxml2::XMLElement* object = Scene->FirstChildElement("Object"); object; object = object->NextSiblingElement("Object"))
			{
				unsigned int model = std::stoi(object->FirstChildElement("model")->GetText());
				const char* name = object->FirstChildElement("name")->GetText();
				const char* position = object->FirstChildElement("position")->GetText();
				const char* rotation = object->FirstChildElement("rotation")->GetText();
				const char* scale = object->FirstChildElement("scale")->GetText();

				glm::vec3 vposition = StringtoVec3(position);
				glm::vec3 vrotation = StringtoVec3(rotation);
				glm::vec3 vscale = StringtoVec3(scale);

				Fusion::Model* fModel = modelMap[model];

				assert(fModel, "Either model.id is wrong or object.model is wrong");

				std::string Name(name);
				char* nstr = new char[Name.length() + 1];
				std::strcpy(nstr, Name.c_str());

				Fusion::Object* fObject = new Fusion::Object(fModel, nstr);
				fObject->position = vposition;
				fObject->rotation = vrotation;
				fObject->scale = vscale;

				printf("[FUSION SCENE SERIALIZATION] Loaded object (name: %s) (model: %d)\n", name, model);

				p_scene->GetObjects()->push_back(fObject);
			}

			Fusion::SceneReady = true;

			LastSerializedScene = SerializeSceneToConstCharP(p_scene);

			delete doc;
		}

		bool ShouldPromptAgreement(Fusion::Scene* p_scene)
		{
			if (LastSerializedScene == SerializeSceneToConstCharP(p_scene))
			{
				return false;
			}

			return true;
		}
	}
}