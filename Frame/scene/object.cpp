#include "Object.h"

void Object::SetName(const char* name)
{
	strcpy_s(this->name, 32, name);
}