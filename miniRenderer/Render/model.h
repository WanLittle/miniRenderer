#pragma once

#include <vector>
#include <string>

#include "../Math/vec.h"
#include "texture2d.h"

class GLFunctions;
class BaseShader;

class Model
{
private:
	std::vector<glm::vec3f> verts_;
	std::vector<std::vector<glm::vec3i> > faces_; // Vec3i 表示 vertex/uv/normal
	std::vector<glm::vec3f> norms_;
	std::vector<glm::vec2f> uv_;
	Texture2D diffusemap_;
    Texture2D normalmap_;
    Texture2D specularmap_;
	void load_texture(std::string filename, const char* suffix, Texture2D& img);
public:
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	glm::vec3f normal(int iface, int nthvert);
	glm::vec3f normal(glm::vec2f uv);
	glm::vec3f vert(int i);
	glm::vec3f vert(int iface, int nthvert);
	glm::vec2f uv(int iface, int nthvert);
    glm::vec4f diffuse(glm::vec2f uv);
	float specular(glm::vec2f uv);
	std::vector<int> face(int idx);
    void draw(std::shared_ptr<GLFunctions> gl, std::shared_ptr<BaseShader> shader);
};
