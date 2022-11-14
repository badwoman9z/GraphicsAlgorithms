#pragma once
#include <GLM/glm.hpp>
#include <vector>


class AABB 
{
public:
	AABB() = default;
	AABB(const glm::vec3& vMin, const glm::vec3& vMax);
	AABB(const std::vector<glm::vec3> vPoints);
	virtual ~AABB() = default;

	

	glm::vec3 getCentre() const;
	glm::vec3 getHalfSize() const;
	glm::vec3 getDiagonal() const;
	const glm::vec3& getMin() const { return m_Min; }
	const glm::vec3& getMax() const { return m_Max; }
	float getSurfaceArea() const;

	void combine(const AABB& vAnotherAABB);
	void combine(const glm::vec3& vPoint);
	bool overlaps(const AABB& vAnotherAABB);
	bool inside(const glm::vec3& vPoint);

private:
	glm::vec3 m_Min;
	glm::vec3 m_Max;
};