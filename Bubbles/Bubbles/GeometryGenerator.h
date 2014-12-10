#ifndef __GEOMETRYGENARATOR__
#define __GEOMETRYGENARATOR__

#include <glm/glm.hpp>
#include <vector>

using namespace glm;

typedef unsigned int UINT;

struct Vertex
{
	Vertex(){}
	Vertex(const vec3& p, const vec3& n)
		: Position(p), Normal(n) {}
	Vertex(
		float px, float py, float pz,
		float nx, float ny, float nz)
		: Position(px, py, pz), Normal(nx, ny, nz) {}

	vec3 Position;
	vec3 Normal;
};

struct MeshData
{
	std::vector<Vertex> Vertices;
	std::vector<UINT> Indices;
};

void CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData& meshData);

void CreateAquarium(float width, float height, float depth, MeshData& meshData);

#endif