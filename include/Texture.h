#ifndef TEXTURE_H
#define TEXTURE_H

class Texture
{
public:
    Texture();
    ~Texture();
    void setUpTexture(const char *fileName);
    void useTexture();
    unsigned int textName[1];
};

#endif