#include "XAlgorithm.h"

XAlgorithm::XAlgorithm()
{

}


XAlgorithm::~XAlgorithm()
{

}

Status XAlgorithm::ClosestPair(const XVector2* const array, int length, int& out_one_index, int& out_two_index)
{
	if (length <= 1)
	{
		return ERROR;
	}
	else if (length == 2)
	{
		out_one_index = 0;
		out_two_index = 1;
		return OK;
	}

	// Init indices
	int* array_indices = new int[length];
	for (int i = 0; i < length; i++)
	{
		array_indices[i] = i;
	}

	// Sort array
	QuickSort<int>(array_indices, length, [&](const int& a, const int& b) -> bool {
		return (int)array[a].x >(int)array[b].x;
	},
		true);

	// Start divide and conquer with heap
	ClosestPair_StackObject* current_stackobject = nullptr;
	XListStack<ClosestPair_StackObject*> stack;

	// Init stack
	current_stackobject = new ClosestPair_StackObject();
	current_stackobject->start = 0;
	current_stackobject->end = length - 1;
	current_stackobject->excuting_line = 0;
	stack.push(current_stackobject);

	// A return value
	ClosestPair_Pair return_value;

	// Start
	while (true)
	{
		int mid_index = (current_stackobject->start + current_stackobject->end) / 2;
		float mid = (array[array_indices[mid_index]].x + array[array_indices[mid_index + 1]].x) / 2;
		int left_count = mid_index - current_stackobject->start + 1;
		int right_count = current_stackobject->end - current_stackobject->start - left_count + 1;

		if (current_stackobject->excuting_line == 0)
		{
			current_stackobject->excuting_line++;

			//Left
			if (left_count <= 2)
			{
				if (left_count == 0)
				{

				}
				else if (left_count == 1)
				{
					current_stackobject->left_min_pair.point_a_index = array_indices[current_stackobject->start];
				}
				else if (left_count == 2)
				{
					current_stackobject->left_min_pair.point_a_index = array_indices[current_stackobject->start];
					current_stackobject->left_min_pair.point_b_index = array_indices[current_stackobject->start + 1];
				}
			}
			else
			{
				// Starting a new function
				ClosestPair_StackObject* new_object = new ClosestPair_StackObject();
				new_object->start = current_stackobject->start;
				new_object->end = current_stackobject->start + left_count - 1;
				new_object->excuting_line = 0;

				// Link return
				current_stackobject->return_to = &(current_stackobject->left_min_pair);

				current_stackobject = new_object;
				stack.push(current_stackobject);
				continue;
			}
		}
		if (current_stackobject->excuting_line == 1)
		{
			current_stackobject->excuting_line++;

			// Right
			if (right_count <= 2)
			{
				if (right_count == 0)
				{

				}
				else if (right_count == 1)
				{
					current_stackobject->right_min_pair.point_a_index = array_indices[current_stackobject->end];
				}
				else if (right_count == 2)
				{
					current_stackobject->right_min_pair.point_a_index = array_indices[current_stackobject->end];
					current_stackobject->right_min_pair.point_b_index = array_indices[current_stackobject->end - 1];
				}
			}
			else
			{
				// Starting a new function
				ClosestPair_StackObject* new_object = new ClosestPair_StackObject();
				new_object->start = current_stackobject->end - right_count + 1;
				new_object->end = current_stackobject->end;
				new_object->excuting_line = 0;

				// Link return
				current_stackobject->return_to = &(current_stackobject->right_min_pair);

				current_stackobject = new_object;
				stack.push(current_stackobject);
				continue;
			}
		}
		if (current_stackobject->excuting_line == 2)
		{
			current_stackobject->excuting_line++;

			if (left_count == 0)
			{
				return_value = current_stackobject->right_min_pair;
				current_stackobject->excuting_line = -1;
			}
			else if (right_count == 0)
			{
				return_value = current_stackobject->left_min_pair;
				current_stackobject->excuting_line = -1;
			}
			else
			{
				float closest_distsqr = -1;

				if (left_count == 1)
				{
					return_value = current_stackobject->right_min_pair;
					closest_distsqr = XVector2::DistanceSqr(
						array[current_stackobject->right_min_pair.point_a_index],
						array[current_stackobject->right_min_pair.point_b_index]);

				}
				else if (right_count == 1)
				{
					return_value = current_stackobject->left_min_pair;
					closest_distsqr = XVector2::DistanceSqr(
						array[current_stackobject->left_min_pair.point_a_index],
						array[current_stackobject->left_min_pair.point_b_index]);

				}
				else
				{
					// If left closest is smaller
					float left_closest = XVector2::DistanceSqr(
						array[current_stackobject->left_min_pair.point_a_index],
						array[current_stackobject->left_min_pair.point_b_index]);
					float right_closest = XVector2::DistanceSqr(
						array[current_stackobject->right_min_pair.point_a_index],
						array[current_stackobject->right_min_pair.point_b_index]);
					if (left_closest < right_closest)
					{
						return_value = current_stackobject->left_min_pair;
						closest_distsqr = left_closest;
					}
					else
					{
						return_value = current_stackobject->right_min_pair;
						closest_distsqr = right_closest;
					}
				}

				float closest_dist = sqrt(closest_distsqr);
				float left_x = mid - closest_dist;
				float right_x = mid + closest_dist;
				int s_lx_count = ClosestPair_GetLeftPointsCount(
					array,
					array_indices + current_stackobject->start,
					current_stackobject->end - current_stackobject->start + 1,
					left_x);
				int rx_e_count = ClosestPair_GetRightPointsCount(
					array,
					array_indices + current_stackobject->start + left_count,
					current_stackobject->end - current_stackobject->start + 1 - left_count,
					right_x);

				// If there are comparisons to make
				if (s_lx_count != left_count &&
					rx_e_count != right_count)
				{
					int lx_m_start = mid_index - (left_count - s_lx_count) + 1;
					int lx_m_end = mid_index;
					int m_rx_start = mid_index + 1;
					int m_rx_end = mid_index + (right_count - rx_e_count);

					for (int i = lx_m_start; i <= lx_m_end; i++)
					{
						for (int j = m_rx_start; j <= m_rx_end; j++)
						{
							// Test if index j is possible to be the closest
							if (array[array_indices[j]].y > array[array_indices[i]].y + closest_dist ||
								array[array_indices[j]].y < array[array_indices[i]].y - closest_dist)
							{
								// Not possible
								continue;
							}

							// Possible
							// The below codes will run 6 times max for each i
							float temp_closest_distsqr = XVector2::DistanceSqr(array[array_indices[i]], array[array_indices[j]]);
							if (temp_closest_distsqr < closest_distsqr)
							{
								return_value.point_a_index = array_indices[i];
								return_value.point_b_index = array_indices[j];
								closest_distsqr = temp_closest_distsqr;
								closest_dist = sqrt(closest_distsqr);
							}
						}
					}

					current_stackobject->excuting_line = -1;
				}
			}
		}

		delete current_stackobject;
		if (stack.pop(current_stackobject) == ERROR)
		{
			return ERROR;
		}

		if (stack.empty())
		{
			// Function over
			break;
		}

		// Load from stack top
		if (stack.top(current_stackobject) == ERROR)
		{
			return ERROR;
		}

		// Return value
		*(current_stackobject->return_to) = return_value;

		// Clear return value
		return_value.point_a_index = -1;
		return_value.point_b_index = -1;
	}

	delete[] array_indices;

	// Fuction over without error
	out_one_index = return_value.point_a_index;
	out_two_index = return_value.point_b_index;
	return OK;
}

