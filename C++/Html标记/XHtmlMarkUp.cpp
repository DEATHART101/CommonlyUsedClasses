#include "XHtmlMarkUp.h"



XHtmlMarkUp::XHtmlMarkUp()
{

}

XHtmlMarkUp::XHtmlMarkUp(const xstring & markup)
{
	Assign(markup);
}


XHtmlMarkUp::~XHtmlMarkUp()
{
}

const xstring & XHtmlMarkUp::GetHead() const
{
	return markup_head;
}

const xslist & XHtmlMarkUp::GetProperties() const
{
	return markup_properties;
}

xstring XHtmlMarkUp::GetMarkUp() const
{
	xstring temp(50);
	temp = "<" + markup_head;
	
	int length = markup_properties.length();
	for (int i = 0; i < length; i++)
	{
		temp += " " + markup_properties[i];
	}

	temp += ">";

	return temp;
}

xstring XHtmlMarkUp::GetClosingTag() const
{
	return "</" + markup_head + ">";
}

bool XHtmlMarkUp::Compare(const XHtmlMarkUp & other, HtmlMarkUp_Compare_Methods methods) const
{
	switch (methods)
	{
	case HtmlMarkUp_Compare_Methods::Head_Property_ALL:
	{
		if (markup_head == other.markup_head)
		{
			if (markup_properties.length() != other.markup_properties.length())
			{
				return false;
			}

			int properties_length = markup_properties.length();
			for (int i = 0; i < properties_length; i++)
			{
				if (!other.HaveProperty(markup_properties[i]))
				{
					return false;
				}
			}

			return true;
		}

		return false;
	}
	case HtmlMarkUp_Compare_Methods::Head_Only:
	{
		return markup_head == other.markup_head;
	}
	case HtmlMarkUp_Compare_Methods::Head_Property_Vague:
	{
		if (markup_head == other.markup_head)
		{
			if (markup_properties.length() != other.markup_properties.length())
			{
				return false;
			}

			int properties_length = markup_properties.length();
			for (int i = 0; i < properties_length; i++)
			{
				if (!other.HaveProperty_Vague(markup_properties[i]))
				{
					return false;
				}
			}

			return true;
		}

		return false;
	}
	default:
		break;
	}
}

bool XHtmlMarkUp::HaveProperty(const xstring & property_name) const
{
	return (markup_properties.search(property_name) != -1);
}

bool XHtmlMarkUp::HaveProperty_Vague(const xstring & property_name) const
{
	xstring temp_comp = property_name.substring(0, property_name.indexOf('=') - 1);

	int len = markup_properties.length();
	for (int i = 0; i < len; i++)
	{
		const xstring& get_str = markup_properties[i];
		if (get_str.substring(0, get_str.indexOf('=')) == temp_comp)
		{
			return true;
		}
	}

	return false;
}

//½²xstring×ª»»³ÉhtmlÔªËØ
void XHtmlMarkUp::Assign(const xstring & markup)
{
	int space_pos = markup.indexOf(' ');
	if (space_pos == -1)
	{
		markup_head = markup.substring(1, markup.length() - 2);
	}
	else
	{
		markup_head = markup.substring(1, space_pos - 1);

		if (markup_head != "div")
		{
			return;
		}
		
		while (true)
		{
			int prop_end_pos = markup.indexOf('\"', space_pos + 1);
			prop_end_pos = markup.indexOf('\"', prop_end_pos + 1);

			markup_properties.push(markup.substring(space_pos + 1, prop_end_pos));

			if (markup.indexOf('=', prop_end_pos + 1) == -1)
			{
				break;
			}
			else
			{
				space_pos = prop_end_pos + 1;
			}
		}
	}
}
