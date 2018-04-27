#pragma once

//≈≈–Ú∆Ω∫‚ ˜

#include "XStatus.h"
#include "XInvertedFile.h"
#include <functional>

using namespace std;

//Equal is bigger.
class XAVL
{
private:
	XAVL* lchild;
	XAVL* rchild;
	XInvertedFile data;

	int height;

public:
	XAVL();
	~XAVL();

	void Insert(const XInvertedFile& value);
	Status Search(xstring to_search, XAVL* & out_find);
	void Adjust(XAVL* mini, XAVL* mini_f, bool lc);
	int length() const;
	//LDR enumerate
	void enumerate(function<void(XInvertedFile& data)> func);
	XInvertedFile* GetDataAddr();

private:
	void Insert_recursive(const XInvertedFile& value, XAVL* & out_mini_, bool & grow, bool & isfather);
	Status Search_recursive(xstring to_search, XAVL *& out_find);
	void  length_recursive(int& total) const;
	int GetAlpha() const;
	void swap(XAVL* other, bool l);

};

