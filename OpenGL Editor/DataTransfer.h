#pragma once
#include <vector>

#include "ImGui/imgui.h"

enum ChangedFlags : uint32_t
{
    NONE_CHANGED = 0,
    ORTHO_CHANGED = 1 << 0,
    MESH_CHANGED = 1 << 1,
    COLOR_CHANGED = 1 << 2,
    SKYBOX_CHANGED = 1 << 3,
    ROUGHNESS_CHANGED = 1 << 4,
    EMISSION_COLOR_CHANGED = 1 << 5,
    EMISSION_RADIUS_CHANGED = 1 << 6,
    MATERIAL_CHANGED = 1 << 7
};

enum class MeshSelection
{
    Triangle,
    Quad,
    Cube,
    Sphere,
    Monkey,
    Ring,
    Cone,
    Cylinder,
    Pyramid
};

enum class MaterialSelection
{
    None,
    Brick,
    Stone,
    Tiles,
    Wood
};

class DataTransfer 
{
public:
    static DataTransfer& Instance() 
    {
        static DataTransfer instance;
        return instance;
    }

    void UpdateVertices(const std::vector<float>& newVertices) 
    {
        vertices = newVertices;
	}

    void UpdateIndices(const std::vector<unsigned int>& newIndices)
    {
        indices = newIndices;
    }

    const std::vector<float>& GetVertices() const 
    {
        return vertices;
	}

    const std::vector<unsigned int>& GetIndices() const 
    {
        return indices;
    }

    void SetOrtho(bool isOrtho)
    {
		if (isOrtho == ortho) return;

        ortho = isOrtho;
		SetChanged(ChangedFlags::ORTHO_CHANGED);
    }

    bool GetOrtho() const
    {
        return ortho;
    }

    MeshSelection GetMeshSelection() const
    {
        return meshSelection;
	}

    void SetMeshSelection(MeshSelection selection)
    {
        if (selection == meshSelection) return;
        meshSelection = selection;
    }

    MaterialSelection GetMaterialSelection() const
    {
        return materialSelection;
    }

    void SetMaterialSelection(MaterialSelection selection)
    {
        if (selection == materialSelection) return;
        materialSelection = selection;
    }

    ImVec4 GetColor()
    {
        return selectedColor;
    }

    void SetColor(ImVec4 color)
    {
        selectedColor = color;
    }

    bool GetSkybox() const
    {
        return skybox;
    }

    void SetSkybox(bool newSkybox)
    {
        skybox = newSkybox;
    }

    float GetRoughness()
    {
        return roughness;
    }

    void SetRoughness(float newRoughness)
    {
        roughness = newRoughness;
    }

    ImVec4 GetEmissionColor() const
    {
        return emissionColor;
    }

    void SetEmissionColor(ImVec4 color)
    {
        emissionColor = color;
    }

    float GetEmissionRadius() const
    {
        return emissionRadius;
    }

    void SetEmissionRadius(float newRadius)
    {
        emissionRadius = newRadius;
    }
    
    void SetChanged(ChangedFlags flag)
    {
        changedFlags |= flag;
    }

    void ClearChanged(ChangedFlags flag)
    {
        changedFlags &= ~flag;
    }

    bool HasChanged(ChangedFlags flag) const
    {
        return (changedFlags & flag) != 0;
    }

    void ClearAll()
    {
        changedFlags = ChangedFlags::NONE_CHANGED;
    }

    DataTransfer(const DataTransfer&) = delete;
	DataTransfer& operator=(const DataTransfer&) = delete;
    
	uint32_t changedFlags = NONE_CHANGED;

private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    bool ortho = false;
    MeshSelection meshSelection = MeshSelection::Cube;
    ImVec4 selectedColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    bool skybox = false;
    float roughness = 0.0f;
    ImVec4 emissionColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    float emissionRadius = 10;
    MaterialSelection materialSelection = MaterialSelection::None;

    DataTransfer() = default;
};

