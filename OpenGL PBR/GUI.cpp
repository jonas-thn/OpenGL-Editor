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

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_TitleBgActive] = hightlightColor;

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

	imageData.uv0 = ImVec2(bottomLeft.x, 1.0f - topRight.y);
	imageData.uv1 = ImVec2(topRight.x, 1.0f - bottomLeft.y);

	imageData.aspectRatio = (topRight.x - bottomLeft.x) / (topRight.y - bottomLeft.y);
	imageData.textureWidth = height * imageData.aspectRatio;
	imageData.textureHeight = height;
}

void GUI::ViewportWindow(unsigned int fboTexture)
{
	ImGui::SetNextWindowSize(ImVec2(imageData.textureWidth, imageData.textureHeight), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	
	ImGui::Begin("OpenGL Viewport", nullptr, flags);

	ImGui::Image((void*)(intptr_t)fboTexture, ImVec2(imageData.textureWidth, imageData.textureHeight), imageData.uv0, imageData.uv1);

	ImGui::End();
	ImGui::PopStyleVar();
}

void GUI::SettingsWindow(Display& display)
{
	ImGui::SetNextWindowSize(ImVec2(display.GetWidth() - imageData.textureWidth, display.GetHeight() - windowSplit), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(imageData.textureWidth, 0), ImGuiCond_Once);

	ImGui::Begin("Object Settings", nullptr, flags);

	ImGui::End();
}

void GUI::PropertiesWindow(Display& display)
{
	ImGui::SetNextWindowSize(ImVec2(display.GetWidth() - imageData.textureWidth, windowSplit), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(imageData.textureWidth, display.GetHeight() - windowSplit), ImGuiCond_Once);

	ImGui::Begin("Properties", nullptr, flags);

	std::vector<float> vertices = DataTransfer::Instance().GetVertices();
	std::vector<unsigned int> indices = DataTransfer::Instance().GetIndices();

	if(vertices.empty() || indices.empty())
	{
		ImGui::Text("No object loaded.");
	}
	else
	{
		ImGui::Text("Vertex Data (%zu): ", vertices.size() / 5);

		for (int i = 0; i < vertices.size(); i++)
		{
			if(i % 5 != 0) ImGui::SameLine();
			ImGui::TextColored(textColor, "%.1f ", vertices[i]);
		}

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::Text("Indices (%zu):", indices.size());

		for(int i = 0; i < indices.size(); i++)
		{
			if(i != 0) ImGui::SameLine();
			ImGui::TextColored(textColor, "%u ", indices[i]);
		}
	}

	ImGui::End();
}

