#include "pch.h"
#include "Shader.h"


Shader::Shader() : SceneObject()
{
  shader = NULL;
  ready = false;
}

Shader::Shader(std::string new_path) : SceneObject()
{
  std::cout << "load shader from " << new_path << std::endl;
  filename = new_path;
}


Shader::~Shader()
{
}
