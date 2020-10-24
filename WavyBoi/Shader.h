#pragma once
#include "SceneObject.h"
#include <unordered_set>
class Shader :
  public SceneObject
{
private:
  std::string path;
  sf::Shader * shader;
  sf::RenderTexture * render_texture;
  std::unordered_set<Parameter *> params;
  sf::Vector2u texture_size;
public:
  Shader();
  Shader(std::string);
  void loadScene();
  void init();
  void update();
  void setParamsToDefault();
  void clearParameter(Parameter * param, int index);
  Parameter getParameter();
  void setParameter(Parameter * parameter, int ind);
  void updateGUIElements();
  ~Shader();
};

