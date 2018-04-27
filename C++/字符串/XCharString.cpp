#include "XCharString.h"
#include "XList.h"

XCharString::XCharString(char* value, int string_length, int string_mem_length)
{
	content_string = value;
	this->string_length = string_length;
	this->string_mem_length = string_mem_length;
}

XCharString::XCharString()
{
	content_string = new char[DEFAULT_CHAR_ARRAY_LENGTH];
	content_string[0] = 0;
	string_mem_length = 1;
	string_length = 0;
}

XCharString::XCharString(int length, bool clear_mem)
{
	content_string = new char[length + 1];

	if (content_string == nullptr)
	{
		cerr << "Array too large, exiting...." << endl;
		exit(1);
	}

	if (clear_mem)
	{
		memset(content_string, 0, length + 1);
	}
	else
	{
		content_string[0] = 0;
	}
	string_mem_length = length + 1;
	string_length = 0;
}

XCharString::XCharString(const XCharString & other)
{
	content_string = new char[other.string_mem_length];
	memcpy(content_string, other.content_string, other.string_mem_length);
	string_mem_length = other.string_mem_length;
	string_length = other.string_length;
}

XCharString::XCharString(XCharString && local_other)
{
	content_string = nullptr;
	l_assign(local_other);
}

XCharString::XCharString(const char * value)
{
	int value_mem_length = strlen(value) + 1;
	content_string = new char[value_mem_length];
	memcpy(content_string, value, value_mem_length);
	string_mem_length = value_mem_length;
	string_length = value_mem_length - 1;
}

XCharString::XCharString(const char value)
{
	content_string = new char[2];
	content_string[0] = value;
	content_string[1] = 0;
	string_length = 1;
	string_mem_length = 2;
}

XCharString::XCharString(const string & value)
{
	int value_mem_length = value.length() + 1;
	content_string = new char[value_mem_length];
	memcpy(content_string, value.c_str(), value_mem_length);
	string_mem_length = value_mem_length;
	string_length = value_mem_length - 1;
}

XCharString::~XCharString()
{
	if (content_string != nullptr)
	{
		delete[] content_string;
	}
}

int XCharString::size() const
{
	return string_mem_length;
}

int XCharString::length() const
{
	return string_length;
}

int XCharString::count_times(const XCharString & to_search) const
{
	int result = 0;

	int pos = indexOf(to_search);
	while (pos != -1)
	{
		result++;
		pos = indexOf(to_search, pos + to_search.length());
	}

	return result;
}

int XCharString::count_times(const char & to_search) const
{
	int result = 0;

	for (int i = 0; i < string_length; i++)
	{
		if (content_string[i] == to_search)
		{
			result++;
		}
	}

	return 0;
}

