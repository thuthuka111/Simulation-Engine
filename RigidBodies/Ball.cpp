//
// Created by Thuthuka on 2022/12/05.
//

#include "Ball.h"
#include <glm/ext/scalar_constants.hpp>

Ball::Ball(float radius) : radius(radius) {}

std::vector<float> Ball::getVertices() {
    std::vector<std::pair<float, float>> vectorSet = getUnusableVertices();
    std::vector<unsigned int> indexSet = getIndices();

    std::vector<float> sphereVerts;
    for (int i = 0; i < indexSet.size(); i++) {
        sphereVerts.push_back(vectorSet[indexSet[i]].first);
        sphereVerts.push_back(vectorSet[indexSet[i]].second);
        sphereVerts.push_back(0.0f);
    }

    return sphereVerts;
}

std::vector<std::pair<float, float>> Ball::getUnusableVertices() {
    std::vector<std::pair<float, float>> sphereVerts;
    sphereVerts.emplace_back(0.0f, 0.0f);
    for (int i = 0; i < this->vertices; i++) {
        sphereVerts.emplace_back(
                round((cos(((2 * glm::pi<float>()) / this->vertices) * i) * radius) * 1000000) / 1000000,
                round(sin(((2 * glm::pi<float>()) / this->vertices) * i) * radius * 1000000) / 1000000);

    }

    return sphereVerts;
}

std::vector<unsigned int> Ball::getIndices() {
    std::vector<unsigned int> indices;

    for (int i = 0; i < this->vertices; i++) {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back((i + 2) > this->vertices ? (i + 2) % this->vertices : (i + 2));
    }

    return indices;
}
