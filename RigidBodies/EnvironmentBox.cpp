//
// Created by Thuthuka on 2022/11/27.
//

#include "EnvironmentBox.h"
#include <map>

EnvironmentBox::EnvironmentBox(float width, float height) : width(width), height(height) {

}

std::vector<float> EnvironmentBox::getVertices() {
    std::vector<std::pair<float, float>> vectorSet = getUnusableVertices();
    std::vector<unsigned int> indexSet = getIndices();

    std::vector<float> vertices;
    for (int i = 0; i < indexSet.size(); i++) {
        vertices.push_back(vectorSet[indexSet[i]].first);
        vertices.push_back(vectorSet[indexSet[i]].second);
        vertices.push_back(0.0f);
    }

    return vertices;
}

std::vector<std::pair<float, float>> EnvironmentBox::getUnusableVertices() {
    std::vector<std::pair<float, float>> vertices;

    vertices.emplace_back(-0.5f * width, -0.5f * height);
    vertices.emplace_back(0.5f * width, -0.5f * height);
    vertices.emplace_back(0.5f * width, 0.5f * height);
    vertices.emplace_back(-0.5f * width, 0.5f * height);

    return vertices;
}

std::vector<unsigned int> EnvironmentBox::getIndices() {
    std::vector<unsigned int> indices;

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

    return indices;
}