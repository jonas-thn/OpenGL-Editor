#include "GUI.h"
#include <vector>

#include "DataTransfer.h"
#include "ImGui/imgui_internal.h"

GUI::~GUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void GUI::Init()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init("#version 130");

	CalculateImageData();
}

void GUI::ProcessEvent(SDL_Event& event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void GUI::Render(unsigned int fboTexture, Display& display)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	SetStyle();

	ViewportWindow(fboTexture);
	
	SettingsWindow(display);

	PropertiesWindow(display);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::CalculateImageData()
{
	imageData.bottomLeft = bottomLeft;
	imageData.topRight = topRight;

	imageData.uv0 = ImVec2(bottomLeft.x, 1.0f - bottomLeft.y);
	imageData.uv1 = ImVec2(topRight.x, 1.0f - topRight.y);

	imageData.aspectRatio = (topRight.x - bottomLeft.x) / (topRight.y - bottomLeft.y);
	imageData.textureWidth = height * imageData.aspectRatio;
	imageData.textureHeight = height;
}

void GUI::SetStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_FrameBg] = grey02Color;
	style.Colors[ImGuiCol_FrameBgHovered] = grey04Color;
	style.Colors[ImGuiCol_FrameBgActive] = grey06Color;
	style.Colors[ImGuiCol_PopupBg] = grey02Color;
	style.Colors[ImGuiCol_Border] = grey04Color;
	style.Colors[ImGuiCol_BorderShadow] = blackColor;
	style.Colors[ImGuiCol_Header] = grey04Color;
	style.Colors[ImGuiCol_HeaderHovered] = grey06Color;
	style.Colors[ImGuiCol_HeaderActive] = grey08Color;
	style.Colors[ImGuiCol_Text] = whiteColor;
	style.Colors[ImGuiCol_CheckMark] = whiteColor;
	style.Colors[ImGuiCol_ScrollbarBg] = grey02Color;
	style.Colors[ImGuiCol_ScrollbarGrab] = grey04Color;
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = grey06Color;
	style.Colors[ImGuiCol_ScrollbarGrabActive] = grey08Color;
	style.Colors[ImGuiCol_Button] = grey04Color;
	style.Colors[ImGuiCol_SliderGrab] = grey08Color;
	style.Colors[ImGuiCol_SliderGrabActive] = whiteColor;
	style.Colors[ImGuiCol_ButtonHovered] = grey06Color;
	style.Colors[ImGuiCol_ButtonActive] = grey08Color;
	style.Colors[ImGuiCol_TextSelectedBg] = grey06Color;
	style.Colors[ImGuiCol_Separator] = grey04Color;
	style.Colors[ImGuiCol_SeparatorHovered] = grey06Color;
	style.Colors[ImGuiCol_SeparatorActive] = grey08Color;
	style.Colors[ImGuiCol_TitleBg] = grey02Color;
	style.Colors[ImGuiCol_TitleBgActive] = grey04Color;
	style.Colors[ImGuiCol_TitleBgCollapsed] = grey02Color;
	style.Colors[ImGuiCol_ResizeGrip] = grey04Color;
	style.Colors[ImGuiCol_ResizeGripHovered] = grey06Color;
	style.Colors[ImGuiCol_ResizeGripActive] = grey08Color;
	style.Colors[ImGuiCol_DragDropTarget] = grey08Color;
	style.Colors[ImGuiCol_NavHighlight] = grey06Color;
	style.Colors[ImGuiCol_NavWindowingHighlight] = grey06Color;
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0, 0, 0, 0.4f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0.4f);
}

