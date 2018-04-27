#pragma once

//template stack. standard interface.

#include <string.h>
#include "XStatus.h"

#define DEFAULT_ARRAY_LENGTH 50
#define OUTOF_ARRAY_ADD_LENGTH 100

template <typename T>
class XArrayStack
{
private:
	int max_length;
	int top_index;

	T* content_array;

public:
	XArrayStack()
	{
		content_array = new T[DEFAULT_ARRAY_LENGTH];
		max_length = DEFAULT_ARRAY_LENGTH;
		top_index = -1;
	}

	XArrayStack(int length)
	{
		content_array = new T[length];
		max_length = length;
		top_index = -1;
	}

	XArrayStack(const XArrayStack<T>& other)
	{
		max_length = other.max_length;
		top_index = other.top_index;
		content_array = new T[max_length];
		memcpy(content_array, other.content_array, sizeof(T) * top_index + 1);
	}

	/*XStack(const XStack<T>&& other)
	{
		l_assign(other);
	}*/

	~XArrayStack()
	{
		if (content_array != nullptr)
		{
			delete[] content_array;
		}
	}

	int size() const
	{
		return max_length;
	}

	int length() const
	{
		return top_index + 1;
	}

	Status push(const T& data)
	{
		if (top_index == max_length - 1)
		{
			T* temp = new T[max_length + OUTOF_ARRAY_ADD_LENGTH];
			
			for (int i = 0; i < max_length; i++)
			{
				temp[i] = content_array[i];
			}

			delete[] content_array;
			content_array = temp;
			max_length = max_length + OUTOF_ARRAY_ADD_LENGTH;
		}

		content_array[top_index + 1] = data;
		top_index++;

		return OK;
	}

	Status pop(T& out_value)
	{
		if (top_index == -1)
		{	
			return ERROR;
		}

		out_value = content_array[top_index];
		top_index--;

		return OK;
	}

	Status top(T& out_value)
	{
		if (top_index == -1)
		{
			return ERROR;
		}

		out_value = content_array[top_index];

		return OK;
	}

	bool empty()
	{
		if (top_index == -1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	const T& operator[](int pos)
	{
		if (pos < 0 || pos > top_index)
		{
			return T();
		}

		return content_array[pos];
	}

private:
	void l_assign(const XArrayStack<T>& other)
	{
		if (content_array != nullptr)
		{
			delete[] content_array;
		}

		content_array = other.content_array;
		top_index = other.top_index;
		max_length = other.max_length;
	}
};