//
// Created by Thuthuka on 2022/11/27.
//

#include "GameObject.h"

class EnvironmentBox : public GameObject {
private:
    float width, height;

    std::vector<std::pair<float, float>> getUnusableVertices();

    std::vector<unsigned int> getIndices();

public:
    EnvironmentBox(float width, float height);

    virtual std::vector<float> getVertices();
};