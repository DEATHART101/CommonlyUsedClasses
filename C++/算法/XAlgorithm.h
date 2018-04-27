#pragma once

#include <iostream>

#include <functional>
#include <math.h>
#include "XStatus.h"
#include "XListStack.h"
#include "XCharString.h"
#include "XVector2.h"

using namespace std;

class XAlgorithm
{
private:
	XAlgorithm();
	~XAlgorithm();

public:
	template <typename T>
	static void PrintArray(T* array, int length, ostream& output, int newline_length = 10);

public:
	template <typename T>
	static Status QuickSort(T* const array, int length, function<bool(const T&, const T&)> func_first_larger, bool bigger);

private:
	// [start, end]
	template <typename T>
	static int QuickSort_Partition(T* array, int start, int end, function<bool(const T&, const T&)> func_first_larger, bool bigger);

	struct QuickSort_StackObject
	{
		int start = -1;
		int end = -1;
		int keyword_index = -1;
		unsigned int excuting_line = -1;
	};

public:
	static Status ClosestPair(const XVector2* const array, int length, int& out_one_index, int& out_two_index);

private:
	static int ClosestPair_GetLeftPointsCount(const XVector2* const point_array, const int* const pointi_array, int length, float middle);
	static int ClosestPair_GetRightPointsCount(const XVector2* const point_array, const int* const pointi_array, int length, float middle);

	struct ClosestPair_Pair
	{
		int point_a_index = -1;
		int point_b_index = -1;
	};
	struct ClosestPair_StackObject
	{
		int start = -1;
		int end = -1;
		ClosestPair_Pair left_min_pair;
		ClosestPair_Pair right_min_pair;

		ClosestPair_Pair* return_to = nullptr;

		unsigned int excuting_line = -1;
	};

public:
	static Status ClosestPairN2(const XVector2* const array, int length, int& out_one_index, int& out_two_index);

public:
	template <typename T>
	static Status InsertionSort(T* const value_array, int length, function<bool(const T&, const T&)> func_first_bigger, bool bigger);

public:
	template <typename T>
	static Status InsertionSort(XList<T>& value_list, function<bool(const T&, const T&)> func_first_bigger, bool bigger);

public:
	template <typename T>
	static Status ShellSort(T* const value_array, int length, function<bool(const T&, const T&)> func_first_bigger, bool bigger);

public:
	template <typename T>
	static Status MergeSort(T* const value_array, int length, function<bool(const T&, const T&)> func_first_bigger, bool bigger);

private:
	struct MergeSort_StackObject
	{
		int start = -1;
		int end = -1;
		unsigned int excuting_line = -1;
	};

public:
	static Status RadixSort(unsigned int * const value_array, int length, bool bigger);

public:
	// Out position is either the position of the value or the postion of the value that is the first value smaller than to_search
	// So there is a chance out_position will be -1
	template <typename T>
	static Status HalfFind(T* const value_array, int length,
		const T& to_search,
		function<bool(const T&, const T&)> func_first_bigger,
		function<bool(const T&, const T&)> func_same,
		int& out_position);

public:
	template <typename T>
	static Status GetLIS(T* const value_array, int length,
		function<bool(const T&, const T&)> func_first_bigger,
		function<bool(const T&, const T&)> func_same,
		XList<T>& out_list);

private:
	typedef struct GetLIS_AnswerListNode
	{
		unsigned int index;
		XList<XAlgorithm::GetLIS_AnswerListNode*> nexts;
	}Galn;

	static Status GetLIS_GetLongestIndexList(const Galn* const head, XList<unsigned int>& out_value)
	{
		if (head->nexts.length() == 0)
		{
			out_value.push_back(head->index);
			return OK;
		}
		
		XList<unsigned int> last_answer;
		for (auto i = head->nexts.begin(); i != head->nexts.end(); i++)
		{
			XList<unsigned int> out_value;
			GetLIS_GetLongestIndexList(*i, out_value);

			if (out_value.length() > last_answer.length())
			{
				last_answer = out_value;
			}
		}

		out_value = last_answer;
		out_value.push(head->index);
		return OK;
	}

private:
	const static int GetShortestEditingLength_EditOperations_Numbers = 6;

public:
	typedef enum GetShortestEditingLength_EditOperations { Delete, Insert, Copy, Replace, Twiddle, Kill } EOperations;
	typedef struct GetShortestEditingLength_CostStruct
	{
		int costs[GetShortestEditingLength_EditOperations_Numbers];
	} ECostStruct;

