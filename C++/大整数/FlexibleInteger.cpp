#include "FlexibleInteger.h"

FlexibleInteger::FlexibleInteger()
{
	Clear();
}

FlexibleInteger::FlexibleInteger(const char *num)
{
	*this = num;
}

FlexibleInteger::FlexibleInteger(const int& num)
{
	*this = num;
}

void FlexibleInteger::Clear()
{
	memset(m_NumberBits, 0, sizeof(m_NumberBits));
	m_NumberLength = 1;
	m_Positive = 1;
}

void FlexibleInteger::Print(ostream & output) const
{
	output << *this;
}

string FlexibleInteger::ToString() const
{
	string result;
	for (int i = 0; i < m_NumberLength; i++)
	{
		result = (char)(m_NumberBits[i] + '0') + result;
	}

	if (!m_Positive)
	{
		result = "-" + result;
	}
		
	return result;
}

istream &operator>>(istream &in, FlexibleInteger &num)
{
	string str;
	in >> str;
	num = str;
	return in;
}

ostream &operator<<(ostream &out, const FlexibleInteger &num)
{
	out << num.ToString();
	return out;
}

FlexibleInteger FlexibleInteger::operator=(const char *nums)
{
	Clear();

	int numLength = strlen(nums);
	if (numLength <= 0 ||
		numLength == 1 && nums[0] == '-')
	{
		return FlexibleInteger();
	}

	m_Positive = (nums[0] != '-');
	int start = m_Positive ? 0 : 1;
	int end = numLength - 1;

	if (start == end && nums[start] == '0')
	{
		m_Positive = 1;
		return FlexibleInteger();
	}
	
	m_NumberLength = end - start + 1;
	for (int i = 0; i < m_NumberLength; i++)
	{
		m_NumberBits[i] = nums[end - i] - '0';
	}

	return *this;
}

FlexibleInteger FlexibleInteger::operator=(int num)
{
	Clear();

	if (num < 0)
	{
		m_Positive = false;
		num = -num;
	}

	int left = num;
	int lowmask = 1;
	int highmask = 10;
	while (left != 0)
	{
		m_NumberBits[m_NumberLength - 1] = (left % highmask) / lowmask;
		m_NumberLength++;
		left = left / highmask;
		highmask *= 10;
		lowmask *= 10;
	}
	
	return *this;
}

FlexibleInteger FlexibleInteger::operator=(const string num)
{
	const char *tmp;
	tmp = num.c_str();
	*this = tmp;
	return *this;
}

FlexibleInteger FlexibleInteger::operator+(const FlexibleInteger &num) const
{
	if (m_Positive ^ num.m_Positive)
	{
		if (m_Positive)
		{
			FlexibleInteger nnum = num;
			nnum.m_Positive = 1;
			return *this - nnum;
		}
		else
		{
			FlexibleInteger nself = *this;
			nself.m_Positive = 1;
			return num - nself;
		}
	}
	else
	{
		FlexibleInteger result;
		result.m_NumberLength = 0;
		int tensadd = 0;
		int longest = MAX(m_NumberLength, num.m_NumberLength);
		for (int i = 0; (tensadd != 0) || i < longest; i++)
		{
			int temp = m_NumberBits[i] + num.m_NumberBits[i] + tensadd;
			result.m_NumberBits[result.m_NumberLength++] = temp % 10;
			tensadd = temp / 10;
		}
		result.m_Positive = m_Positive;
		return result;
	}
}

FlexibleInteger FlexibleInteger::operator-(const FlexibleInteger &num) const
{
	FlexibleInteger one = *this;
	FlexibleInteger two = num;
	if (!num.m_Positive && !m_Positive)
	{
		one.m_Positive = 1;
		two.m_Positive = 1;
		return two - one;
	}
	else if (!two.m_Positive)
	{
		two.m_Positive = 1;
		return one + two;
	}
	else if (!one.m_Positive)
	{
		one.m_Positive = 1;
		two = FlexibleInteger() - (one + two);
		return two;
	}
	else if (one < two)
	{
		FlexibleInteger c = (two - one);
		c.m_Positive = false;
		return c;
	}

	FlexibleInteger result;
	result.m_NumberLength = 0;
	int decrease = 0;
	for (int i = 0; i < one.m_NumberLength; i++)
	{
		int temp = one.m_NumberBits[i] - decrease;
		if (i < two.m_NumberLength)
		{
			temp -= two.m_NumberBits[i];
		}
		if (temp >= 0)
		{
			decrease = 0;
		}
		else
		{
			decrease = 1;
			temp += 10;
		}
		result.m_NumberBits[result.m_NumberLength] = temp;
		result.m_NumberLength;
	}
	result.fixlength();
	return result;
}

FlexibleInteger FlexibleInteger::operator * (const FlexibleInteger &num)const
{
	FlexibleInteger result;
	result.m_NumberLength = m_NumberLength + num.m_NumberLength;

	for (int i = 0; i < m_NumberLength; i++)
	{
		for (int j = 0; j < num.m_NumberLength; j++)
		{
			result.m_NumberBits[i + j] += m_NumberBits[i] * num.m_NumberBits[j];
		}
	}

	for (int i = 0; i < result.m_NumberLength; i++)
	{
		result.m_NumberBits[i + 1] += result.m_NumberBits[i] / 10;
		result.m_NumberBits[i] %= 10;
	}

	result.fixlength();
	result.m_Positive = !(m_Positive ^ num.m_Positive);
	return result;
}

