#pragma once

#include <vector>
#include <string>
#include "../Math/vec.h"
#include "tgaimage.h"

class GLFunctions;
class BaseShader;

class Model
{
private:
	std::vector<glm::Vec3f> verts_;
	std::vector<std::vector<glm::Vec3i> > faces_; // Vec3i 表示 vertex/uv/normal
	std::vector<glm::Vec3f> norms_;
	std::vector<glm::Vec2f> uv_;
	TGAImage diffusemap_;
	TGAImage normalmap_;
	TGAImage specularmap_;
	void load_texture(std::string filename, const char* suffix, TGAImage& img);
public:
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	glm::Vec3f normal(int iface, int nthvert);
	glm::Vec3f normal(glm::Vec2f uv);
	glm::Vec3f vert(int i);
	glm::Vec3f vert(int iface, int nthvert);
	glm::Vec2f uv(int iface, int nthvert);
	TGAColor diffuse(glm::Vec2f uv);
	float specular(glm::Vec2f uv);
	std::vector<int> face(int idx);
    void draw(std::shared_ptr<GLFunctions> gl, std::shared_ptr<BaseShader> shader);
};
