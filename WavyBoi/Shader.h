#pragma once
#include "SceneObject.h"
class Shader :
  public SceneObject
{
private:
  std::string path;
  sf::Shader * shader;
public:
  Shader();
  Shader(std::string);
  ~Shader();
};