inline int XAlgorithm::ClosestPair_GetLeftPointsCount(const XVector2 * const point_array, const int * const pointi_array, int length, float middle)
{
	for (int i = 0; i < length; i++)
	{
		if (point_array[pointi_array[i]].x > middle)
		{
			return i;
		}
	}

	return 0;
}

inline int XAlgorithm::ClosestPair_GetRightPointsCount(const XVector2 * const point_array, const int * const pointi_array, int length, float middle)
{
	for (int i = 0; i < length; i++)
	{
		if (point_array[pointi_array[i]].x > middle)
		{
			return length - i;
		}
	}

	return 0;
}

Status XAlgorithm::ClosestPairN2(const XVector2 * const array, int length, int & out_one_index, int & out_two_index)
{
	if (length <= 1)
	{
		return ERROR;
	}

	if (length == 2)
	{
		out_one_index = 0;
		out_two_index = 1;
		return OK;
	}

	float closest_sqr = XVector2::DistanceSqr(array[0], array[1]);
	int point_a_index = 0;
	int point_b_index = 1;
	for (int i = 0; i < length - 1; i++)
	{
		for (int j = i + 1; j < length; j++)
		{
			float temp_sqr = XVector2::DistanceSqr(array[i], array[j]);
			if (temp_sqr < closest_sqr)
			{
				closest_sqr = temp_sqr;
				point_a_index = i;
				point_b_index = j;
			}
		}
	}

	out_one_index = point_a_index;
	out_two_index = point_b_index;

	return OK;
}