	static Status GetShortestEditingLength(
		const xstring& a,
		const xstring& b,
		const GetShortestEditingLength_CostStruct& cost,
		XList<GetShortestEditingLength_EditOperations>& out_operations,
		int& out_length);

private:
	struct GetShortestEditingLength_StackObject
	{
		int i = -1;
		int j = -1;
		int* return_to = nullptr;

		int return_copy_replace = 0;
		int return_delete = 0;
		int return_insert = 0;
		int return_twiddle = 0;

		bool copied = false;
		int excuting_line = -1;
	};

public:
	static unsigned int GetDigit(int value, int digit);
	static unsigned int GetDigit(unsigned int value, int digit);
};

template<typename T>
inline void XAlgorithm::PrintArray(T * array, int length, ostream & output, int newline_length)
{
	for (int i = 0; i < length; i++)
	{
		output << array[i] << ' ';
		if ((i + 1) % newline_length == 0)
		{
			output << endl;
		}
	}

	output << endl;
}

template<typename T>
Status XAlgorithm::QuickSort(T * const array, int length, function<bool(const T&, const T&)> func_first_larger, bool bigger)
{
	if (length <= 0)
	{
		return ERROR;
	}

	if (length == 1)
	{
		return OK;
	}

	QuickSort_StackObject* current_stackobject = nullptr;
	XListStack<QuickSort_StackObject*> stack;

	//Init stack
	current_stackobject = new QuickSort_StackObject();
	current_stackobject->start = 0;
	current_stackobject->end = length - 1;
	current_stackobject->keyword_index = -1;
	current_stackobject->excuting_line = 0;
	stack.push(current_stackobject);

	while (true)
	{
		if (current_stackobject->excuting_line == 0)
		{
			current_stackobject->excuting_line++;

			current_stackobject->keyword_index = QuickSort_Partition(array, current_stackobject->start, current_stackobject->end, func_first_larger, bigger);
		}
		if (current_stackobject->excuting_line == 1)
		{
			current_stackobject->excuting_line++;

			// Left
			if (current_stackobject->keyword_index - current_stackobject->start > 1)
			{
				auto new_object = new QuickSort_StackObject();				// Create new stack (starting a new function)
				new_object->start = current_stackobject->start;				// Passing parameters
				new_object->end = current_stackobject->keyword_index - 1;	// Passing parameters
				new_object->keyword_index = -1;								// Passing parameters
				new_object->excuting_line = 0;								// Initialize stack

				current_stackobject = new_object;
				stack.push(current_stackobject);
				continue;
			}
		}
		if (current_stackobject->excuting_line == 2)
		{
			current_stackobject->excuting_line++;

			// Right
			if (current_stackobject->end - current_stackobject->keyword_index > 1)
			{
				auto new_object = new QuickSort_StackObject();				// Create new stack (starting a new function)
				new_object->start = current_stackobject->keyword_index + 1;	// Passing parameters
				new_object->end = current_stackobject->end;					// Passing parameters
				new_object->keyword_index = -1;								// Passing parameters
				new_object->excuting_line = 0;								// Initialize stack

				current_stackobject = new_object;
				stack.push(current_stackobject);
				continue;
			}
		}

		// Pop stack
		delete current_stackobject;
		if (stack.pop(current_stackobject) == ERROR)
		{
			return ERROR;
		}

		if (stack.empty())
		{
			// Fucntion over
			break;
		}

		// Load from stack top
		if (stack.top(current_stackobject) == ERROR)
		{
			return ERROR;
		}
	}

	return OK;
}

template<typename T>
inline int XAlgorithm::QuickSort_Partition(T * array, int start, int end, function<bool(const T&, const T&)> func_first_larger, bool bigger)
{
	int keyword_index = start;

	int i = start;
	int j = end;
	if (i == j)
	{
		return keyword_index;
	}
	while (i != j)
	{
		while (i != j)
		{
			if ((bigger && func_first_larger(array[keyword_index], array[j])) ||
				(!bigger && func_first_larger(array[j], array[keyword_index])))
			{
				T temp = array[j];
				array[j] = array[keyword_index];
				array[keyword_index] = temp;
				keyword_index = j;
				break;
			}
			j--;
		}
		while (i != j)
		{
			if ((bigger && func_first_larger(array[i], array[keyword_index])) ||
				(!bigger && func_first_larger(array[keyword_index], array[i])))
			{
				T temp = array[i];
				array[i] = array[keyword_index];
				array[keyword_index] = temp;
				keyword_index = i;
				break;
			}
			i++;
		}
	}

	return keyword_index;
}

