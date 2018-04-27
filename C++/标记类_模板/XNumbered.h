#pragma once

template <typename T>
class XNumbered
{
public:
	T data;
	int identifier;

	XNumbered()
	{

	}

	XNumbered(const T& data, int identifier)
	{
		this->data = data;
		this->identifier = identifier;
	}
};