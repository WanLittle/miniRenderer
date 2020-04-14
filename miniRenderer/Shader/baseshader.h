#pragma once
#include "../Math/vec.h"
#include "../Render/model.h"
class VertexIn
{
public:
    glm::Vec4f cam_pos;   // Position
    glm::Vec2f texcoord;   // Texture coordinate.
    glm::Vec3f normal;     // Vertex normal.

    VertexIn() = default;
    VertexIn(glm::Vec4f _pos, glm::Vec2f _tex, glm::Vec3f _normal)
        :cam_pos(_pos), texcoord(_tex), normal(_normal) {}
    VertexIn(const VertexIn &rhs)
        :cam_pos(rhs.cam_pos), texcoord(rhs.texcoord), normal(rhs.normal) {}
};

class VertexOut
{
public:
    glm::Vec4f FragPos;          // 顶点坐标
    glm::Vec2f TexCoords;        // 纹理坐标
    glm::Vec3f Normal;	        // 顶点法线

    glm::Vec4f gl_position;      // 相当于 GLSL 的内置变量
    glm::Vec4f gl_FragCoord;     // 相当于 GLSL 的内置变量

    VertexOut() = default;
    VertexOut(glm::Vec4f _posT, glm::Vec4f _posH, glm::Vec2f _tex, glm::Vec3f _normal, glm::Vec4f _color, double _oneDivZ)
        :FragPos(_posT), gl_position(_posH), TexCoords(_tex), Normal(_normal) {}
    VertexOut(const VertexOut& rhs) 
        :FragPos(rhs.FragPos), gl_position(rhs.gl_position), TexCoords(rhs.TexCoords), Normal(rhs.Normal) {}
};

class BaseShader
{
public:
    virtual ~BaseShader();
    virtual VertexOut vertex_shader(const VertexIn &in) = 0; // 顶点着色器
    virtual bool fragment_shader(const VertexOut &out, glm::Vec4f& gl_FragColor, Model* m_model) = 0; // 像素着色器
};