template <typename T>
Status XAlgorithm::InsertionSort(T* const value_array, int length, function<bool(const T&, const T&)> func_first_bigger, bool bigger)
{
	if (length <= 0)
	{
		return ERROR;
	}

	if (length == 1)
	{
		return OK;
	}

	if (bigger)
	{
		for (int i = 1; i < length; i++)
		{
			if (func_first_bigger(value_array[i - 1], value_array[i]))
			{
				for (int j = i - 1; j >= 0; j--)
				{
					if (j == 0 || (j != 0 && func_first_bigger(value_array[i], value_array[j - 1])))
					{
						T swap_temp = value_array[i];
						for (int k = i - 1; k >= j; k--)
						{
							value_array[k + 1] = value_array[k];
						}
						value_array[j] = swap_temp;
						break;
					}
				}
			}
		}
	}
	else
	{
		for (int i = length - 2; i >= 0; i--)
		{
			if (func_first_bigger(value_array[i + 1], value_array[i]))
			{
				for (int j = i + 1; j <= length - 1; j++)
				{
					if (j == length - 1 || (j != length - 1 && func_first_bigger(value_array[i], value_array[j + 1])))
					{
						T swap_temp = value_array[i];
						for (int k = i + 1; k <= j; k++)
						{
							value_array[k - 1] = value_array[k];
						}
						value_array[j] = swap_temp;
						break;
					}
				}
			}
		}
	}

	return OK;
}

template <typename T>
Status XAlgorithm::InsertionSort(XList<T>& value_list, function<bool(const T&, const T&)> func_first_bigger, bool bigger)
{
	return value_list.sort(func_first_bigger, bigger);
}

template <typename T>
Status XAlgorithm::ShellSort(T* const value_array, int length, function<bool(const T&, const T&)> func_first_bigger, bool bigger)
{
	if (length <= 0)
	{
		return ERROR;
	}

	if (length == 1)
	{
		return OK;
	}

	// Use index instead of the original array is because sometimes T coudle be very difficult to dublicate
	int* value_array_index = new int[length];
	for (int i = 0; i < length; i++)
	{
		value_array_index[i] = i;
	}

	// Get half on the first time, then divive it by two, until it becomse one
	int distance = length;
	do
	{
		distance /= 2;
		 
		// Create a group of index arrays
		int** group_index_arrays = new int*[distance];
		int basic_numbers = length / distance;
		int extra_numbers = length % distance;
		for (int i = 0; i < distance; i++)
		{
			group_index_arrays[i] = new int[
			basic_numbers + (i < extra_numbers ? 1 : 0)];
		}

		// Initialization
		for (int i = 0; i < distance; i++)
		{
			int group_i_length = basic_numbers + (i < extra_numbers ? 1 : 0);
			for (int j = 0; j < group_i_length; j++)
			{
				group_index_arrays[i][j] = value_array_index[i + j * distance];
			}
		}

		// Sort group indices with value_array
		for (int i = 0; i < distance; i++)
		{
			InsertionSort<int>(
				group_index_arrays[i],
				basic_numbers + (i < extra_numbers ? 1 : 0),
				[&](const int& a, const int& b) -> bool {
				return func_first_bigger(value_array[a], value_array[b]);
			},
				bigger);
		}
		
		// Set value_array_index
		for (int i = 0; i < distance; i++)
		{
			int group_i_length = basic_numbers + (i < extra_numbers ? 1 : 0);
			for (int j = 0; j < group_i_length; j++)
			{
				value_array_index[i + j * distance] = group_index_arrays[i][j];
			}
		}

		// Uninitialization
		for (int i = 0; i < distance; i++)
		{
			delete[] group_index_arrays[i];
		}
		delete[] group_index_arrays;

	} while (distance != 1);

	// Sort value_array with new value_array_index

	// Helper array
	T** content_of_value_array = new T*[length];
	// Initialize
	for (int i = 0; i < length; i++)
	{
		content_of_value_array[i] = nullptr;
	}

	for (int i = 0; i < length; i++)
	{
		if (i == value_array_index[i])
		{
			continue;
		}

		// Push content to content array
		content_of_value_array[i] = new T(value_array[i]);

		// If the content was pushed out of the array
		if (content_of_value_array[value_array_index[i]] != nullptr)
		{
			value_array[i] = *content_of_value_array[value_array_index[i]];
			delete content_of_value_array[value_array_index[i]];
			content_of_value_array[value_array_index[i]] = nullptr;
			continue;
		}
		
		value_array[i] = value_array[value_array_index[i]];
	}
	delete[] content_of_value_array;

	delete[] value_array_index;
	return OK;
}

