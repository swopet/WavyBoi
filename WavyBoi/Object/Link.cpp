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
		out_pos = in->getLeftPos();
	}
	else {
		out_pos = in_pos;
	}
	parameter = new_parameter;
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
		out_pos = out->getLeftPos(out_ind);
	}
	if (in != NULL) {
		parameter->setValue(in->getVal());
	}
}

Parameter * Link::getParameterFromLink()
{
	return parameter;
}

bool Link::checkOverlap(sf::RectangleShape select_rect)
{
	return checkIntersection(select_rect,in_pos,out_pos);
}

void Link::draw(sf::RenderTarget& target, sf::RenderStates states){
	sf::Vertex line[] = {
		sf::Vertex(in_pos),
		sf::Vertex(out_pos)
	};
	glLineWidth(LINK_LINE_THICKNESS);
	target.draw(line, 2, sf::Lines);
}

void Link::setOutInd(int new_ind)
{
	out_ind = new_ind;
}

int Link::getOutInd()
{
	return out_ind;
}

ClickResponse Link::processLeftClick(sf::Vector2i mouse_pos){
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	
	return response;
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
