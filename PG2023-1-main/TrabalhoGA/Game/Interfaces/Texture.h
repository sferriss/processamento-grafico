#pragma once
#include <string>

class Texture
{
public:
    Texture();

    static int load(const std::string& path);
};