FlexibleInteger FlexibleInteger::operator /(const FlexibleInteger&num)const
{
	FlexibleInteger result;
	result.m_NumberLength = m_NumberLength - num.m_NumberLength + 1;
	if (result.m_NumberLength < 0)
	{
		result.m_NumberLength = 1;
		return result;
	}

	FlexibleInteger divisor = *this;
	FlexibleInteger	divid = num;
	divisor.m_Positive = divid.m_Positive = 1;
	int k = result.m_NumberLength - 1;
	int j = m_NumberLength - 1;
	while (k >= 0)
	{
		while (divisor.m_NumberBits[j] == 0)
		{
			j--;
		}
		if (k > j)
		{
			k = j;
		}

		char temp[MAX_LENGTH];
		memset(temp, 0, sizeof(temp));
		for (int i = j; i >= k; i--)
		{
			temp[j - i] = divisor.m_NumberBits[i] + '0';
		}
		FlexibleInteger dividend = temp;

		if (dividend < divid) 
		{
			k--; continue;
		}
		int key = 0;
		while (divid * key <= dividend)
		{
			key++;
		}
		key--;
		result.m_NumberBits[k] = key;

		FlexibleInteger tempp = divid * key;
		for (int i = 0; i < k; i++)
		{
			tempp = tempp * 10;
		}
		divisor = divisor - tempp;
		k--;
	}
	result.fixlength();
	result.m_Positive = !(m_Positive^num.m_Positive);
	return result;
}

bool FlexibleInteger::operator>(const FlexibleInteger & num) const
{
	if (m_Positive && !num.m_Positive)
	{
		return true;
	}
	else if (!m_Positive && num.m_Positive)
	{ 
		return false;
	}

	if (m_NumberLength != num.m_NumberLength)
	{
		return !((m_NumberLength > num.m_NumberLength ? true : false) ^ m_Positive);
	}

	for (int i = m_NumberLength - 1; i >=0; i--)
	{
		if (m_NumberBits[i] > num.m_NumberBits[i])
		{
			return (true ^ m_Positive);
		}
	}

	return !(false ^ m_Positive);
}

bool FlexibleInteger::operator<(const FlexibleInteger &num) const
{
	if (m_Positive && !num.m_Positive)
	{
		return false;
	}
	else if (!m_Positive && num.m_Positive)
	{
		return true;
	}

	if (m_NumberLength != num.m_NumberLength)
	{
		return !((m_NumberLength > num.m_NumberLength ? false : true) ^ m_Positive);
	}

	for (int i = m_NumberLength - 1; i >= 0; i--)
	{
		if (m_NumberBits[i] < num.m_NumberBits[i])
		{
			return !(true ^ m_Positive);
		}
	}

	return !(false ^ m_Positive);
}

bool FlexibleInteger::operator==(const FlexibleInteger & num) const
{
	if (m_Positive ^ num.m_Positive)
	{
		return false;
	}

	if (m_NumberLength != num.m_NumberLength)
	{
		return false;
	}

	for (int i = m_NumberLength - 1; i >= 0; i--)
	{
		if (m_NumberBits[i] != num.m_NumberBits[i])
		{
			return false;
		}
	}

	return true;
}

bool FlexibleInteger::operator!=(const FlexibleInteger & num) const
{
	return !(*this == num);
}

bool FlexibleInteger::operator<=(const FlexibleInteger & num) const
{
	if (m_Positive && !num.m_Positive)
	{
		return false;
	}
	else if (!m_Positive && num.m_Positive)
	{
		return true;
	}

	if (m_NumberLength != num.m_NumberLength)
	{
		return !((m_NumberLength > num.m_NumberLength ? false : true) ^ m_Positive);
	}

	for (int i = m_NumberLength - 1; i >= 0; i--)
	{
		if (m_NumberBits[i] > num.m_NumberBits[i])
		{
			return !(false ^ m_Positive);
		}
	}

	return !(true ^ m_Positive);
}

bool FlexibleInteger::operator>=(const FlexibleInteger & num) const
{
	if (m_Positive && !num.m_Positive)
	{
		return true;
	}
	else if (!m_Positive && num.m_Positive)
	{
		return false;
	}

	if (m_NumberLength != num.m_NumberLength)
	{
		return !((m_NumberLength > num.m_NumberLength ? true : false) ^ m_Positive);
	}

	for (int i = m_NumberLength - 1; i >= 0; i--)
	{
		if (m_NumberBits[i] < num.m_NumberBits[i])
		{
			return !(false ^ m_Positive);
		}
	}

	return !(true ^ m_Positive);
}

void FlexibleInteger::fixlength()
{
	if (m_NumberLength == 0)
	{
		m_NumberLength = 1;
		return;
	}

	for (int i = m_NumberLength - 1; i >= 0; i--)
	{
		if (m_NumberBits[i] != 0)
		{
			m_NumberLength = i + 1;
			return;
		}
	}

	m_NumberLength = 1;
}

FlexibleInteger::~FlexibleInteger()
{
}