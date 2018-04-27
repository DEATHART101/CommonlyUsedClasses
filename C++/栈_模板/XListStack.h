#pragma once
#pragma once

//template stack. standard interface.

#include <string.h>
#include "XStatus.h"
#include "XList.h"

template <typename T>
class XListStack
{
private:
	XList<T> m_contentList;

public:
	XListStack()
	{

	}

	XListStack(const XListStack<T>& other)
	{
		other.m_contentList.enumerate_readonly([](const T& data) -> void {
			m_contentList.push(data);
		});
	}

	~XListStack()
	{

	}

	int length() const
	{
		return m_contentList.length();
	}

	Status push(const T& data)
	{
		return m_contentList.push_back(data);
	}

	Status pop(T& out_value)
	{
		return m_contentList.pop_back(out_value);
	}

	Status top(T& out_value)
	{
		return m_contentList.get_back(out_value);
	}

	bool empty()
	{
		return m_contentList.empty();
	}

	int length()
	{
		return m_contentList.length();
	}

	const T& operator[](int pos)
	{
		try
		{
			return m_contentList[pos];
		}
		catch (...)
		{
			throw "Out of Range";
		}
	}
};