#pragma once
#include "XCharStringList.h"

typedef class XHtmlMarkUp
{
public:
	enum HtmlMarkUp_Compare_Methods { Head_Property_ALL,
		Head_Only,
		Head_Property_Vague,
	};

public:
	XHtmlMarkUp();
	XHtmlMarkUp(const xstring& markup);
	~XHtmlMarkUp();

	const xstring& GetHead() const;
	const xslist& GetProperties() const;
	xstring GetMarkUp() const;
	xstring GetClosingTag() const;
	bool Compare(const XHtmlMarkUp& other, HtmlMarkUp_Compare_Methods methods = Head_Property_ALL) const;
	bool HaveProperty(const xstring& property_name) const;
	bool HaveProperty_Vague(const xstring& property_name) const;
	void Assign(const xstring& markup);

private:
	xstring markup_head;
	xslist markup_properties;
}xmarkup;

