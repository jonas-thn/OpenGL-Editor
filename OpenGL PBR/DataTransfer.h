#pragma once
#include <vector>

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
		distanceChanged = true;
    }

    void SetOrtho(bool isOrtho)
    {
		if (isOrtho == ortho) return;

        ortho = isOrtho;
		distanceChanged = true;
    }

    bool GetOrtho() const
    {
        return ortho;
    }

    DataTransfer(const DataTransfer&) = delete;
	DataTransfer& operator=(const DataTransfer&) = delete;
    
	bool distanceChanged = false;
	bool orthoisChanged = false;
private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    float distance = 2.0f;
    bool ortho = false;

    DataTransfer() = default;
};

