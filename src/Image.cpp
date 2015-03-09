#include "../include/Image.h"

Image::Image()
{
    components = width = height = 0;
    data = 0;
}

Image::~Image()
{
    if(data)
        delete [] data;
}

void Image::LoadFromFile(const std::string& filepath)
{
    data = ReadImage(filepath.c_str(), components, width, height);
}

int Image::GetSize() const
{
    return width * height * components * sizeof(unsigned char);
}

int Image::GetComponents() const
{
    return components;
}

int Image::GetWidth() const
{
    return width;
}

int Image::GetHeight() const
{
    return height;
}

const void* Image::GetData() const
{
    return data;
}