template<typename T>
Status XAlgorithm::MergeSort(T * const value_array, int length, function<bool(const T&, const T&)> func_first_bigger, bool bigger)
{
	if (length <= 0)
	{
		return ERROR;
	}

	if (length == 1)
	{
		return OK;
	}

	int* value_array_index = new int[length];

	MergeSort_StackObject* current_stackobject = nullptr;
	XListStack<MergeSort_StackObject*> stack;

	// Initialization
	current_stackobject = new MergeSort_StackObject();
	current_stackobject->start = 0;
	current_stackobject->end = length - 1;
	current_stackobject->excuting_line = 0;
	stack.push(current_stackobject);

	for (int i = 0; i < length; i++)
	{
		value_array_index[i] = i;
	}

	// Start
	while (true)
	{
		int mid = (current_stackobject->start + current_stackobject->end) >> 1;

		if (current_stackobject->excuting_line == 0)
		{
			current_stackobject->excuting_line++;

			if ((mid - current_stackobject->start) >= 1)
			{
				// Start new function
				MergeSort_StackObject* new_stackobject = new MergeSort_StackObject();
				new_stackobject->start = current_stackobject->start;
				new_stackobject->end = mid;
				new_stackobject->excuting_line = 0;

				stack.push(new_stackobject);
				current_stackobject = new_stackobject;
				continue;
			}
		}
		if (current_stackobject->excuting_line == 1)
		{
			current_stackobject->excuting_line++;

			if ((current_stackobject->end - (mid + 1)) >= 1)
			{
				// Start new function
				MergeSort_StackObject* new_stackobject = new MergeSort_StackObject();
				new_stackobject->start = mid + 1;
				new_stackobject->end = current_stackobject->end;
				new_stackobject->excuting_line = 0;

				stack.push(new_stackobject);
				current_stackobject = new_stackobject;
				continue;
			}
		}
		if (current_stackobject->excuting_line == 2)
		{
			current_stackobject->excuting_line++;

			int left_min_index = current_stackobject->start;
			int right_min_index = mid + 1;
			int sorted_length = current_stackobject->end - current_stackobject->start + 1;
			int* sorted_array_index = new int[sorted_length];
			int sorted_array_count = 0;
			while (sorted_array_count < sorted_length)
			{
				if (left_min_index > mid)
				{
					sorted_array_index[sorted_array_count] = value_array_index[right_min_index];
					sorted_array_count++;
					right_min_index++;
				}
				else if (right_min_index > current_stackobject->end)
				{
					sorted_array_index[sorted_array_count] = value_array_index[left_min_index];
					sorted_array_count++;
					left_min_index++;
				}
				else
				{
					if (bigger)
					{
						if (func_first_bigger(
							value_array[value_array_index[left_min_index]],
							value_array[value_array_index[right_min_index]]))
						{
							sorted_array_index[sorted_array_count] = value_array_index[right_min_index];
							sorted_array_count++;
							right_min_index++;
						}
						else if (func_first_bigger(
							value_array[value_array_index[right_min_index]],
							value_array[value_array_index[left_min_index]]))
						{
							sorted_array_index[sorted_array_count] = value_array_index[left_min_index];
							sorted_array_count++;
							left_min_index++;
						}
						else
						{
							sorted_array_index[sorted_array_count] = value_array_index[left_min_index];
							sorted_array_count++;
							left_min_index++;
						}
					}
					else
					{
						if (func_first_bigger(
							value_array[value_array_index[left_min_index]],
							value_array[value_array_index[right_min_index]]))
						{
							sorted_array_index[sorted_array_count] = value_array_index[left_min_index];
							sorted_array_count++;
							left_min_index++;
						}
						else if (func_first_bigger(
							value_array[value_array_index[right_min_index]],
							value_array[value_array_index[left_min_index]]))
						{
							sorted_array_index[sorted_array_count] = value_array_index[right_min_index];
							sorted_array_count++;
							right_min_index++;
						}
						else
						{
							sorted_array_index[sorted_array_count] = value_array_index[left_min_index];
							sorted_array_count++;
							left_min_index++;
						}
					}
				}
			}

			for (int i = 0; i < sorted_length; i++)
			{
				value_array_index[current_stackobject->start + i] = sorted_array_index[i];
			}
			delete[] sorted_array_index;
		}

		delete current_stackobject;
		if (stack.pop(current_stackobject) == ERROR)
		{
			delete[] value_array_index;
			return ERROR;
		}

		if (stack.empty())
		{
			break;
		}

		if (stack.top(current_stackobject) == ERROR)
		{
			delete[] value_array_index;
			return ERROR;
		}
	}

	// Merge sort over

	// Helper array
	T** content_of_value_array = new T*[length];
	// Initialize
	for (int i = 0; i < length; i++)
	{
		content_of_value_array[i] = nullptr;
	}

	for (int i = 0; i < length; i++)
	{
		if (i == value_array_index[i])
		{
			continue;
		}

		// Push content to content array
		content_of_value_array[i] = new T(value_array[i]);

		// If the content was pushed out of the array
		if (content_of_value_array[value_array_index[i]] != nullptr)
		{
			value_array[i] = *content_of_value_array[value_array_index[i]];
			delete content_of_value_array[value_array_index[i]];
			content_of_value_array[value_array_index[i]] = nullptr;
			continue;
		}

		value_array[i] = value_array[value_array_index[i]];
	}
	delete[] content_of_value_array;

	// Uninitialization
	delete[] value_array_index;

	return OK;
}

