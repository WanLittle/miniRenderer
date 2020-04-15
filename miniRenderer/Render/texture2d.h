#pragma once

#include <string>
#include "../Math/vec.h"

class Texture2D
{
private:
    int m_width;                        // 宽
    int m_height;                       // 高
    int m_channel;                      // 颜色通道的个数
    unsigned char *m_pixelBuffer;       // 数据

public:
    Texture2D() :m_width(0), m_height(0), m_channel(0), m_pixelBuffer(nullptr) {}
    ~Texture2D();

    bool loadImage(const std::string &path);

    glm::vec4f sample(const glm::vec2f &texcoord) const;

};