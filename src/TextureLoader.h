#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <SOIL/SOIL.h>

struct TextureData{
  int width;
  int height;
  unsigned char* image;
  int numChannel;
};

class TextureLoader{
 public:
  static TextureData LoadHeightMap(const char* filename){
    TextureData data;
    data.image = SOIL_load_image(filename, &data.width, &data.height, &data.numChannel, SOIL_LOAD_AUTO);
    return data;
  }

  static unsigned int LoadTexture(const char* filename){
    int width, height, nrChannel;
    unsigned char* image = SOIL_load_image(filename, &width, &height, &nrChannel, SOIL_LOAD_AUTO);
    if(image == 0){
      std::cerr << "Failed to Load Image " << std::endl;
      return -1;
    }

    GLenum format = GL_RGBA;
    if(nrChannel == 1) format = GL_RED;
    else if(nrChannel == 3) format = GL_RGB;

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureId;
  }


};

#endif