template<typename T>
Status XAlgorithm::HalfFind(T * const value_array, int length,
	const T& to_search,
	function<bool(const T&, const T&)> func_first_bigger,
	function<bool(const T&, const T&)> func_same,
	int & out_position)
{
	if (length <= 0)
	{
		return ERROR;
	}

	int start = 0;
	int end = length - 1;
	int half = (start + end) >> 1;
	while (!func_same(to_search, value_array[half]))
	{
		if (func_first_bigger(to_search, value_array[half]))
		{
			if (start == end || half == end)
			{
				out_position = half;
				return WARNING;
			}

			start = half + 1;
		}
		else if (func_first_bigger(value_array[half], to_search))
		{
			if (start == end || half == start)
			{
				out_position = half - 1;
				return WARNING;
			}

			end = half - 1;
		}
		
		half = (start + end) >> 1;
	}

	out_position = half;
	return OK;
}

template<typename T>
Status XAlgorithm::GetLIS(T * const value_array, int length,
	function<bool(const T&, const T&)> func_first_bigger,
	function<bool(const T&, const T&)> func_same,
	XList<T>& out_lists)
{
	if (length <= 0)
	{
		return ERROR;
	}

	unsigned int* smallest_array_index = new unsigned int[length];
	XList<Galn*>** answer_lists = new XList<Galn*>*[length];
	for (int i = 0; i < length; i++)
	{
		answer_lists[i] = nullptr;
	}
	int longest_length;
	bool zero_index_changed = true;

	// Initialization
	smallest_array_index[0] = 0;
	longest_length = 1;
	answer_lists[0] = new XList<Galn*>;
	Galn* temp = new Galn();
	temp->index = 0;
	answer_lists[0]->push_back(temp);
	for (int i = 1; i < length; i++)
	{
		int out_position;
		Status return_status = XAlgorithm::HalfFind<unsigned int>(
			smallest_array_index,
			longest_length,
			i,
			[&](const unsigned int& a, const unsigned int& b) -> bool {
			return func_first_bigger(value_array[a], value_array[b]);
		},
			[&](const unsigned int& a, const unsigned int& b) -> bool {
			return func_same(value_array[a], value_array[b]);
		},
			out_position);

		if (return_status == WARNING)
		{
			int put_position = out_position + 1;
			smallest_array_index[put_position] = i;
			if (put_position == longest_length)
			{
				longest_length++;
			}

			// Push the galn node into answer_lists and connect with the previous's last node 
			if (answer_lists[put_position] == nullptr)
			{
				answer_lists[put_position] = new XList<Galn*>();
			}
			Galn* temp_galn = new Galn();
			temp_galn->index = i;
			answer_lists[put_position]->push_back(temp_galn);
			if (put_position != 0)
			{
				Galn* out_galn;
				answer_lists[put_position - 1]->get_back(out_galn);
				out_galn->nexts.push_back(temp_galn);
			}
		}
	}

	// Load from answer_list
	XList<unsigned int> longest_index_list;
	if (answer_lists[0] != nullptr) 
	{
		for (auto i = answer_lists[0]->begin(); i != answer_lists[0]->end(); i++)
		{
			XList<unsigned int> out_value;
			GetLIS_GetLongestIndexList(*i, out_value);

			if (out_value.length() > longest_index_list.length())
			{
				longest_index_list = out_value;
			}
		}
	}

	// Return values
	out_lists.clear();
	longest_index_list.enumerate_reverse_readonly([&](const unsigned int & data) -> void {
		out_lists.push(value_array[data]);
	});

	// Release pointers;
	for (int i = 0; i < length; i++)
	{
		if (answer_lists[i] != nullptr)
		{
			answer_lists[i]->enumerate_readonly([](Galn *const &data) -> void {
				delete data;
			});
			delete answer_lists[i];
		}
		else
		{
			break;
		}
	}
	delete[] smallest_array_index;
	delete[] answer_lists;

	return OK;
}
