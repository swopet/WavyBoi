#pragma once
#include "Object/Object.h"

class Register :
  public Object
{
private:
  PARAM_TYPE in_param_type;
  PARAM_TYPE out_param_type;
  param in_val;
  param out_val;
  sf::RenderTexture * out_texture;
public:
  Register();
  void initializeElements();
  void update();
  void updateGUIElements();
  void updateRegister();
  void setParameter(Parameter * parameter, int ind);
  Parameter * getNewParameter();
  Parameter getParameter();
  ~Register();
};

