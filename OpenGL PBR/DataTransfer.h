#pragma once
#include <vector>

#include "ImGui/imgui.h"

enum ChangedFlags : uint32_t
{
    NONE_CHANGED = 0,
    DISTANCE_CHANGED = 1 << 0,
    ORTHO_CHANGED = 1 << 1,
    MESH_CHANGED = 1 << 2,
    COLOR_CHANGED = 1 << 3
};

enum class MeshSelection
{
    Triangle,
    Quad,
    Cube,
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

    float GetDistance() const 
    {
        return distance;
	}

    void SetDistance(float newDistance)
    {
        distance = newDistance;
		SetChanged(ChangedFlags::DISTANCE_CHANGED);
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

    ImVec4 GetColor()
    {
        return selectedColor;
    }

    void SetColor(ImVec4 color)
    {
        selectedColor = color;
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
    float distance = 2.0f;
    bool ortho = false;
    MeshSelection meshSelection = MeshSelection::Cube;
    ImVec4 selectedColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    DataTransfer() = default;
};

