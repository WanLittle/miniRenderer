#pragma  once

#include <vector>
#include "../Math/vec.h"

class FrameBuffer
{
private:
    std::vector<double> m_depthBuffer;          // z-buffer
    unsigned int m_width, m_height, m_channel;  // viewport.
    std::vector<unsigned char> m_colorBuffer;   // color buffer.

public:
    FrameBuffer(int width, int height);
    ~FrameBuffer() = default;

    void clearColorAndDepthBuffer(const glm::vec4f &color);

    int getWidth()const {return m_width;}
    int getHeight()const {return m_height;}

    unsigned char *getColorBuffer() {return m_colorBuffer.data();}
    double getDepth(const unsigned int x, const unsigned int y)const;

    void drawDepth(const unsigned int x, const unsigned int y, const double value);
    void drawPixel(const unsigned int x, const unsigned int y, const glm::vec4f &color);

private:
    inline bool checkXY(unsigned int x, unsigned int y);
};

