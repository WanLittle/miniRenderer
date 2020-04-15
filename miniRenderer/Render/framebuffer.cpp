#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(int width, int height)
    :m_channel(4), m_width(width), m_height(height)
{
    m_depthBuffer.resize(m_width * m_height, 1.0f);
    m_colorBuffer.resize(m_width * m_height * m_channel, 255);
}

double FrameBuffer::getDepth(unsigned int x, unsigned int y) const
{
    if(x < 0 || x >= m_width || y < 0 || y >= m_height)
        return 0.0f;
    return m_depthBuffer[y * m_width + x];
}

void FrameBuffer::clearColorAndDepthBuffer(const glm::vec4f &color)
{
    auto red = static_cast<unsigned char>(255 * color[0]);
    auto green = static_cast<unsigned char>(255 * color[1]);
    auto blue = static_cast<unsigned char>(255 * color[2]);
    auto alpha = static_cast<unsigned char>(255 * color[3]);

    for(unsigned int row = 0; row < m_height; ++row)
    {
        for(unsigned int col = 0; col < m_width; ++col)
        {
            m_depthBuffer[row * m_width + col] = 1.0f;
            m_colorBuffer[row * m_width * m_channel + col * m_channel + 0] = red;
            m_colorBuffer[row * m_width * m_channel + col * m_channel + 1] = green;
            m_colorBuffer[row * m_width * m_channel + col * m_channel + 2] = blue;
            m_colorBuffer[row * m_width * m_channel + col * m_channel + 3] = alpha;
        }
    }
}

void FrameBuffer::drawDepth(unsigned int x, unsigned int y, double value)
{
    if(x < 0 || x >= m_width || y < 0 || y >= m_height)
        return;
    unsigned int index = y * m_width + x;
    m_depthBuffer[index] = value;
}

void FrameBuffer::drawPixel(unsigned int x, unsigned int y, const glm::vec4f &color)
{
    if(x < 0 || x >= m_width || y < 0 || y >= m_height)
        return;
    unsigned int index = y * m_width * m_channel + x * m_channel;
    m_colorBuffer[index + 0] = static_cast<unsigned char>(255 * pow(color[0], 1.0 / 2.2)); // Gamma Ð£Õý
    m_colorBuffer[index + 1] = static_cast<unsigned char>(255 * pow(color[1], 1.0 / 2.2));
    m_colorBuffer[index + 2] = static_cast<unsigned char>(255 * pow(color[2], 1.0 / 2.2));
    m_colorBuffer[index + 3] = static_cast<unsigned char>(255 * color[3]);
}

bool FrameBuffer::checkXY(unsigned int x, unsigned int y)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        return false;
    return true;
}
