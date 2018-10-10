#include <pch.h>
#include <Object\Parameter.h>

PARAM_TYPE Parameter::getType()
{
	return type;
}

param Parameter::getValue()
{
	return value;
}

std::string Parameter::getName()
{
	return name;
}

void Parameter::setValue(param new_value)
{
	value = new_value;
}

Parameter::Parameter(PARAM_TYPE new_type, param new_value, std::string new_name)
{
	type = new_type;
	value = new_value;
	name = new_name;
}

Parameter::Parameter(){
	name = "unnamed";
}