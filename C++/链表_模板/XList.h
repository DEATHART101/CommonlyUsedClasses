#pragma once

#include <functional>
#include "XStatus.h"
using namespace std;

template <typename T>
class XList
{
private:
	struct XListNode 
	{
	public:
		T* data;
		XListNode* prev;
		XListNode* next;

	public:
		XListNode(const T& value) :
			data(new T(value)),
			prev(nullptr),
			next(nullptr)
		{

		}

		~XListNode()
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
	};

public:
	class iterator
	{
	private:
		iterator() :
			m_listnode(nullptr)
		{

		}

	public:
		iterator(XListNode* node) :
			m_listnode(node)
		{

		}

	private:
		XListNode* m_listnode;

	public:
		void operator++ (int)
		{
			if (m_listnode != nullptr)
			{
				m_listnode = m_listnode->next;
			}
		}

		T& operator*()
		{
			return *(m_listnode->data);
		}

		bool operator== (const iterator& other)
		{
			return m_listnode == other.m_listnode;
		}

		bool operator!= (const iterator& other)
		{
			return m_listnode != other.m_listnode;
		}
	};

private:
	XListNode * m_start;
	XListNode* m_end;

	int m_length;

public:
	XList() :
		m_start(nullptr),
		m_end(nullptr),
		m_length(0)
	{

	}

	~XList()
	{
		if (m_start != nullptr)
		{
			delete m_start;
		}
	}

	Status push(const T& value)
	{
		return push_back(value);
	}

	Status push_back(const T& value)
	{
		if (m_start == nullptr)
		{
			CreateHeaderNode(value);
			m_length++;
			return OK;
		}

		XListNode* temp = new XListNode(value);
		m_end->next = temp;
		temp->prev = m_end;
		m_end = temp;
		m_length++;
		return OK;
	}

	Status push_front(const T& value)
	{
		if (m_start == nullptr)
		{
			CreateHeaderNode(value);
			m_length++;
			return OK;
		}

		XListNode* temp = new XListNode(value);
		temp->next = m_start;
		m_start->prev = temp;
		m_start = temp;
		m_length++;
		return OK;
	}

	Status pop_front()
	{
		if (m_start == nullptr)
		{
			return ERROR;
		}

		if (m_length == 1)
		{
			clear();
			return OK;
		}

		XListNode* second = m_start->next;
		if (second != nullptr)
		{
			second->prev = nullptr;
		}
		m_start->next = nullptr;
		delete m_start;

		m_start = second;
		m_length--;
		return OK;
	}

	Status pop_back()
	{
		if (m_start == nullptr)
		{
			return ERROR;
		}

		if (m_length == 1)
		{
			clear();
			return OK;
		}

		XListNode* rsecond = m_end->prev;
		if (rsecond != nullptr)
		{
			rsecond->next = nullptr;
		}
		m_end->prev = nullptr;
		delete m_end;
		
		m_end = rsecond;
		m_length--;
		return OK;
	}

	Status pop_front(T& out_value)
	{
		if (m_start == nullptr)
		{
			return ERROR;
		}

		if (m_length == 1)
		{
			out_value = *(m_start->data);
			clear();
			return OK;
		}

		XListNode* second = m_start->next;
		if (second != nullptr)
		{
			second->prev = nullptr;
		}
		m_start->next = nullptr;

		out_value = *(m_start->data);
		delete m_start;

		m_start = second;
		m_length--;
		return OK;
	}

	Status pop_back(T& out_value)
	{
		if (m_start == nullptr)
		{
			return ERROR;
		}

		if (m_length == 1)
		{
			out_value = *(m_start->data);
			clear();
			return OK;
		}

		XListNode* rsecond = m_end->prev;
		if (rsecond != nullptr)
		{
			rsecond->next = nullptr;
		}
		m_end->prev = nullptr;

		out_value = *(m_end->data);
		delete m_end;

		m_end = rsecond;
		m_length--;
		return OK;
	}

	void enumerate(function<void(T& data)> func)
	{
		if (m_start == nullptr)
		{
			return;
		}

		XListNode* pin = m_start;
		while (pin != nullptr)
		{
			func(*(pin->data));
			pin = pin->next;
		}
	}

	void enumerate_readonly(function<void(const T& data)> func) const
	{
		if (m_start == nullptr)
		{
			return;
		}

		const XListNode* pin = m_start;
		while (pin != nullptr)
		{
			func(*(pin->data));
			pin = pin->next;
		}
	}

