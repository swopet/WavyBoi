#include "pch.h"
#include "Register.h"


Register::Register()
{
  type = OBJECT_TYPE::REGISTER;
  in_param_type = PARAM_TYPE::NONE;
  out_param_type = PARAM_TYPE::NONE;
  out_texture = NULL;
  position = sf::Vector2f(32.0f, 32.0f);
  initializeElements();
}

void Register::initializeElements() {
  elements.clear();
  inputs.clear();
  outputs.clear();
  GUIElement left_box(GUIElement::RECTANGLE, 24 - gui.outline_thickness * 2, 24 - gui.outline_thickness * 2);
  left_box.setOrigin(position);
  left_box.setPosition(sf::Vector2f(0.0f, 0.0f));
  GUIElement right_box(GUIElement::RECTANGLE, 24 - gui.outline_thickness * 2, 24 - gui.outline_thickness * 2);
  right_box.setPosition(sf::Vector2f(24 - gui.outline_thickness * 2, 0.0));
  elements.push_back(left_box);
  elements.push_back(right_box);
  GUIElement input_circle = GUIElement(GUIElement::CIRCLE, gui.obj_circle_radius);
  inputs.push_back(input_circle);
  if (in_param_type != PARAM_TYPE::NONE) {
    GUIElement output_circle = GUIElement(GUIElement::CIRCLE, gui.obj_circle_radius);
    outputs.push_back(output_circle);
  }
  updateGUIElements();
}

void Register::update() {
  updateGUIElements();
}

void Register::updateGUIElements() {
  inputs.at(0).setPosition(
    sf::Vector2f(0, elements.at(0).getGlobalBounds().height / 2.0) +
    sf::Vector2f(-gui.outline_thickness / 2.0, 0.0) +
    sf::Vector2f(-gui.obj_circle_radius, -gui.obj_circle_radius));
  if (in_param_type != PARAM_TYPE::NONE) {
    outputs.at(0).setPosition(
      sf::Vector2f(elements.at(0).getGlobalBounds().width + elements.at(1).getGlobalBounds().width, elements.at(1).getGlobalBounds().height / 2.0) +
      sf::Vector2f(gui.outline_thickness / 2.0, 0.0) +
      sf::Vector2f(-gui.obj_circle_radius, -gui.obj_circle_radius));
  }
  Object::updateGUIElements();
}

void Register::updateRegister()
{
  bool flip = false;
  switch (in_param_type) {
  case PARAM_TYPE::VIDEO_TEXTURE:
    flip = true;
  case PARAM_TYPE::SHADER_TEXTURE:
    if (out_param_type != PARAM_TYPE::SHADER_TEXTURE) {
      out_texture = new sf::RenderTexture();
      out_texture->create(in_val.texture->getSize().x, in_val.texture->getSize().y);
      out_param_type = PARAM_TYPE::SHADER_TEXTURE;
    }
    else if (out_texture->getSize() != in_val.texture->getSize()) {
      delete out_texture;
      out_texture = new sf::RenderTexture();
      out_texture->create(in_val.texture->getSize().x, in_val.texture->getSize().y);
      out_param_type = PARAM_TYPE::SHADER_TEXTURE;
    }
    out_texture->setActive();
    { //rect scope for switch
      sf::RectangleShape rect(sf::Vector2f(in_val.texture->getSize().x, (flip ? -1.0 : 1.0) * in_val.texture->getSize().y));
      rect.setPosition(sf::Vector2f(0.0, (flip ? in_val.texture->getSize().y : 0.0)));
      rect.setTexture(in_val.texture);
      out_texture->draw(rect);
    }
    out_texture->display();
    out_val.texture = &out_texture->getTexture();
    break;
  default:
    out_val = in_val;
    out_param_type = in_param_type;
    break;
  }
}

void Register::setParameter(Parameter * parameter, int ind)
{
  if (ind != 0) return;
  in_val = parameter->getValue();
  if (parameter->getType() != in_param_type) {
    in_param_type = parameter->getType();
    initializeElements();
  }
}

Parameter * Register::getNewParameter() {
  std::cout << "returning new parameter from register of type ";
  switch (out_param_type) {
  case PARAM_TYPE::INT:
      std::cout << "int: ";
      std::cout << out_val.int_val << std::endl;
      break;
    case PARAM_TYPE::FLOAT:
      std::cout << "float: ";
      std::cout << out_val.float_val << std::endl;
      break;
  }
  return new Parameter(out_param_type, out_val, name);
}

Parameter Register::getParameter() {
  return Parameter(out_param_type, out_val, name);
}

Register::~Register()
{
}