int XCharString::indexOf(const XCharString & to_search, int begin_pos, SearchAlgorithm algorithm) const
{
	if (begin_pos < 0 || begin_pos + to_search.string_length >= string_length)
	{
		return -1;
	}

	switch (algorithm)
	{
	case SearchAlgorithm::String_H:
	{
		char* pos = strstr(content_string + begin_pos, to_search.c_str());
		if (pos == nullptr)
		{
			return -1;
		}
		return pos - content_string;
	}
	case SearchAlgorithm::Brute_Force:
	{
		int end_pos = string_length - to_search.string_length;
		for (int i = begin_pos; i <= end_pos; i++)
		{
			int j = 0;
			for (; j < to_search.string_length; j++)
			{
				if (content_string[i + j] != to_search.content_string[j])
				{
					break;
				}
			}

			if (j == to_search.string_length)
			{
				return i;
			}
		}
		return -1;
	}
	case SearchAlgorithm::KMP:
	{
		unsigned short* next = new unsigned short[to_search.string_length];
		unsigned short* next_val = new unsigned short[to_search.string_length];

		// Init
		next[0] = next_val[0] = 0;
		next[1] = next_val[1] = 1;

		// Calculate next
		int k = 0;
		int cal_length = to_search.string_length - 1;
		for (int i = 1; i < cal_length;)
		{
			if (k == -1 || to_search[k] == to_search[i])
			{
				i++;
				k++;
				next[i] = k + 1;
			}
			else
			{
				k = next[k] - 1;
			}
		}

		// Calculate next_val
		for (int i = 2; i < to_search.string_length; i++)
		{
			if (to_search[i] == to_search[next[i] - 1])
			{
				next_val[i] = next[next[i] - 1];
			}
			else
			{
				next_val[i] = next[i];
			}
		}

		// UnInit
		delete[] next;

		int result = -1;

		int end_pos = string_length - to_search.string_length;
		for (int i = begin_pos; i <= end_pos;)
		{
			int j = 0;
			for (; j < to_search.string_length; j++)
			{
				if (content_string[i + j] != to_search.content_string[j])
				{
					break;
				}
			}

			if (j == to_search.string_length)
			{
				result = i;
				break;
			}

			i += j + 1 - next_val[j];
		}
		
		// UnInit
		delete[] next_val;

		return result;
	}
	case SearchAlgorithm::BM:
	{
		short* bmBc = new short[ASCIICODE_TOTAL_LENGTH];

		// Calculate bmBc
		for (int i = 0; i < ASCIICODE_TOTAL_LENGTH; i++)
		{
			bmBc[i] = -1;
		}
		for (int i = 0; i < to_search.string_length; i++)
		{
			bmBc[(unsigned int)to_search[i]] = i;
		}

		unsigned short* Osuff = new unsigned short[to_search.string_length];

		// Calculate Osuff
		Osuff[to_search.string_length - 1] = to_search.string_length;
		for (int i = to_search.string_length - 2; i >= 0; i--)
		{
			int j = i;
			for (; j >= 0; j--)
			{
				if (to_search[j] != to_search[to_search.string_length - 1 - i + j])
				{
					Osuff[i] = i - j;
					break;
				}
			}

			if (j == -1)
			{
				Osuff[i] = i + 1;
			}
		}

		unsigned short* bmGs = new unsigned short[to_search.string_length];

		// Calculate bmGs
		for (int i = 0; i < to_search.string_length; i++)
		{
			bmGs[i] = to_search.string_length;
		}

		int k = 0;
		for (int i = to_search.string_length - 1; i >= 0; i--)
		{
			if (Osuff[i] == i + 1)
			{
				for (; k < to_search.string_length - 1 - i; k++)
				{
					if (bmGs[i] == to_search.string_length)
					{
						bmGs[i] = i;
					}
				}
			}
		}

		for (int i = 0; i <= to_search.string_length - 2; i++)
		{
			k = to_search.string_length - 1 - Osuff[i];
			bmGs[k] = i;
		}

		// Search
		int result = -1;

		int index = begin_pos;
		int end_pos = string_length - to_search.string_length;
		while (index <= end_pos)
		{
			k = to_search.string_length - 1;
			while (k >= 0 && to_search[k] == operator[](index + k))
			{
				k--;
			}

			if (k == -1)
			{
				result = index;
				break;
			}

			/*index += (index + to_search.string_length < string_length) ?
				to_search.string_length - bmBc[operator[](index + to_search.string_length)] :
				1;*/
				
			int a = k - bmBc[operator[](index + k)];
			int b = k - bmGs[k];
			index += a > b ? a : b;
		}

		delete[] bmBc;
		delete[] Osuff;
		delete[] bmGs;

		return result;
	}
	break;
	default:
		break;
	}
}

int XCharString::indexOf(const char & to_search, int begin_pos) const
{
	if (begin_pos < 0 || begin_pos >= string_length)
	{
		return -1;
	}

	for (int i = begin_pos; i < string_length; i++)
	{
		if (*(content_string + i) == to_search)
		{
			return i;
		}
	}

	return -1;
}

int XCharString::indexOf_Prev(const char & to_search, int begin_pos) const
{
	if (begin_pos < 0 || begin_pos >= string_length)
	{
		return -1;
	}

	for (; begin_pos >= 0; begin_pos--)
	{
		if (content_string[begin_pos] == to_search)
		{
			return begin_pos;
		}
	}

	return -1;
}

