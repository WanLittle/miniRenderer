#pragma once

#include "FrameBuffer.h"
#include "../Math/mat.h"
#include "../Shader/baseshader.h"

class Model;

struct Profile
{
public:
    unsigned int num_triangles;
    unsigned int num_vertices;
    Profile():num_triangles(0),num_vertices(0) {}
    void setZero(){num_triangles = num_vertices = 0;}
};

class GLFunctions
{
private:
    class Setting
    {
    public:
        bool m_depthTesting;                            // ��Ȳ��Կ���
        bool m_backFaceCulling;                         // �����޳�����

        Profile m_profile;                              // Record.
        int m_width, m_height;                          // �ӿڿ��
        std::shared_ptr<FrameBuffer> m_backBuffer;      // �󻺳���������д
        std::shared_ptr<FrameBuffer> m_frontBuffer;     // ǰ��������������ʾ

        std::shared_ptr<BaseShader> m_shader;
        Model* m_model;
        glm::Matrix viewport_mat;
        float n = 0.0, f = 1.0; // ��ȷ�Χ
    };

public:
    Setting m_config; // ����״̬

    void UseShader(std::shared_ptr<BaseShader> shader);
    void BindModel(Model* model);
public:
    GLFunctions(int width, int height);
    ~GLFunctions();

    void initialize();
    void beginFrame();
    void endFrame();

    void setDepthTesting(bool open){ m_config.m_depthTesting = open; }
    void setBackFaceCulling(bool open){ m_config.m_backFaceCulling = open; }

    void swapFrameBuffer();
    void clearFrameBuffer(float r, float g, float b, float a)
    {
        clearFrameBuffer(glm::vec4f(r, g, b, a));
    }
    void clearFrameBuffer(const glm::vec4f &color)
    { 
        m_config.m_backBuffer->clearColorAndDepthBuffer(color); 
    }

    unsigned char *getFrameResult(){ return m_config.m_frontBuffer->getColorBuffer(); }

    void DrawModel();
    void DepthRange(float _n, float _f);
    void Viewport(int x, int y, int w, int h);

private:
    void setDefaultConfig(); // Ĭ������

    //------��Ⱦ����------
    void triangle_barycentric(VertexOut &v1, VertexOut &v2, VertexOut &v3);
    void perspective_division(VertexOut& v);
    void viewport_transform(VertexOut& v);

};
