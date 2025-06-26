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

    DataTransfer(const DataTransfer&) = delete;
	DataTransfer& operator=(const DataTransfer&) = delete;
    
private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    DataTransfer() = default;
};

