#include "GUI.h"
#include <vector>

#include "DataTransfer.h"

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

	//Distance
	static float dist = DataTransfer::Instance().GetDistance();
	if(ImGui::SliderFloat("Distance", &dist, 1.0f, 5.0f, "%.3f"))
	{
		DataTransfer::Instance().SetDistance(dist);
		DataTransfer::Instance().SetChanged(DISTANCE_CHANGED);
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	//Orho
	bool orthoTemp = DataTransfer::Instance().GetOrtho();
	static bool ortho = DataTransfer::Instance().GetOrtho();
	ImGui::Text("Enable Orthographic: ");
	ImGui::SameLine();
	ImGui::Checkbox("##switch", &ortho);
	
	if(ortho != orthoTemp)
	{
		DataTransfer::Instance().SetOrtho(ortho);
		DataTransfer::Instance().SetChanged(ORTHO_CHANGED);
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	//Object Selection
	const char* items[] = { "Triangle", "Quad", "Cube", "Sphere"};
	static int selectedIndex = 2;
	if (ImGui::Combo("Mesh", &selectedIndex, items, IM_ARRAYSIZE(items)))
	{
		switch (selectedIndex)
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
		}

		DataTransfer::Instance().SetChanged(MESH_CHANGED);
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	//ColorPicker
	static ImVec4 pickedColor(1.0f, 1.0f, 1.0f, 1.0f);

	if (ImGui::ColorPicker4("Color", (float*)&pickedColor))
	{
		DataTransfer::Instance().SetChanged(COLOR_CHANGED);
		DataTransfer::Instance().SetColor(pickedColor);
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

