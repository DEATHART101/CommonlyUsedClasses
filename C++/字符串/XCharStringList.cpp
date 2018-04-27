#include "XCharStringList.h"
#include <vector>

XCharStringList::XCharStringList()
{
	data = nullptr;
	next = nullptr;
	prev = nullptr;
}

XCharStringList::XCharStringList(const XCharString & data)
{
	this->data = new XCharString(data);
	next = nullptr;
	prev = nullptr;
}

XCharStringList::~XCharStringList()
{
	if (data != nullptr)
	{
		delete data;
	}
	if (next != nullptr)
	{
		delete next;
	}
}

Status XCharStringList::push(const XCharString & data)
{
	XCharStringList* end = this;
	while (end->next != nullptr)
	{
		end = end->next;
	}

	end->next = new XCharStringList(data);
	end->next->prev = end;

	return OK;
}

Status XCharStringList::remove(int position)
{
	XCharStringList* pin = next;
	while (position != 0)
	{
		pin = pin->next;
		position--;
	}

	pin->prev->next = pin->next;
	pin->next->prev = pin->prev;
	pin->next = nullptr;
	pin->next = nullptr;

	delete pin;

	return OK;
}

int XCharStringList::search(const XCharString & data) const
{
	int pos = -1;
	XCharStringList* pin = next;
	while (true)
	{
		if (pin == nullptr)
		{
			return -1;
		}

		pos++;
		if (*(pin->data) == data)
		{
			return pos;
		}
		pin = pin->next;
	}
}

int XCharStringList::length() const
{
	int result = 0;
	XCharStringList* pin = next;
	while (pin != nullptr)
	{
		result++;
		pin = pin->next;
	}

	return result;
}

Status XCharStringList::find_at(int position, XCharString & output) const
{
	if (position < 0)
	{
		return ERROR;
	}

	XCharStringList* pin = next;
	while (position != 0)
	{
		if (pin == nullptr)
		{
			return ERROR;
		}
		pin = pin->next;
		position--;
	}

	output = *pin->data;
}

void XCharStringList::enumerate(function<void(XCharString&data)> func) const
{
	XCharStringList* pin = next;
	while (pin != nullptr)
	{
		func(*(pin->data));
		pin = pin->next;
	}
}

void XCharStringList::clear()
{
	if (data != nullptr)
	{
		delete data;
		data = nullptr;
	}
	if (next != nullptr)
	{
		delete next;
		next = nullptr;
	}
}

const XCharString & XCharStringList::operator[](int position) const
{
	if (position < 0 || next == nullptr)
	{
		throw "Out Of Range";
	}

	XCharStringList* pin = next;
	while (position != 0)
	{
		if (pin == nullptr)
		{
			throw "Out Of Range";
		}
		pin = pin->next;
		position--;
	}

	return *pin->data;
}