//[start, \0]
XCharString XCharString::substring(int start) const
{
	if (start < 0 || start >= string_mem_length || start >= string_length)
	{
		return XCharString();
	}

	XCharString result(string_length - start, false);
	result.string_length = string_length - start;
	memcpy(result.content_string, content_string + start, string_length - start + 1);

	return result;
}

//[start, end]
XCharString XCharString::substring(int start, int end) const
{
	if (start > end || start < 0 || end >= string_length)
	{
		return XCharString();
	}

	XCharString result(end - start + 1, false);
	result.string_length = end - start + 1;
	memcpy(result.content_string, content_string + start, end - start + 1);
	result.content_string[end - start + 1] = 0;

	return result;
}

XCharString& XCharString::concat(const XCharString & other)
{
	int string_t_length = string_length + other.string_length + 1;
	if (string_mem_length < string_t_length)
	{
		char* temp = new char[string_t_length];
		memcpy(temp, content_string, string_length + 1);
		delete[] content_string;
		content_string = temp;
		string_mem_length = string_t_length;
	}

	memcpy(content_string + string_length, other.content_string, other.string_length + 1);
	string_length = string_length + other.string_length;

	return *this;
}

XCharString& XCharString::concat(const char * other)
{
	int other_length = strlen(other);
	int string_t_length = string_length + other_length + 1;
	if (string_mem_length < string_t_length)
	{
		char* temp = new char[string_t_length];
		memcpy(temp, content_string, string_length);
		delete[] content_string;
		content_string = temp;
		string_mem_length = string_t_length;
	}

	memcpy(content_string + string_length, other, other_length + 1);
	string_length = string_length + other_length;

	return *this;
}

XCharString XCharString::Html_UnicodeString_to_XCharString()
{
	//用来存储所有HtmlUnicode的总长度
	int wchars_length = 0;
	//用来存储所有宽字符的编码
	XList<wchar_t> wchars_int;
	//有多少个非Unicode字符
	int chars_count = 0;
	
	int pos = indexOf('&');

	if (pos == -1)
	{
		return *this;
	}
	else
	{
		//用来获取开头部分的非Unicode字符
		for (int i = 0; i != pos; i++)
		{
			wchars_int.push((int)content_string[i]);
		}

		while (true)
		{
			int end_pos;
			//存下'#''&'和';'
			if (content_string[pos + 1] == '#')
			{
				wchars_length += 3;
				int int_length = 0;
				int num_pos = pos + 2;
				char cnum = content_string[num_pos];
				int get_num = 0;
				//string to int
				while (cnum >= '0' && cnum <= '9')
				{
					get_num = get_num * 10 + (cnum - '0');
					int_length += 1;
					num_pos++;
					cnum = content_string[num_pos];
				}

				wchars_length += int_length;

				wchars_int.push(get_num);

				end_pos = pos + 3 + int_length;
			}
			else
			{
				end_pos = indexOf(';', pos);
				if (substring(pos + 1, end_pos - 1) == "nbsp")
				{
					wchars_length += 6;
					wchars_int.push((int)' ');
				}

				end_pos++;
				chars_count++;
			}

			
			pos = indexOf('&', end_pos);

			if (pos == -1)
			{
				//获取结尾的非Unicode字符
				for (int i = end_pos; i != string_length; i++)
				{
					wchars_int.push((int)content_string[i]);
				}
				break;
			}
			else
			{
				//获取中间的非Unicode字符
				for (int i = end_pos; i != pos; i++)
				{
					wchars_int.push((int)content_string[i]);
				}
			}
		}
	}

	//开始转换
	int wchars_chars_counts = wchars_int.length();
	wchar_t* converted_wchars = new wchar_t[wchars_chars_counts + 1];

	int i = 0;
	wchars_int.enumerate([&](wchar_t& data) -> void {
		converted_wchars[i] = data;
		i++;
	});

	converted_wchars[i] = 0;

	chars_count += string_length - wchars_length;
	//加上一个安全位。
	int result_mem_length = (wchars_chars_counts - chars_count) * 2 + chars_count + 1 + WCHAR_CHAR_TRANSFROM_EXTRA_BYTE;
	char* result = new char[result_mem_length];

	int convered_length = WCharStr_to_CharStr(result, converted_wchars, wchars_chars_counts - (chars_count / 2));

	delete[] converted_wchars;

	return XCharString(result, convered_length - 1, result_mem_length);
}

