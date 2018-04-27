#include "XHtmlStack.h"

XHtmlStack::XHtmlStack() : 
	content_stack(XStack<string>(DEFAULT_STACK_LENGTH))
{
	my_html = nullptr;
	stack_positions = new int[DEFAULT_STACK_LENGTH];
}

XHtmlStack::XHtmlStack(string * html) :
	content_stack(XStack<string>(DEFAULT_STACK_LENGTH))
{
	my_html = html;
	stack_positions = new int[DEFAULT_STACK_LENGTH];
}


XHtmlStack::~XHtmlStack()
{
	if (stack_positions != nullptr)
	{
		delete[] stack_positions;
	}
}

bool XHtmlStack::PushStack()
{
	if (my_html == nullptr)
	{
		return false;
	}

	int length = (*my_html).length();

	int cur_push_pos = (*my_html).find('<');
	int cur_push_end = 0;

	while (cur_push_pos != -1)
	{
		cur_push_end = (*my_html).find('>', cur_push_pos);
		content_stack.push((*my_html).substr(cur_push_pos, cur_push_end + 1));
		cur_push_pos = (*my_html).find('<', cur_push_end + 1);
	}

	return true;
}

string XHtmlStack::SearchStack(const string& tag_begin)
{
	int length = content_stack.length();
	for (int i = 0; i < length; i++)
	{
		string get_i = content_stack[i];
		if (tag_begin == get_i)
		{
			return my_html->substr(
				stack_positions[i] + content_stack[i].length(),
				stack_positions[i + 1] - 1
			);
		}
	}

	return string();
}

void XHtmlStack::PrintStack()
{
	for (int i = 0; i < content_stack.length(); i++)
	{
		cout << content_stack[i] << endl;
	}

	cout << endl;
}
