#include "Pipeline.h"

#include <algorithm>

#include "render.h"
#include "../Math/transform.h"
#include "model.h"


void GLFunctions::UseShader(std::shared_ptr<BaseShader> shader)
{
    m_config.m_shader = shader;
}

void GLFunctions::BindModel(Model* model)
{
    m_config.m_model = model;
}

GLFunctions::GLFunctions(int width, int height)
{
    m_config.m_width = width;
    m_config.m_height = height;
    m_config.m_frontBuffer = nullptr;
    m_config.m_backBuffer = nullptr;
}

GLFunctions::~GLFunctions()
{
}

void GLFunctions::initialize()
{
    m_config.m_backBuffer = std::make_shared<FrameBuffer>(m_config.m_width, m_config.m_height);
    m_config.m_frontBuffer = std::make_shared<FrameBuffer>(m_config.m_width, m_config.m_height);
    setDefaultConfig();
}

void GLFunctions::beginFrame()
{
    m_config.m_profile.setZero();
}

void GLFunctions::endFrame() {}


void GLFunctions::setDefaultConfig()
{
    setDepthTesting(true);
    setBackFaceCulling(true);
    DepthRange(0.0f, 1.0f);
}

glm::Vec3f barycentric(glm::Vec2f A, glm::Vec2f B, glm::Vec2f C, glm::Vec2f P)
{
    glm::Vec3f s[2];
    for (int i = 2; i--; )
    {
        s[i][0] = C[i] - A[i];
        s[i][1] = B[i] - A[i];
        s[i][2] = A[i] - P[i];
    }
    glm::Vec3f u = cross(s[0], s[1]);
    // TODO：为什么这种情况是退化三角形
    if (std::abs(u[2]) > 1e-2) //  u[2] is integer. 如果为0 则三角形ABC是退化的degenerate
    {
        return glm::Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    }
    return glm::Vec3f(-1, 1, 1); // 丢弃退化三角形
}


void GLFunctions::triangle_barycentric(VertexOut &v1, VertexOut &v2, VertexOut &v3)
{
    std::shared_ptr<FrameBuffer> framebuffer = m_config.m_backBuffer;

    glm::Vec2f vertex_fragment_coord[3];
    vertex_fragment_coord[0] = glm::proj<2>(v1.gl_position);
    vertex_fragment_coord[1] = glm::proj<2>(v2.gl_position);
    vertex_fragment_coord[2] = glm::proj<2>(v3.gl_position);

    glm::Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    glm::Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    glm::Vec2f clamp(framebuffer->getWidth() - 1, framebuffer->getHeight() - 1);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            bboxmin[j] = std::max(0.f, std::min(bboxmin[j], vertex_fragment_coord[i][j]));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], vertex_fragment_coord[i][j]));
        }
    }

    glm::Vec2i P;
    glm::Vec4f color;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; ++P.x)
    {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; ++P.y)
        {
            // 屏幕空间的质心坐标，用于插值顶点深度
            glm::Vec3f bary_coor_screen = barycentric(vertex_fragment_coord[0], vertex_fragment_coord[1], vertex_fragment_coord[2], P);

            // 透视校正插值，用于插值顶点属性
            glm::Vec3f perspective_correction = glm::Vec3f(
                bary_coor_screen.x / v1.gl_position.w, 
                bary_coor_screen.y / v2.gl_position.w,
                bary_coor_screen.z / v3.gl_position.w
            );
            perspective_correction = perspective_correction / (perspective_correction.x + perspective_correction.y + perspective_correction.z);

            // 顶点深度插值
            float frag_depth = v1.gl_position.z * perspective_correction[0] 
                + v2.gl_position.z * perspective_correction[1] + v3.gl_position.z * perspective_correction[2];
            
            if (bary_coor_screen.x < 0 || bary_coor_screen.y < 0 || bary_coor_screen.z < 0) // 像素不在三角形内
                continue;

            if (framebuffer->getDepth(P.x, P.y) > frag_depth) // 不通过深度测试
                continue;

            VertexOut frag;
            float w1 = perspective_correction.x, w2 = perspective_correction.y, w3 = perspective_correction.z;
            frag.FragPos = v1.FragPos * w1 + v2.FragPos * w2 + v3.FragPos * w3;
            frag.Normal = v1.Normal * w1 + v2.Normal * w2 + v3.Normal * w3;
            frag.TexCoords = v1.TexCoords * w1 + v2.TexCoords * w2 + v3.TexCoords * w3;
            frag.gl_FragCoord = glm::Vec4f(P.x, P.y, frag_depth, 1.0); // TODO 计算w

            bool discard = m_config.m_shader->fragment_shader(frag, color, m_config.m_model);
            if (!discard)
            {
                framebuffer->drawDepth(P.x, P.y, frag_depth);
                framebuffer->drawPixel(P.x, P.y, color);
            }
        }
    }
}

void GLFunctions::perspective_division(VertexOut& v)
{
    v.gl_position.x /= v.gl_position.w;
    v.gl_position.y /= v.gl_position.w;
    v.gl_position.z /= v.gl_position.w;
    //assert(v.gl_position.x < 1.0f && v.gl_position.x);
    //assert(v.gl_position.y < 1.0f && v.gl_position.y);
    //assert(v.gl_position.z < 1.0f && v.gl_position.z);
    // w 不变，用于透视校正插值
}

void GLFunctions::viewport_transform(VertexOut& v)
{
    v.gl_position = m_config.viewport_mat * v.gl_position;
}

void GLFunctions::DrawModel()
{
    std::shared_ptr<BaseShader> shader = m_config.m_shader;
    Model* model = m_config.m_model;
    for (int i = 0; i < model->nfaces(); ++i)
    {
        VertexIn p1(glm::embed<4, 3, float>(model->vert(i, 0), 1.0f), model->uv(i, 0), model->normal(i, 0));
        VertexIn p2(glm::embed<4, 3, float>(model->vert(i, 1), 1.0f), model->uv(i, 1), model->normal(i, 1));
        VertexIn p3(glm::embed<4, 3, float>(model->vert(i, 2), 1.0f), model->uv(i, 2), model->normal(i, 2));

        // 顶点着色器
        VertexOut v1 = shader->vertex_shader(p1);
        VertexOut v2 = shader->vertex_shader(p2);
        VertexOut v3 = shader->vertex_shader(p3);

        // TODO 视锥剔除

        // TODO 背面剔除

        // 透视除法
        perspective_division(v1);
        perspective_division(v2);
        perspective_division(v3);

        // 视口变换
        viewport_transform(v1);
        viewport_transform(v2);
        viewport_transform(v3);


        // 光栅化 + 片段着色器
        triangle_barycentric(v1, v2, v3);

    }
}

void GLFunctions::swapFrameBuffer()
{
    std::swap(m_config.m_backBuffer, m_config.m_frontBuffer);
}

void GLFunctions::DepthRange(float _n, float _f)
{
    m_config.n = _n;
    m_config.f = _f;
}

void GLFunctions::Viewport(int x, int y, int w, int h)
{
    m_config.viewport_mat = glm::viewport( x, y, w, h, m_config.n, m_config.f);
}
