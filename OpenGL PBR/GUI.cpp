#include "GUI.h"

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

	//Viewport
	ImGui::SetNextWindowSize(ImVec2(imageData.textureWidth, imageData.textureHeight), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
	ImGui::Begin("OpenGL Viewport", nullptr, flags);

	ImGui::Image((void*)(intptr_t)fboTexture, ImVec2(imageData.textureWidth, imageData.textureHeight), imageData.uv0, imageData.uv1);

	ImGui::End();
	ImGui::PopStyleVar();

	//Object Settings
	ImGui::SetNextWindowSize(ImVec2(display.GetWidth() - imageData.textureWidth, display.GetHeight() - windowSplit), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(imageData.textureWidth, 0), ImGuiCond_Once);

	ImGui::Begin("Object Settings", nullptr, flags);

	ImGui::End();

	//Properties
	ImGui::SetNextWindowSize(ImVec2(display.GetWidth() - imageData.textureWidth, windowSplit), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(imageData.textureWidth, display.GetHeight() - windowSplit), ImGuiCond_Once);

	ImGui::Begin("Properties", nullptr, flags);

	ImGui::End();

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