Status XAlgorithm::RadixSort(unsigned int * const value_array, int length, bool bigger)
{
	if (length <= 0)
	{
		return ERROR;
	}

	if (length == 1)
	{
		return OK;
	}

	XList<unsigned int> sort_buckets[10];

	unsigned int max_number = 0;
	for (int i = 0; i < length; i++)
	{
		if (value_array[i] > max_number)
		{
			max_number = value_array[i];
		}
	}

	// Calculate how many times should we sort
	unsigned int sort_times = 0;
	for (int i = 1; i <= 10; i++)
	{
		int flag = 1;
		for (int j = 1; j < i; j++)
		{
			flag *= 10;
		}

		if ((max_number / flag) != 0)
		{
			sort_times = i;
		}
		else
		{
			break;
		}
	}

	// Sorting
	for (int i = 0; i < sort_times; i++)
	{
		// Push datas into sort buckets
		for (int j = 0; j < length; j++)
		{
			sort_buckets[GetDigit(value_array[j], i)].push_back(value_array[j]);
		}

		// Sort buckets
		for (int i = 0; i < 10; i++)
		{
			InsertionSort<unsigned int>(sort_buckets[i], [](const unsigned int & one, const unsigned int & two) -> bool {
				return one > two;
			},
				bigger);
		}

		// Load from buckets
		if (bigger)
		{
			int already_loaded_count = 0;
			for (int i = 0; i < 10; i++)
			{
				sort_buckets[i].enumerate_readonly([&](const unsigned int& data) -> void {
					value_array[already_loaded_count] = data;
					already_loaded_count++;
				});
			}
		}
		else
		{
			int already_loaded_count = 0;
			for (int i = 9; i >= 0; i--)
			{
				sort_buckets[i].enumerate_readonly([&](const unsigned int& data) -> void {
					value_array[already_loaded_count] = data;
					already_loaded_count++;
				});
			}
		}

		// Clear buckets
		for (int i = 0; i < 10; i++)
		{
			sort_buckets[i].clear();
		}
	}

	return OK;
}

