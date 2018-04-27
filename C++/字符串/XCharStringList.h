#pragma once

//used to contain strings.
//have an empty header

#include "XCharString.h"
#include <functional>

typedef class XCharStringList
{
public:
	XCharStringList();
	~XCharStringList();

	Status push(const XCharString& data);
	Status remove(int position);
	int search(const XCharString& data) const;
	int length() const;
	Status find_at(int position, XCharString& output) const;
	void enumerate(function<void(XCharString& data)>) const;
	void clear();

	const XCharString & operator[](int position) const;

private:
	XCharStringList(const XCharString& data);

private:
	XCharString* data;
	XCharStringList* prev;
	XCharStringList* next;
	
}xslist;