	void enumerate_reverse_readonly(function<void(const T& data)> func) const
	{
		if (m_end == nullptr) 
		{
			return;
		}

		const XListNode* pin = m_end;
		while (pin != nullptr)
		{
			func(*(pin->data));
			pin = pin->prev;
		}
	}

	iterator begin() const
	{
		return iterator(m_start);
	}

	iterator end() const
	{
		return iterator(nullptr);
	}

	Status sort(function<bool(const T&, const T&)> func_first_bigger, bool bigger)
	{
		if (m_length == 0)
		{
			return ERROR;
		}

		if (m_length == 1)
		{
			return OK;
		}

		if (bigger)
		{
			XListNode* pin_i = m_start->next;
			while (pin_i != nullptr)
			{
				if (func_first_bigger(*(pin_i->prev->data), *(pin_i->data)))
				{
					XListNode* save_i = pin_i->prev;
					XListNode* pin_j = pin_i->prev;
					while (pin_j != nullptr)
					{
						if (pin_j->prev == nullptr || func_first_bigger(*(pin_i->data), *(pin_j->prev->data)))
						{
							pin_i->prev->next = pin_i->next;
							if (pin_i->next != nullptr)
							{
								pin_i->next->prev = pin_i->prev;
							}
							pin_i->next = pin_j;
							pin_i->prev = pin_j->prev;
							
							// If pin_j is start node
							if (pin_j->prev == nullptr)
							{
								m_start = pin_i;
							}
							else
							{
								pin_j->prev->next = pin_i;
							}
							pin_j->prev = pin_i;

							pin_i = save_i;
							break;
						}
						pin_j = pin_j->prev;
					}
				}
				pin_i = pin_i->next;
			}
		}
		else
		{
			XListNode* pin_i = m_end->prev;
			while (pin_i != nullptr)
			{
				if (func_first_bigger(*(pin_i->next->data), *(pin_i->data)))
				{
					XListNode* save_i = pin_i->next;
					XListNode* pin_j = pin_i->next;
					while (pin_j != nullptr)
					{
						if (pin_j->next == nullptr || func_first_bigger(*(pin_i->data), *(pin_j->next->data)))
						{
							pin_i->next->prev = pin_i->prev;
							if (pin_i->prev != nullptr)
							{
								pin_i->prev->next = pin_i->next;
							}
							pin_i->next = pin_j->next;
							pin_i->prev = pin_j;

							// If pin_j is end node
							if (pin_j->next == nullptr)
							{
								m_end = pin_i;
							}
							else
							{
								pin_j->next->prev = pin_i;
							}
							pin_j->next = pin_i;

							pin_i = save_i;
							break;
						}
						pin_j = pin_j->next;
					}
				}
				pin_i = pin_i->prev;
			}
		}

		return OK;
	}

	Status get_front(T& out_value)
	{
		if (m_start == nullptr)
		{
			return ERROR;
		}

		out_value = *(m_start->data);
		return OK;
	}

	Status get_back(T & out_value)
	{
		if (m_end == nullptr)
		{
			return ERROR;
		}

		out_value = *(m_end->data);
		return OK;
	}

	Status get_position(T& out_value, int position)
	{
		if (position < 0 ||
			position >= m_length ||
			m_start == nullptr)
		{
			return ERROR;
		}

		XListNode* pin = m_start;
		while (position != 0)
		{
			pin = pin->next;
			position--;
		}

		out_value = *(pin->data);
		return OK;
	}

	void clear()
	{
		if (m_start != nullptr)
		{
			delete m_start;
			m_start = nullptr;
			m_end = nullptr;
			m_length = 0;
		}
	}

	bool empty() const
	{
		return m_start == nullptr;
	}

	int length() const
	{
		return m_length;
	}

	const T& operator[] (int position) const
	{
		T result;
		if (get_position(result, position) == ERROR)
		{
			throw "Out of Range";
		}
		else
		{
			return result;
		}
	}

	XList& operator= (const XList& other)
	{
		clear();

		other.enumerate_readonly([&](const T& data) -> void {
			this->push(data);
		});

		return *this;
	}

private:
	void CreateHeaderNode(const T& value)
	{
		if (m_start != nullptr)
		{
			return;
		}

		m_start = new XListNode(value);
		m_end = m_start;
	}
};