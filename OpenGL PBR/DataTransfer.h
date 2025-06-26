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
		changed = true;
	}

    void UpdateIndices(const std::vector<unsigned int>& newIndices)
    {
        indices = newIndices;
		changed = true;
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
		changed = true;
    }

    DataTransfer(const DataTransfer&) = delete;
	DataTransfer& operator=(const DataTransfer&) = delete;
    
	bool changed = false;
private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    float distance = 2.0f;


    DataTransfer() = default;
};

