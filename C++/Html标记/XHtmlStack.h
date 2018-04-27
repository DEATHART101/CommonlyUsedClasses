#pragma once

//use to hold html stack.
//for some reason this is not used.

#include "XStack.h"
#include "XCharString.h"

#define DEFAULT_STACK_LENGTH 10000

typedef class XHtmlStack
{
public:
	XHtmlStack();
	XHtmlStack(string* html);
	~XHtmlStack();

	bool PushStack();
	string SearchStack(const string& tag_begin);
	void PrintStack();

private:
	XStack<string> content_stack;
	string* my_html;
	int* stack_positions;
}xhstack;

