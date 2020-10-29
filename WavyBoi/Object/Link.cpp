#include <pch.h>
#include <Object\Link.h>

Link::Link(Object * obj_in, Object *obj_out, Parameter *new_parameter)
{
	in = obj_in;
	out = obj_out;
	if (in != NULL) {
		in_pos = in->getRightPos();
	}
	if (out != NULL) {
		out_pos = out->getLeftPos();
	}
	else {
		out_pos = in_pos;
	}
	parameter = new_parameter;
	name = parameter->getName();
	type = OBJECT_TYPE::LINK;
}

Link::Link(){
	in = NULL;
	out = NULL;
	parameter = NULL;
	type = OBJECT_TYPE::LINK;
}

Link::~Link(){
	if (parameter != NULL) {
		std::cout << "deleted link for " << parameter->getName() << std::endl;
		delete parameter;
	}
}

void Link::update()
{
	if (in != NULL) {
		in_pos = in->getRightPos();
	}
	if (out != NULL) {
		out_pos = out->getLeftPos(out_index);
	}
	if (in != NULL) {
		Parameter new_parameter = in->getParameter();
		parameter->setValue(new_parameter.getValue());
		parameter->setType(new_parameter.getType());
	}

	if (name.length() == 0) text.setString("...");
	else text.setString(name);
	text.setFont(gui.font);
	text.setCharacterSize(gui.input_text_height);
	text.setFillColor(sf::Color::White);
	//clean this up a little bit yo
	text.setPosition((sf::Vector2f(in_pos.x + 10 , in_pos.y) + sf::Vector2f(out_pos.x - 10,out_pos.y)) / 2.0f + sf::Vector2f(-text.getGlobalBounds().width / 2, 0));

}

Parameter * Link::getParameterFromLink()
{
	parameter->setName(name);
	return parameter;
}

bool Link::checkOverlap(sf::RectangleShape select_rect)
{
	bool return_value = (checkIntersection(select_rect.getGlobalBounds(), in_pos, sf::Vector2f(in_pos.x + 10, in_pos.y))
		|| checkIntersection(select_rect.getGlobalBounds(), sf::Vector2f(in_pos.x + 10, in_pos.y), sf::Vector2f(out_pos.x - 10, out_pos.y))
		|| checkIntersection(select_rect.getGlobalBounds(), sf::Vector2f(out_pos.x - 10, out_pos.y), out_pos)
		|| checkIntersection(select_rect.getGlobalBounds(), in_pos)
		|| checkIntersection(select_rect.getGlobalBounds(), sf::Vector2f(in_pos.x + 10, in_pos.y))
		|| checkIntersection(select_rect.getGlobalBounds(), sf::Vector2f(out_pos.x - 10, out_pos.y))
		|| checkIntersection(select_rect.getGlobalBounds(), out_pos));
	return return_value;
}

void Link::draw(sf::RenderTarget& target, sf::RenderStates states){
	sf::Vertex line[] = {
		sf::Vertex(in_pos),
		sf::Vertex(sf::Vector2f(in_pos.x+10,in_pos.y)),
		sf::Vertex(sf::Vector2f(out_pos.x-10,out_pos.y)),
		sf::Vertex(out_pos)
	};
	for (int i = 0; i < 4; i++) {
		line[i].color = sf::Color::White;
	}
	target.draw(line, 4, sf::LineStrip, states);
	target.draw(text, states);
}

void Link::setOutIndex(int new_index)
{
	out_index = new_index;
}

int Link::getOutIndex()
{
	return out_index;
}

ClickResponse Link::processLeftClick(sf::Vector2i mouse_pos){
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	return response;
}

ClickResponse Link::processDoubleLeftClick(sf::Vector2i mouse_pos)
{
	sf::FloatRect text_rect(text.getGlobalBounds());
	sf::RectangleShape rect(sf::Vector2f(text_rect.width, text_rect.height));
	rect.setPosition(text_rect.left, text_rect.top);
	if (checkIntersection(rect.getGlobalBounds(), sf::Vector2f(mouse_pos))) {
		ClickResponse response;
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_TEXT_FIELD;
		response.field = "name";
		return response;
	}
	return ClickResponse();
}

Object * Link::getInObj()
{
	return in;
}

Object * Link::getOutObj()
{
	return out;
}

void Link::setOutPos(sf::Vector2f new_out_pos)
{
	out_pos = new_out_pos;
}

bool Link::setOutObject(Object * new_out_obj)
{
	if (new_out_obj == out) return false;
	out = new_out_obj;
	out_pos = out->getLeftPos();
	return true;
}

void Link::processNewString(std::string field, std::string input)
{
	if (field == "name") {
		name = input;
	}
}