void XCharString::Html_Pop_Markups()
{
	return;

	int start_pos = indexOf('<');
	if (start_pos == -1)
	{
		return;
	}

	int end_pos = indexOf('>', start_pos + 1);
	while (content_string[end_pos + 1] == '<')
	{
		end_pos = indexOf('>', end_pos + 2);
	}

	pop_out(start_pos, end_pos);
}

Status XCharString::assign(const XCharString & other)
{
	if (string_mem_length < other.string_length + 1)
	{
		if (content_string != nullptr)
		{
			delete[] content_string;
		}
		content_string = new char[other.string_length + 1];
		string_mem_length = other.string_length + 1;
	}

	memcpy(content_string, other.content_string, other.string_length + 1);
	string_length = other.string_length;

	return OK;
}

Status XCharString::assign(XCharString && local_other)
{
	l_assign(local_other);

	return OK;
}

Status XCharString::pop_out(int start, int end)
{
	int shift_length = string_length - end + 1;

	for (int i = 0; i < shift_length; i++)
	{
		content_string[start + i] = content_string[end + i + 1];
	}

	string_length = string_length - (end - start + 1);
	content_string[string_length] = 0;

	return OK;
}

void XCharString::swap(XCharString & other)
{
	int int_swap;

	int_swap = string_length;
	string_length = other.string_length;
	other.string_length = int_swap;

	int_swap = string_mem_length;
	string_mem_length = other.string_mem_length;
	other.string_mem_length = int_swap;

	char* string_swap;

	string_swap = content_string;
	content_string = other.content_string;
	other.content_string = string_swap;
}

int XCharString::replace(char from, char to)
{
	int result = 0;

	for (int i = 0; i < string_length; i++)
	{
		if (content_string[i] == from)
		{
			content_string[i] = to;
			result++;
		}
	}

	return result;
}

void XCharString::reverse()
{
	int half_length = string_length / 2;
	for (int i = 0; i < half_length; i++)
	{
		char temp = content_string[i];
		content_string[i] = content_string[string_length - 1 - i];
		content_string[string_length - 1 - i] = temp;

	}
}

void XCharString::clear()
{
	if (content_string != nullptr)
	{
		delete[] content_string;
	}

	content_string = new char[DEFAULT_CHAR_ARRAY_LENGTH];
	content_string[0] = 0;
	string_mem_length = 1;
	string_length = 0;
}

int XCharString::getHashCode() const
{
	return 0;
}

bool XCharString::operator==(const XCharString & other) const
{
	if (string_length != other.string_length)
	{
		return false;
	}

	for (int i = 0; i < string_length; i++)
	{
		if (content_string[i] != other.content_string[i])
		{
			return false;
		}
	}

	return true;
}

bool XCharString::operator!=(const XCharString & other) const
{
	return !(*this == other);
}

bool XCharString::operator>(const XCharString & other) const
{
	if (string_length > other.string_length)
	{
		return true;
	}
	else if (string_length < other.string_length)
	{
		return false;
	}
	else
	{
		for (int i = 0; i < string_length; i++)
		{
			if (content_string[i] > other.content_string[i])
			{
				return true;
			}
			else if (content_string[i] < other.content_string[i])
			{
				return false;
			}
		}
	}

	return false;
}

bool XCharString::operator<(const XCharString & other) const
{
	if (string_length > other.string_length)
	{
		return false;
	}
	else if (string_length < other.string_length)
	{
		return true;
	}
	else
	{
		for (int i = 0; i < string_length; i++)
		{
			if (content_string[i] > other.content_string[i])
			{
				return false;
			}
			else if (content_string[i] < other.content_string[i])
			{
				return true;
			}
		}
	}

	return false;
}

