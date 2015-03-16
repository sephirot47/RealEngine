#ifndef GBUFFER_H
#define GBUFFER_H

#include <vector>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>

#include "stb_image.h"
#include "FrameBuffer.h"
#include "Debug.h"
#include "glm/glm.hpp"

class GBuffer : public FrameBuffer
{
private:

public:

    GBuffer(int width, int height);
    ~GBuffer();


};

#endif // GBUFFER_H