Status XAlgorithm::GetShortestEditingLength(
	const xstring& a,
	const xstring& b,
	const GetShortestEditingLength_CostStruct&  cost,
	XList<XAlgorithm::GetShortestEditingLength_EditOperations>& out_operations,
	int& out_length)
{
	// Check before run
	for (int i = 0; i < GetShortestEditingLength_EditOperations_Numbers; i++)
	{
		if (cost.costs[i] < 0)
		{
			return ERROR;
		}
	}

	if (cost.costs[EOperations::Replace] <= cost.costs[EOperations::Copy] ||
		cost.costs[EOperations::Delete] + cost.costs[EOperations::Insert] <= cost.costs[EOperations::Copy] ||
		cost.costs[EOperations::Delete] + cost.costs[EOperations::Insert] <= cost.costs[EOperations::Replace] ||
		cost.costs[EOperations::Twiddle] >= (cost.costs[EOperations::Replace] * 2))
	{
		return ERROR;
	}

	int a_length = a.length();
	int b_length = b.length();

	if (a_length == 0 && b_length == 0)
	{
		out_length = 0;
		out_operations.clear();
		return OK;
	}

	int rows = a_length + 1;
	int cols = b_length + 1;
	int** shortestLengthMatrix = new int*[rows];
	for (int i = 0; i < rows; i++)
	{
		shortestLengthMatrix[i] = new int[cols];
	}

	XList<EOperations>** operationMatrix = new XList<EOperations>*[rows];
	for (int i = 0; i < rows; i++)
	{
		operationMatrix[i] = new XList<EOperations>[cols];
	}

	// Init
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			shortestLengthMatrix[i][j] = -1;
		}
	}

	// Recursive start
	XListStack<GetShortestEditingLength_StackObject*> stack;
	GetShortestEditingLength_StackObject* current_stackobject = nullptr;

	// A return value
	int return_value;

	// Init stack
	current_stackobject = new GetShortestEditingLength_StackObject();
	current_stackobject->i = 0;
	current_stackobject->j = 0;
	current_stackobject->excuting_line = 0;
	stack.push(current_stackobject);

	while (true)
	{
		if (current_stackobject->excuting_line == 0)
		{
			current_stackobject->excuting_line++;

			if (shortestLengthMatrix[current_stackobject->i][current_stackobject->j] != -1)
			{
				return_value = shortestLengthMatrix[current_stackobject->i][current_stackobject->j];
				current_stackobject->excuting_line = -1;
			}
		}
		if (current_stackobject->excuting_line == 1)
		{
			current_stackobject->excuting_line++;

			if (current_stackobject->i == a_length - 1 &&
				current_stackobject->j == b_length - 1)
			{
				if (a[current_stackobject->i] == b[current_stackobject->j])
				{
					return_value = cost.costs[EOperations::Copy];
					operationMatrix[current_stackobject->i][current_stackobject->j].push_back(EOperations::Copy);
					shortestLengthMatrix[current_stackobject->i][current_stackobject->j] = cost.costs[EOperations::Copy];
					current_stackobject->excuting_line = -1;
				}
				else
				{
					return_value = cost.costs[EOperations::Replace];
					operationMatrix[current_stackobject->i][current_stackobject->j].push_back(EOperations::Replace);
					shortestLengthMatrix[current_stackobject->i][current_stackobject->j] = cost.costs[EOperations::Replace];
					current_stackobject->excuting_line = -1;
				}
			}
			else if (current_stackobject->i == a_length)
			{
				int insert_times = b_length - current_stackobject->j;
				return_value = cost.costs[EOperations::Insert] * insert_times;
				shortestLengthMatrix[current_stackobject->i][current_stackobject->j] = return_value;
				for (int i = 0; i < insert_times; i++)
				{
					operationMatrix[current_stackobject->i][current_stackobject->j].push_back(EOperations::Insert);
				}
				current_stackobject->excuting_line = -1;
			}
			else if (current_stackobject->j == b_length)
			{
				int delete_times = a_length - current_stackobject->i;
				int delete_cost = cost.costs[EOperations::Delete] * delete_times;
				if (delete_cost < cost.costs[EOperations::Kill])
				{
					return_value = delete_cost;
					shortestLengthMatrix[current_stackobject->i][current_stackobject->j] = delete_cost;
					for (int i = 0; i < delete_times; i++)
					{
						operationMatrix[current_stackobject->i][current_stackobject->j].push_back(EOperations::Delete);
					}
				}
				else
				{
					return_value = cost.costs[EOperations::Kill];
					shortestLengthMatrix[current_stackobject->i][current_stackobject->j] = cost.costs[EOperations::Kill];
					operationMatrix[current_stackobject->i][current_stackobject->j].push_back(EOperations::Kill);
				}
				current_stackobject->excuting_line = -1;
			}
			else if (current_stackobject->i == a_length - 2 &&
				current_stackobject->j == b_length - 2)
			{
				if (a[current_stackobject->i] == b[current_stackobject->j + 1] &&
					a[current_stackobject->i + 1] == b[current_stackobject->j])
				{
					return_value = cost.costs[EOperations::Twiddle];
					operationMatrix[current_stackobject->i][current_stackobject->j].push_back(EOperations::Twiddle);
					shortestLengthMatrix[current_stackobject->i][current_stackobject->j] = cost.costs[EOperations::Twiddle];
					current_stackobject->excuting_line = -1;
				}
			}
		}
		if (current_stackobject->excuting_line == 2)
		{
			current_stackobject->excuting_line++;

			// Copy or replace
			if (a[current_stackobject->i] == b[current_stackobject->j])
			{
				auto new_stackobject = new GetShortestEditingLength_StackObject();
				new_stackobject->i = current_stackobject->i + 1;
				new_stackobject->j = current_stackobject->j + 1;
				new_stackobject->excuting_line = 0;
				current_stackobject->return_copy_replace = cost.costs[EOperations::Copy];
				current_stackobject->return_to = &(current_stackobject->return_copy_replace);
				current_stackobject->copied = true;

				current_stackobject = new_stackobject;
				stack.push(new_stackobject);
				continue;
			}
			else
			{
				auto new_stackobject = new GetShortestEditingLength_StackObject();
				new_stackobject->i = current_stackobject->i + 1;
				new_stackobject->j = current_stackobject->j + 1;
				new_stackobject->excuting_line = 0;
				current_stackobject->return_copy_replace = cost.costs[EOperations::Replace];
				current_stackobject->return_to = &(current_stackobject->return_copy_replace);

				current_stackobject = new_stackobject;
				stack.push(new_stackobject);
				continue;
			}
		}
		if (current_stackobject->excuting_line == 3)
		{
			current_stackobject->excuting_line++;

			// Delete
			auto new_stackobject = new GetShortestEditingLength_StackObject();
			new_stackobject->i = current_stackobject->i + 1;
			new_stackobject->j = current_stackobject->j;
			new_stackobject->excuting_line = 0;
			current_stackobject->return_delete = cost.costs[EOperations::Delete];
			current_stackobject->return_to = &(current_stackobject->return_delete);

			current_stackobject = new_stackobject;
			stack.push(new_stackobject);
			continue;
		}
		if (current_stackobject->excuting_line == 4)
		{
			current_stackobject->excuting_line++;

			// Insert
			auto new_stackobject = new GetShortestEditingLength_StackObject();
			new_stackobject->i = current_stackobject->i;
			new_stackobject->j = current_stackobject->j + 1;
			new_stackobject->excuting_line = 0;
			current_stackobject->return_insert = cost.costs[EOperations::Insert];
			current_stackobject->return_to = &(current_stackobject->return_insert);

			current_stackobject = new_stackobject;
			stack.push(new_stackobject);
			continue;
		}
		if (current_stackobject->excuting_line == 5)
		{
			current_stackobject->excuting_line++;

			// Twiddle
			if (current_stackobject->i < a_length - 1 &&
				current_stackobject->j < b_length - 1)
			{
				auto new_stackobject = new GetShortestEditingLength_StackObject();
				new_stackobject->i = current_stackobject->i + 2;
				new_stackobject->j = current_stackobject->j + 2;
				new_stackobject->excuting_line = 0;
				current_stackobject->return_twiddle = cost.costs[EOperations::Twiddle];
				current_stackobject->return_to = &(current_stackobject->return_twiddle);

				current_stackobject = new_stackobject;
				stack.push(new_stackobject);
				continue;
			}
		}
		if (current_stackobject->excuting_line == 6)
		{
			current_stackobject->excuting_line++;

			EOperations smallest_oper = EOperations::Copy;
			int smallest = current_stackobject->return_copy_replace;
			if (current_stackobject->return_delete < smallest)
			{
				smallest = current_stackobject->return_delete;
				smallest_oper = EOperations::Delete;
			}
			if (current_stackobject->return_insert < smallest)
			{
				smallest = current_stackobject->return_insert;
				smallest_oper = EOperations::Insert;
			}

			if (smallest_oper == EOperations::Copy)
			{
				operationMatrix[current_stackobject->i][current_stackobject->j] =
					operationMatrix[current_stackobject->i + 1][current_stackobject->j + 1];
				if (current_stackobject->copied)
				{
					operationMatrix[current_stackobject->i][current_stackobject->j].push_back(EOperations::Copy);
				}
				else
				{
					operationMatrix[current_stackobject->i][current_stackobject->j].push_back(EOperations::Replace);
				}
			}
			else if (smallest_oper == EOperations::Delete)
			{
				operationMatrix[current_stackobject->i][current_stackobject->j] =
					operationMatrix[current_stackobject->i + 1][current_stackobject->j];
				operationMatrix[current_stackobject->i][current_stackobject->j].push_back(EOperations::Delete);
			}
			else if (smallest_oper == EOperations::Insert)
			{
				operationMatrix[current_stackobject->i][current_stackobject->j] =
					operationMatrix[current_stackobject->i][current_stackobject->j + 1];
				operationMatrix[current_stackobject->i][current_stackobject->j].push_back(EOperations::Insert);
			}

			return_value = smallest;
			shortestLengthMatrix[current_stackobject->i][current_stackobject->j] = smallest;
		}

		delete current_stackobject;
		stack.pop(current_stackobject);

		if (stack.empty())
		{
			break;
		}

		stack.top(current_stackobject);
		*(current_stackobject->return_to) += return_value;

		// Clear return value
		return_value = -1;
	}

	out_length = return_value;
	out_operations = operationMatrix[0][0];

	// Release memory
	for (int i = 0; i < rows; i++)
	{
		delete[] shortestLengthMatrix[i];
	}
	delete[] shortestLengthMatrix;
	for (int i = 0; i < rows; i++)
	{
		delete[] operationMatrix[i];
	}
	delete[] operationMatrix;
	return OK;
}

unsigned int XAlgorithm::GetDigit(int value, int digit)
{
	if (value < 0)
	{
		value = -value;
	}

	int flag = 1;
	for (int j = 0; j < digit; j++)
	{
		flag *= 10;
	}

	return (unsigned int)(value % (flag * 10)) / flag;
}

unsigned int XAlgorithm::GetDigit(unsigned int value, int digit)
{
	int flag = 1;
	for (int j = 0; j < digit; j++)
	{
		flag *= 10;
	}

	return (unsigned int)(value % (flag * 10)) / flag;
}
