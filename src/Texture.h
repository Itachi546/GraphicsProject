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

class Texture{
 public:
  static TextureData LoadHeightMap(const char* filename){
    TextureData data;
    data.image = SOIL_load_image(filename, &data.width, &data.height, &data.numChannel, SOIL_LOAD_AUTO);
    return data;
  }

  static unsigned int LoadTexture(const char* filename){
    int width, height, nrChannel;
    unsigned char* image = SOIL_load_image(filename, &width, &height, &nrChannel, SOIL_LOAD_AUTO);
    if(image == null){
      std::cerr << "Failed to Load Image " << std::endl;
    }
  }
};

#endif
