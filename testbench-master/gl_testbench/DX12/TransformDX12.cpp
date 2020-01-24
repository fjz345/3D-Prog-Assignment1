#include "TransformDX12.h"

TransformDX12::TransformDX12()
{
};


TransformDX12::~TransformDX12()
{
};

void TransformDX12::translate(float x, float y, float z)
{
	transform[3][0] += x;
	transform[3][1] += y;
	transform[3][2] += z;
};

void TransformDX12::rotate(float radians, float x, float y, float z)
{
	transform = glm::rotate(transform, radians, glm::vec3(x, y, z));
}

