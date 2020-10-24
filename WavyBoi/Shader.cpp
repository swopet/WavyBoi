#include "pch.h"
#include "Shader.h"


Shader::Shader() : SceneObject()
{
  shader = NULL;
  ready = false;
  texture_size = sf::Vector2u(1920, 1080);
}

Shader::Shader(std::string new_path) : SceneObject()
{
  std::cout << "load shader from " << new_path << std::endl;
  shader = NULL;
  ready = false;
  filename = new_path;
  texture_size = sf::Vector2u(1920, 1080);
  loadScene();
}

void Shader::loadScene()
{
  std::cout << "loadScene" << std::endl;
  sf::Clock clock;
  sf::Time start_time = clock.getElapsedTime();
  if (shader != NULL) {
    delete shader;
  }
  shader = new sf::Shader();
  if (shader->loadFromFile(filename, sf::Shader::Fragment)) {
    valid = true;
  }
  else {
    delete shader;
    shader = NULL;
  }
  ready_mutex.lock();
  ready = true;
  ready_mutex.unlock();
  sf::Time elapsed_time = clock.getElapsedTime() - start_time;
  std::cout << "loaded " << filename << " in " << elapsed_time.asSeconds() << "s, ready!" << std::endl;
  init();
}

void Shader::init()
{
  ready_mutex.lock();
  size = sf::Vector2f(60, 60);
  if (render_texture == NULL) {
    render_texture = new sf::RenderTexture();
    render_texture->create(texture_size.x, texture_size.y);
  }
  if (ready && shader != NULL) {
    sf::Vector2f scene_size = sf::Vector2f(texture_size);
    if (scene_size.y != 0) {
      float ratio = scene_size.x / scene_size.y;
      size = sf::Vector2f(60 * ratio, 60);
    }
  }
  ready_mutex.unlock();
  initializeElements();
}

void Shader::update() {
  if (ready && shader != NULL)
  {
    render_texture->clear(sf::Color::Black);
    render_texture->setActive();
    sf::RectangleShape full_rect(sf::Vector2f(render_texture->getSize()));
    shader->setUniform("iResolution", sf::Vector2f(render_texture->getSize()));
    for (auto it = params.begin(); it != params.end(); ++it) {
      switch ((*it)->getType()) {
      case PARAM_TYPE::TEXTURE:
        shader->setUniform((*it)->getName(), *(*it)->getValue().texture);
        shader->setUniform((*it)->getName() + std::string("_set"), 1);
        break;
      case PARAM_TYPE::FLOAT:
        shader->setUniform((*it)->getName(), (*it)->getValue().float_val);
        shader->setUniform((*it)->getName() + std::string("_set"), 1);
        break;
      case PARAM_TYPE::INT:
        shader->setUniform((*it)->getName(), (*it)->getValue().int_val);
        shader->setUniform((*it)->getName() + std::string("_set"), 1);
        break;
      }
    }
    sf::Shader::bind(shader);
    render_texture->draw(full_rect);
    render_texture->display();
    sf::Shader::bind(NULL);
  }
  params.clear();
}

Parameter Shader::getParameter()
{
  param return_param;
  ready_mutex.lock();
  if (ready) {
    return_param.texture = &render_texture->getTexture();
  }
  else {
    return_param.texture = NULL;
  }
  ready_mutex.unlock();
  return Parameter(PARAM_TYPE::TEXTURE, return_param, name);
}

void Shader::setParameter(Parameter * parameter, int ind)
{
  params.insert(parameter);
}

void Shader::updateGUIElements() {
  ready_mutex.lock();
  if (ready && shader != NULL)
  {
    if (elements.size() == 3) {
      elements.at(1).setTexture(&render_texture->getTexture());
    }
  }
  else {
    if (elements.size() == 3) {
      elements.at(1).setTexture(NULL);
    }
  }
  ready_mutex.unlock();
  size_t loop_rect = 2;
  if (elements.size() >= 3) elements.at(loop_rect).setTexture(&gui.loop_false_24x24_tex);
  Object::updateGUIElements();
}

Shader::~Shader()
{
}
