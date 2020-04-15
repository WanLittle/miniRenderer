#include "Texture2D.h"

#include <iostream>

#include "../../external/stb_image.h"

#define INV_SCALE 0.003921568627451 // 1/255.0f

Texture2D::~Texture2D()
{
    stbi_image_free(m_pixelBuffer);
    m_pixelBuffer = nullptr;
}

// TODO 支持透明度，单通道
glm::vec4f Texture2D::sample(const glm::vec2f &texcoord) const
{
    glm::vec4f result(0.0, 0.0, 0.0, 1.0);
    if (m_pixelBuffer == nullptr)
        return result;
    unsigned int x = 0, y = 0;

    double factorU = 0, factorV = 0;
    // 计算factorU factorV
    if (texcoord.x >= 0.0f && texcoord.x <= 1.0f 
        && texcoord.y >= 0.0f && texcoord.y <= 1.0f)
    {
        double trueU = texcoord.x * (m_width - 1);
        double trueV = texcoord.y * (m_height - 1);
        x = static_cast<unsigned int>(trueU);
        y = static_cast<unsigned int>(trueV);
        factorU = trueU - x;
        factorV = trueV - y;
    }
    else
    {
        // 重复
        float u = texcoord.x, v = texcoord.y;
        if (texcoord.x > 1.0f)
            u = texcoord.x - static_cast<int>(texcoord.x);
        else if (texcoord.x < 0.0f)
            u = 1.0f - (static_cast<int>(texcoord.x) - texcoord.x);
        if (texcoord.y > 1.0f)
            v = texcoord.y - static_cast<int>(texcoord.y);
        else if (texcoord.y < 0.0f)
            v = 1.0f - (static_cast<int>(texcoord.y) - texcoord.y);

        double trueU = u * (m_width - 1);
        double trueV = v * (m_height - 1);
        x = static_cast<unsigned int>(trueU);
        y = static_cast<unsigned int>(trueV);
        factorU = trueU - x;
        factorV = trueV - y;
    }
    
    // 获取texel
    glm::vec3f texels[4];
    int index[4];
    index[0] = (x * m_width + y) * m_channel;
    if (y + 1 >= m_height)
        index[1] = index[0];
    else
        index[1] = (x * m_width + y + 1) * m_channel;
    if (y + 1 >= m_height || x + 1 >= m_width)
        index[2] = index[0];
    else
        index[2] = ((x + 1) * m_width + y + 1) * m_channel;
    if (x + 1 >= m_width)
        index[3] = index[0];
    else
        index[3] = ((x + 1) * m_width + y) * m_channel;

    // 左下
    texels[0].x = static_cast<float>(m_pixelBuffer[index[0] + 0]) * INV_SCALE;
    texels[0].y = static_cast<float>(m_pixelBuffer[index[0] + 1]) * INV_SCALE;
    texels[0].z = static_cast<float>(m_pixelBuffer[index[0] + 2]) * INV_SCALE;

    // 左上
    texels[1].x = static_cast<float>(m_pixelBuffer[index[1] + 0]) * INV_SCALE;
    texels[1].y = static_cast<float>(m_pixelBuffer[index[1] + 1]) * INV_SCALE;
    texels[1].z = static_cast<float>(m_pixelBuffer[index[1] + 2]) * INV_SCALE;

    // 右上
    texels[2].x = static_cast<float>(m_pixelBuffer[index[2] + 0]) * INV_SCALE;
    texels[2].y = static_cast<float>(m_pixelBuffer[index[2] + 1]) * INV_SCALE;
    texels[2].z = static_cast<float>(m_pixelBuffer[index[2] + 2]) * INV_SCALE;

    // 右下
    texels[3].x = static_cast<float>(m_pixelBuffer[index[3] + 0]) * INV_SCALE;
    texels[3].y = static_cast<float>(m_pixelBuffer[index[3] + 1]) * INV_SCALE;
    texels[3].z = static_cast<float>(m_pixelBuffer[index[3] + 2]) * INV_SCALE;

    // 双线性插值
    // 先插值水平方向
    texels[0] = texels[0] * (1.0 - factorU) + texels[3] * factorU;
    texels[1] = texels[1] * (1.0 - factorU) + texels[2] * factorU;
    // 再插值垂直方向
    result = texels[0] * (1.0 - factorV) + texels[1] * factorV;

    return result;
}

bool Texture2D::loadImage(const std::string &path)
{
    if (m_pixelBuffer)
        delete m_pixelBuffer;
    m_pixelBuffer = nullptr;
    m_pixelBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_channel, 0);
    if (m_pixelBuffer == nullptr)
    {
        std::cout << "加载纹理失败，文件路径：" << path;
    }
    return  m_pixelBuffer != nullptr;
}

