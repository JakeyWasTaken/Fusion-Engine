// DearImGui code for debugging
#pragma once
#include "FusionCore.h"

bool SceneMenuOpen = false;
bool editingObjectOpen = false;
bool reloadShaders = false;
Fusion::Object* editingObject;

namespace Fusion
{
	namespace Debug
	{
        void DrawNavbar()
        {
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
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
                    if (ImGui::MenuItem("Reload shaders")) { reloadShaders = true; }
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }
        }

        void ReloadShaders(Fusion::Scene scene)
        {
            reloadShaders = false;

            for (uint32_t i = 0; i < scene.GetObjects().size(); i++)
            {
                Object* object = scene.GetObjects()[i];
                object->GetModel().ReloadShader();
            }
        }

		void DrawImGui(Fusion::Scene scene)
		{         
            DrawNavbar();

            if (reloadShaders)
                ReloadShaders(scene);


            uint32_t sceneVertices = 0;
            uint32_t sceneTriangles = 0;
            uint32_t sceneObjects = 0;
            uint32_t sceneModels = 0;
            uint32_t sceneMeshes = 0;

            scene.GetSceneTopologyStats(sceneVertices, sceneTriangles, sceneObjects, sceneModels, sceneMeshes);

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
                        for (uint32_t i = 0; i < scene.GetObjects().size(); i++)
                        {
                            Object* object = scene.GetObjects()[i];

                            ImGui::PushID(i);
                            ImGui::TableNextColumn(); ImGui::Text(object->GetName());
                            ImGui::TableNextColumn(); ImGui::Text(object->GetModel().GetName());
                            ImGui::TableNextColumn(); if (ImGui::Button("Edit")) { editingObject = object; editingObjectOpen = true; };
                            ImGui::PopID();
                        }
                        ImGui::EndTable();
                    }
                }
                ImGui::End();
            }
        
            if (editingObjectOpen)
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

                    ImGui::Separator();

                    ImGui::DragFloat3("Position", glm::value_ptr(editingObject->position), 0.1f);
                    ImGui::DragFloat3("Rotation", glm::value_ptr(editingObject->rotation), 0.1f);
                    ImGui::DragFloat3("Scale", glm::value_ptr(editingObject->scale), 0.1f);

                    ImGui::Checkbox("Draw Wireframe", &editingObject->drawWireframe);

                    ImGui::PopID();
                ImGui::End();
            }
        }
	}
}