XCharString & XCharString::operator=(const XCharString & other)
{
	assign(other);

	return *this;
}

XCharString & XCharString::operator=(XCharString && other)
{
	l_assign(other);

	return *this;
}

XCharString XCharString::operator+(const XCharString & other) const
{
	int string_t_length = string_length + other.string_length + 1;

	XCharString result(string_t_length - 1, false);
	result.string_length = string_t_length - 1;
	memcpy(result.content_string, content_string, string_length);
	memcpy(result.content_string + string_length, other.content_string, other.string_length + 1);

	return result;
}

XCharString & XCharString::operator+=(const XCharString & other)
{
	return concat(other);
}

XCharString & XCharString::operator+=(const char * other)
{
	return concat(other);
}

XCharString XCharString::operator+(const char * other) const
{
	int string_t_length = string_length + strlen(other) + 1;
	
	XCharString result(string_t_length, false);
	result.string_length = string_t_length - 1;
	memcpy(result.content_string, content_string, string_length);
	memcpy(result.content_string + string_length, other, strlen(other) + 1);

	return result;
}

const char & XCharString::operator[](int pos) const
{
	if (pos < 0 || pos >= string_length)
	{
		return 0;
	}

	return content_string[pos];
}

XCharString::operator int() const
{
	return string_length;
}

XCharString::operator bool() const
{
	return string_length != 0;
}

const char * XCharString::c_str() const
{
	return content_string;
}

const char * XCharString::data() const
{
	return content_string;
}

void XCharString::PrintCharCodes() const
{
	for (int i = 0; i < string_length; i++)
	{
		cout << (int)content_string[i] << ' ';
		if ((i + 1) % 10 == 0)
		{
			cout << endl;
		}
	}
	cout << endl;
}

void XCharString::l_assign(XCharString & other)
{
	if (content_string != nullptr)
	{
		delete[] content_string;
	}

	content_string = other.content_string;
	other.content_string = nullptr;
	string_mem_length = other.string_mem_length;
	string_length = other.string_length;
}

size_t XCharString::WCharStr_to_CharStr(char * dest, wchar_t * src, int wlen)
{
	const char* curLocale = setlocale(LC_ALL, "chs");
	size_t converted;
	wcstombs_s(&converted, dest, (size_t)(sizeof(wchar_t) * wlen + 1), src, _TRUNCATE);
	setlocale(LC_ALL, curLocale);

	return converted;
}

size_t XCharString::CharStr_to_WCharStr(wchar_t * dest, char * src, int clen)
{
	const char* curLocale = setlocale(LC_ALL, "chs");
	size_t converted;
	mbstowcs_s(&converted, dest, (size_t)(sizeof(char) * clen + 1), src, _TRUNCATE);
	setlocale(LC_ALL, curLocale);

	return converted;
}

size_t XCharString::UTF8_to_UTF16(wchar_t * dest, char * src, int clen)
{
	int i = 0;

	char hide_code_tail = 63;

	XList<wchar_t> total_get;

	while (i < clen)
	{
		int utf8_length;
		if (src[i] >> 7 == 0)
		{
			utf8_length = 1;
		}
		else
		{
			utf8_length = 2;
			while ((src[i] >> (6 - utf8_length + 1) & 1) == 1)
			{
				utf8_length++;
			}
		}

		wchar_t temp_get = 0;
		if (utf8_length == 1)
		{
			temp_get = src[i];
		}
		else
		{
			//Get tail utf8
			for (int k = 0; k < utf8_length - 1; k++)
			{
				int final_temp = src[i + utf8_length - 1 - k] & hide_code_tail;
				final_temp = final_temp << (6 * k);
				temp_get = temp_get | final_temp;
			}
			//Get head utf8
			char temp = src[i] << (utf8_length);
			temp = temp >> (utf8_length);
			int final_temp = temp << (6 * (utf8_length - 1));
			temp_get = temp_get | final_temp;
		}

		total_get.push(temp_get);
		i += utf8_length;
	}

	i = 0;

	total_get.enumerate([&](wchar_t& data) -> void {
		dest[i] = data;
		i++;
	});

	dest[i] = 0;

	return i;
}

