#pragma once
#include "../Math/vec.h"
#include "../Render/model.h"
class VertexIn
{
public:
    glm::vec4f cam_pos;   // Position
    glm::vec2f texcoord;   // Texture coordinate.
    glm::vec3f normal;     // Vertex normal.

    VertexIn() = default;
    VertexIn(glm::vec4f _pos, glm::vec2f _tex, glm::vec3f _normal)
        :cam_pos(_pos), texcoord(_tex), normal(_normal) {}
    VertexIn(const VertexIn &rhs)
        :cam_pos(rhs.cam_pos), texcoord(rhs.texcoord), normal(rhs.normal) {}
};

class VertexOut
{
public:
    glm::vec4f FragPos;          // ��������
    glm::vec2f TexCoords;        // ��������
    glm::vec3f Normal;	        // ���㷨��

    glm::vec4f gl_position;      // �൱�� GLSL �����ñ���
    glm::vec4f gl_FragCoord;     // �൱�� GLSL �����ñ���

    VertexOut() = default;
    VertexOut(glm::vec4f _posT, glm::vec4f _posH, glm::vec2f _tex, glm::vec3f _normal, glm::vec4f _color, double _oneDivZ)
        :FragPos(_posT), gl_position(_posH), TexCoords(_tex), Normal(_normal) {}
    VertexOut(const VertexOut& rhs) 
        :FragPos(rhs.FragPos), gl_position(rhs.gl_position), TexCoords(rhs.TexCoords), Normal(rhs.Normal) {}
};

class BaseShader
{
public:
    virtual ~BaseShader();
    virtual VertexOut vertex_shader(const VertexIn &in) = 0; // ������ɫ��
    virtual bool fragment_shader(const VertexOut &out, glm::vec4f& gl_FragColor, Model* m_model) = 0; // ������ɫ��
};
