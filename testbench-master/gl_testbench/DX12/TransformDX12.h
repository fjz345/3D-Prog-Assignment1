#pragma once
#include "../Transform.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class TransformDX12 : public Transform
{
public:
	TransformDX12();
	~TransformDX12();
	void translate(float x, float y, float z);
	void rotate(float radians, float x, float y, float z);
	glm::mat4 transform;
};