XCharString::Unicode_Encoding XCharString::CheckBom(char * src, int len)
{
	if (len <= 1)
	{
		return Unicode_Encoding::NONE;
	}
	if (len >= 2)
	{
		if (src[0] == (char)0xFE && src[1] == (char)0xFF)
		{
			return Unicode_Encoding::UTF16_BigEndian;
		}
		else if (src[0] == (char)0xFF && src[1] == (char)0xFE)
		{
			return Unicode_Encoding::UTF16_LittleEndian;
		}
	}
	if (len >= 3)
	{
		if (src[0] == (char)0xEF && src[1] == (char)0xBB && src[2] == (char)0xBF)
		{
			return Unicode_Encoding::UTF8;
		}
	}
	if (len >= 4)
	{
		if (src[0] == 0 && src[1] == 0 && src[2] == (char)0xEF && src[3] == (char)0xFF)
		{
			return Unicode_Encoding::UTF32_BigEndian;
		}
		else if (src[0] == (char)0xFF && src[1] == (char)0xFE && src[2] == 0 && src[3] == 0)
		{
			return Unicode_Encoding::UTF32_LittleEndian;
		}
	}

	return Unicode_Encoding::NONE;
}

Status XCharString::GetInt(char * source, int len, int & out_value)
{
	if (len < 0)
	{
		return ERROR;
	}

	if (len == 0)
	{
		return xstring('0');
	}

	out_value = 0;
	for (int i = 0; i < len; i++)
	{
		if (source[i] == '\0')
		{
			break;
		}

		if (source[i] >= '0' && source[i] <= '9')
		{
			out_value = out_value * 10 + (source[i] - '0');
		}
		else
		{
			return ERROR;
		}
	}

	return OK;
}

Status XCharString::GetInt(wchar_t * source, int len, int & out_value)
{
	if (len < 0)
	{
		return ERROR;
	}

	if (len == 0)
	{
		return xstring('0');
	}

	out_value = 0;
	for (int i = 0; i < len; i++)
	{
		if (source[i] == '\0')
		{
			break;
		}

		if (source[i] >= '0' && source[i] <= '9')
		{
			out_value = out_value * 10 + (source[i] - '0');
		}
		else
		{
			return ERROR;
		}
	}

	return OK;
}

int XCharString::GetLength(wchar_t * source)
{
	for (int i = 0; i < GETLENGTH_DETECTION_LENGTH; i++)
	{
		if (source[i] == '\0')
		{
			return i;
		}
	}

	return -1;
}

int XCharString::GetLength(char * source)
{
	for (int i = 0; i < GETLENGTH_DETECTION_LENGTH; i++)
	{
		if (source[i] == '\0')
		{
			return i;
		}
	}

	return -1;
}

XCharString XCharString::ToString(const int & value)
{
	if (value == 0)
	{
		return xstring('0');
	}

	xstring result(10);

	int rest = value;

	while (rest != 0)
	{
		int digit = rest % 10;
		rest /= 10;
		result.concat(xstring((char)(digit + '0')));
	}
	result.reverse();

	if (value < 0)
	{
		result = xstring('-') + result;
	}

	return result;
}


XCharString operator+(const char * str1, XCharString str2)
{
	int string_t_length = str2.string_length + strlen(str1) + 1;

	XCharString result(string_t_length - 1, false);
	result.string_length = string_t_length - 1;
	memcpy(result.content_string, str1, strlen(str1));
	memcpy(result.content_string + strlen(str1), str2.content_string, str2.string_length + 1);

	return result;
}

ostream & operator<<(ostream & out, const XCharString & to_out)
{
	out << to_out.content_string;
	return out;
}

istream & operator >> (istream & in, XCharString & to_in)
{
	char temp[1000] = { 0 };

	while (!in.eof())
	{
		in.get(temp, 1000); 
		to_in += temp;
	}

	return in;
}