void GUI::ViewportWindow(unsigned int fboTexture)
{
	ImGui::SetNextWindowSize(ImVec2(imageData.textureWidth, imageData.textureHeight), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	
	ImGui::Begin("OpenGL Viewport", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse);

	ImGui::Image((void*)(intptr_t)fboTexture, ImVec2(imageData.textureWidth, imageData.textureHeight), imageData.uv0, imageData.uv1);

	ImGui::End();

	ImGui::PopStyleVar();
}

void GUI::SettingsWindow(Display& display)
{
	ImGui::SetNextWindowSize(ImVec2(display.GetWidth() - imageData.textureWidth + 1, display.GetHeight() - windowSplit), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(imageData.textureWidth, 0), ImGuiCond_Once);

	ImGui::Begin("Object Settings", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

	//Object Selection
	const char* meshes[] = { "Triangle", "Quad", "Cube", "Sphere", "Ring", "Cone", "Cylinder", "Monkey" };
	static int selectedIndexMesh = 2;
	if (ImGui::Combo("Mesh", &selectedIndexMesh, meshes, IM_ARRAYSIZE(meshes)))
	{
		switch (selectedIndexMesh)
		{
		case 0:
			DataTransfer::Instance().SetMeshSelection(MeshSelection::Triangle);
			break;
		case 1:
			DataTransfer::Instance().SetMeshSelection(MeshSelection::Quad);
			break;
		case 2:
			DataTransfer::Instance().SetMeshSelection(MeshSelection::Cube);
			break;
		case 3:
			DataTransfer::Instance().SetMeshSelection(MeshSelection::Sphere);
			break;
		case 4:
			DataTransfer::Instance().SetMeshSelection(MeshSelection::Ring);
			break;
		case 5:
			DataTransfer::Instance().SetMeshSelection(MeshSelection::Cone);
			break;
		case 6:
			DataTransfer::Instance().SetMeshSelection(MeshSelection::Cylinder);
			break;
		case 7:
			DataTransfer::Instance().SetMeshSelection(MeshSelection::Monkey);
			break;
		}

		DataTransfer::Instance().SetChanged(MESH_CHANGED);
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	//Orho
	bool orthoTemp = DataTransfer::Instance().GetOrtho();
	static bool ortho = DataTransfer::Instance().GetOrtho();
	
	ImGui::Checkbox("Enable Orthographic", &ortho);
	
	if(ortho != orthoTemp)
	{
		DataTransfer::Instance().SetOrtho(ortho);
		DataTransfer::Instance().SetChanged(ORTHO_CHANGED);
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	//Skybox
	bool skyboxTemp = DataTransfer::Instance().GetSkybox();
	static bool skybox = DataTransfer::Instance().GetSkybox();

	ImGui::Checkbox("Enable Skybox", &skybox);

	if (skybox != skyboxTemp)
	{
		DataTransfer::Instance().SetSkybox(skybox);
		DataTransfer::Instance().SetChanged(SKYBOX_CHANGED);
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	//ColorPicker
	static float baseColor [3] { 1.0, 1.0, 1.0 };
	static bool pickerOpenColor = false;

	if(ImGui::Button("Base Color"))
	{
		pickerOpenColor = !pickerOpenColor;
	}

	ImGui::SameLine();
	ImVec2 p0 = ImGui::GetCursorScreenPos(); 
	ImVec2 p1 = ImVec2(p0.x + 40, p0.y + 19); 
	ImU32 color = IM_COL32((baseColor[0] * 255), (baseColor[1] * 255), (baseColor[2] * 255), 255);
	ImGui::GetWindowDrawList()->AddRectFilled(p0, p1, color);
	ImGui::NewLine();

	if (pickerOpenColor)
	{
		if (ImGui::ColorPicker3("Base Color", baseColor))
		{
			DataTransfer::Instance().SetChanged(COLOR_CHANGED);
			ImVec4 colorData(baseColor[0], baseColor[1], baseColor[2], 1.0);
			DataTransfer::Instance().SetColor(colorData);
		}
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	//Roughness
	static float rough = DataTransfer::Instance().GetRoughness();
	if (ImGui::SliderFloat("Roughness", &rough, 0.0f, 1.0f, "%.3f"))
	{
		DataTransfer::Instance().SetRoughness(rough);
		DataTransfer::Instance().SetChanged(ROUGHNESS_CHANGED);
	}

	//Emission Color
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	static float emissionColor[3]{ 0.0, 0.0, 0.0 };
	static bool pickerOpenEmission = false;

	if (ImGui::Button("Emission Color"))
	{
		pickerOpenEmission = !pickerOpenEmission;
	}

	ImGui::SameLine();
	p0 = ImGui::GetCursorScreenPos();
	p1 = ImVec2(p0.x + 40, p0.y + 19);
	color = IM_COL32((emissionColor[0] * 255), (emissionColor[1] * 255), (emissionColor[2] * 255), 255);
	ImGui::GetWindowDrawList()->AddRectFilled(p0, p1, color);
	ImGui::NewLine();

	if (pickerOpenEmission)
	{
		if (ImGui::ColorPicker3("Emission Color", emissionColor))
		{
			DataTransfer::Instance().SetChanged(EMISSION_COLOR_CHANGED);
			ImVec4 colorData(emissionColor[0], emissionColor[1], emissionColor[2], 1.0);
			DataTransfer::Instance().SetEmissionColor(colorData);
		}
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	static float rad = DataTransfer::Instance().GetEmissionRadius();
	if (ImGui::SliderFloat("Emission\nRadius", &rad, 3.0f, 20.0f, "%.3f"))
	{
		DataTransfer::Instance().SetEmissionRadius(rad);
		DataTransfer::Instance().SetChanged(EMISSION_RADIUS_CHANGED);
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	//Material Selection
	const char* materials[] = { "Brick", "Wood", "Conatiner" };
	static int selectedIndexMaterial = 0;
	if (ImGui::Combo("Material", &selectedIndexMaterial, materials, IM_ARRAYSIZE(materials)))
	{
		switch (selectedIndexMaterial)
		{
		case 0:
			DataTransfer::Instance().SetMaterialSelection(MaterialSelection::Brick);
			break;
		case 1:
			DataTransfer::Instance().SetMaterialSelection(MaterialSelection::Wood);
			break;
		case 2:
			DataTransfer::Instance().SetMaterialSelection(MaterialSelection::Container);
			break;
		}

		DataTransfer::Instance().SetChanged(MATERIAL_CHANGED);
	}

	ImGui::End();
}

void GUI::PropertiesWindow(Display& display)
{
	ImGui::SetNextWindowSize(ImVec2(display.GetWidth() - imageData.textureWidth + 1, windowSplit), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(imageData.textureWidth, display.GetHeight() - windowSplit), ImGuiCond_Once);

	ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

	std::vector<float> vertices = DataTransfer::Instance().GetVertices();
	std::vector<unsigned int> indices = DataTransfer::Instance().GetIndices();

	if(vertices.empty())
	{
		ImGui::Text("No object loaded");
	}
	else
	{
		ImGui::Text("Vertex Data (%zu): ", vertices.size() / 5);

		for (int i = 0; i < vertices.size(); i++)
		{
			if(i % 5 != 0) ImGui::SameLine();
			ImGui::TextColored(grey06Color, "%.1f ", vertices[i]);
		}

		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		if (indices.empty())
		{
			ImGui::Text("Indices (0): NULL");
		}
		else
		{
			ImGui::Text("Indices (%zu):", indices.size());

			for (int i = 0; i < indices.size(); i++)
			{
				if (i != 0) ImGui::SameLine();
				ImGui::TextColored(grey06Color, "%u ", indices[i]);
			}
		}		
	}

	ImGui::End();
}

