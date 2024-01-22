// DearImGui code for debugging
#pragma once
#include "FusionCore.h"

bool SceneMenuOpen = false;
bool editingObjectOpen = false;
bool editingModelOpen = false;
bool editingShaderOpen = false;
bool reloadShaders = false;
bool modalAgreed = false;
bool modalIgnored = false;
bool modalling = false;
bool newScene = false;
bool saveScene = false;
bool openScene = false;
bool chosenFile = false;
Fusion::Object* editingObject;
Fusion::Model* editingModel;
Fusion::Shader* editingShader;

const char* modalText = "";
const char* openFileLocation = "";

void ImGuiRightAlignText(const char* text)
{
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(text).x
        - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
}

namespace Fusion
{
	namespace Debug
	{
        void ResetData()
        {
            SceneMenuOpen = false;
            editingObjectOpen = false;
            editingModelOpen = false;
            editingShaderOpen = false;
            reloadShaders = false;
            newScene = false;
            saveScene = false;
            openScene = false;
        }

        void DrawNavbar()
        {
            if (ImGui::BeginMainMenuBar())
            {

                if (ImGui::BeginMenu("File"))
                {
                    //ImGui::SeparatorText("File");

                    if (ImGui::MenuItem("New Scene")) { newScene = true; }
                    if (ImGui::MenuItem("Open Scene")) { openScene = true; }
                    if (ImGui::MenuItem("Save Scene")) { saveScene = true; }
                    
                    ImGui::Separator();

                    if (ImGui::MenuItem("Exit", "ESC"))
                    {
                        printf("Application manually quit via Navbar\n");
                        glfwSetWindowShouldClose(Fusion::CurrentWindow, true);
                    }

                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Tools"))
                {
                    if (ImGui::MenuItem("Scene Menu")) { SceneMenuOpen = !SceneMenuOpen; }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Shaders"))
                {
                    if (ImGui::MenuItem("Shader Info")) {}
                    if (ImGui::MenuItem("Reload shaders")) { reloadShaders = true; }
                    ImGui::EndMenu();
                }

                std::string s_BuildType = std::string(FS_BUILD_TYPE);
                char* buildType = (char*)s_BuildType.c_str();
                char* RightInfoFormat = (char*)"Fusion Engine (Build: %s) (Frametime: %.3f ms (%.2f fps) )";
                char RightInfo[FS_MAX_CHAR_SIZE];
                sprintf(RightInfo, RightInfoFormat, buildType, Fusion::DeltaTime, 1 / Fusion::DeltaTime);

                ImGuiRightAlignText(RightInfo);
                ImGui::Text(RightInfo);

                ImGui::EndMainMenuBar();
            }
        }

        void DrawObjectEditor(Fusion::Scene* p_scene)
        {
            std::string objName = editingObject->GetName();
            std::string concat = ("Editing object \"" + objName + "\"###ObjectEditor");
            const char* title = concat.c_str();

            ImGui::Begin(title, &editingObjectOpen);
            ImGui::PushID(objName.c_str());

            // Show object mesh data
            uint32_t vertices = 0;
            uint32_t tris = 0;
            uint32_t meshes = 0;

            editingObject->GetObjectTopologyStats(vertices, tris, meshes);

            ImGui::Text("Meshes: %d", meshes);
            ImGui::Text("Vertices: %d", vertices);
            ImGui::Text("Triangles: %d", tris);
            ImGui::Text("Model Id: %d", editingObject->GetModel()->Id);

            ImGui::Separator();

            ImGui::DragFloat3("Position", glm::value_ptr(editingObject->position), 0.1f);
            ImGui::DragFloat3("Rotation", glm::value_ptr(editingObject->rotation), 0.1f);
            ImGui::DragFloat3("Scale", glm::value_ptr(editingObject->scale), 0.1f);

            ImGui::Checkbox("Draw Wireframe", &editingObject->drawWireframe);

            if (ImGui::Button("View model"))
            {
                editingModelOpen = true;
                editingModel = editingObject->GetModel();
            }

            if (ImGui::Button("Reload Object Shader"))
            {
                editingObject->GetModel()->ReloadShader();
            }

            ImGui::PopID();
            ImGui::End();
        }

        void DrawModelEditor()
        {
            std::string objName = editingModel->GetName();
            std::string concat = ("Viewing model \"" + objName + "\" (" + std::to_string(editingModel->Id) + ")###ModelEditor");
            const char* title = concat.c_str();

            ImGui::Begin(title, &editingModelOpen);

            // Show object mesh data
            uint32_t vertices = 0;
            uint32_t tris = 0;
            uint32_t meshes = 0;

            editingModel->GetObjectTopologyStats(vertices, tris, meshes);

            ImGui::Text("Meshes: %d", meshes);
            ImGui::Text("Vertices: %d", vertices);
            ImGui::Text("Triangles: %d", tris);

            ImGui::Separator();

            ImGui::Text("File: \"%s\"", editingModel->GetPath());
            ImGui::Text("Shader Id: %d", editingModel->GetShader()->getProgId());

            if (ImGui::Button("View Shader"))
            {
                editingShaderOpen = true;
                editingShader = editingModel->GetShader();
            }

            ImGui::End();
        }

        void DrawShaderEditor()
        {
            std::string progId = std::to_string(editingShader->getProgId());
            std::string concat = ("Viewing shader (" + progId + ")###ShaderEditor");
            const char* title = concat.c_str();

            ImGui::Begin(title, &editingShaderOpen);

            ImGui::Text("Vertex Shader: \"%s\"", editingShader->getVertPath());
            ImGui::Text("Fragment Shader: \"%s\"", editingShader->getFragPath());
            ImGui::Text("Is Active Shader: %s", ActiveShaderProg == editingShader->getProgId() ? "true" : "false");

            ImGui::End();
        }

		void DrawImGui(Fusion::Scene* p_scene)
		{         
            DrawNavbar();

            if (ImGui::BeginPopupModal("Are you sure?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text(modalText);
                ImGui::Separator();

                if (ImGui::Button("OK", ImVec2(120, 0))) { modalAgreed = true; ImGui::CloseCurrentPopup(); }
                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120, 0))) { modalIgnored = true; ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }

            if (reloadShaders)
            {
                reloadShaders = false;
                p_scene->ReloadShaders();
            }

            if (newScene)
            {
                bool ShouldModal = Serializer::ShouldPromptAgreement(p_scene);

                modalText = "Are you sure you want to open a new scene?\nAll your changes will be reverted.\nThis operation cannot be undone!";

                if (!modalling && ShouldModal)
                    ImGui::OpenPopup("Are you sure?");

                modalling = modalling || ShouldModal;

                if (modalAgreed || !ShouldModal)
                {
                    modalling = false;
                    newScene = false;
                    modalAgreed = false;
                    ResetData();
                    p_scene->ClearScene();

                    Serializer::LastSerializedScene = Serializer::SerializeSceneToConstCharP(p_scene);
                }
                else if (modalIgnored)
                {
                    modalling = false;
                    newScene = false;
                    modalIgnored = false;
                }
            }

            if (saveScene)
            {
                saveScene = false;
                char* filePath = FileInterface::SaveSceneFile();

                if ((filePath != NULL) && (filePath[0] == '\0') || (filePath == NULL)) {
                    return; // User exited dialog box
                }

                Serializer::SerializeSceneToXml(p_scene, filePath);
            }

            if (openScene)
            {
                if (!chosenFile)
                {
                    char* filePath = FileInterface::OpenSceneFile();

                    if ((filePath != NULL) && (filePath[0] == '\0') || (filePath == NULL)) {
                        openScene = false;
                        chosenFile = false;
                        return; // User exited dialog box
                    }

                    chosenFile = true;
                    openFileLocation = filePath;
                }

                bool ShouldModal = Serializer::ShouldPromptAgreement(p_scene);

                modalText = "Are you sure you want to open a scene file?\nAll your changes will be reverted.\nThis operation cannot be undone!";

                if (!modalling && ShouldModal)
                    ImGui::OpenPopup("Are you sure?");

                modalling = modalling || ShouldModal;

                if (modalAgreed || !ShouldModal)
                {
                    modalling = false;
                    modalAgreed = false;
                    openScene = false;
                    chosenFile = false;

                    ResetData();
                    Serializer::SceneFromXml(p_scene, openFileLocation);
                }
                else if (modalIgnored)
                {
                    modalling = false;
                    modalIgnored = false;
                    openScene = false;
                    chosenFile = false;
                }

            }

            uint32_t sceneVertices = 0;
            uint32_t sceneTriangles = 0;
            uint32_t sceneObjects = 0;
            uint32_t sceneModels = 0;
            uint32_t sceneMeshes = 0;

            p_scene->GetSceneTopologyStats(sceneVertices, sceneTriangles, sceneObjects, sceneModels, sceneMeshes);

            if (SceneMenuOpen)
            {
                ImGui::Begin("Fusion Scene Debug", &SceneMenuOpen);
                {
                    ImGui::Text("Scene Info:");
                    ImGui::Indent();
                    ImGui::Text("Frame Time: %.3f ms (%.2f fps)", Fusion::DeltaTime * 1000, 1 / Fusion::DeltaTime);
                    ImGui::Text("Objects in active scene: %d", sceneObjects);
                    ImGui::Text("Models in active scene: %d", sceneModels);
                    ImGui::Text("Meshes in active scene: %d", sceneMeshes);
                    ImGui::Text("Vertices in active scene: %d", sceneVertices);
                    ImGui::Text("Triangles in active scene: %d", sceneTriangles);

                    ImGui::Unindent();
                    ImGui::Separator();

                    if (ImGui::BeginTable("Scene Items TopText", 3, ImGuiTableFlags_BordersV))
                    {
                        ImGui::TableNextColumn(); ImGui::Text("Object Name");
                        ImGui::TableNextColumn(); ImGui::Text("Model Name");
                        ImGui::TableNextColumn(); ImGui::Text("Edit");
                        ImGui::EndTable();
                    }

                    if (ImGui::BeginTable("Scene Items", 3, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders))
                    {
                        for (uint32_t i = 0; i < (*p_scene->GetObjects()).size(); i++)
                        {
                            Object* object = (*p_scene->GetObjects())[i];

                            ImGui::PushID(i);
                            ImGui::TableNextColumn(); ImGui::Text(object->GetName());
                            ImGui::TableNextColumn(); ImGui::Text(object->GetModel()->GetName());
                            ImGui::TableNextColumn(); if (ImGui::Button("Edit")) { editingObject = object; editingObjectOpen = true; };
                            ImGui::PopID();
                        }
                        ImGui::EndTable();
                    }
                }
                ImGui::End();
            }
        
            if (editingObjectOpen)
                DrawObjectEditor(p_scene);
            if (editingModelOpen)
                DrawModelEditor();
            if (editingShaderOpen)
                DrawShaderEditor();
        }
	}
}