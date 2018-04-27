#include "XAVL.h"

XAVL::XAVL() :
	data()
{
	lchild = nullptr;
	rchild = nullptr;

	height = 1;
}


XAVL::~XAVL()
{
	if (lchild != nullptr)
	{
		delete lchild;
	}
	if (rchild != nullptr)
	{
		delete rchild;
	}
}

void XAVL::Insert(const XInvertedFile & value)
{
	if (value.word.data.length() == 1 && value.word.data[0] == ' ')
	{
		return;
	}

	if (data.word.data.length() == 0)
	{
		data = value;
		return;
	}

	//mini_f是最小不平衡树的父 或 最小不平衡树
	XAVL* mini_f = nullptr;
	bool is_father = false;
	bool g1;
	Insert_recursive(value, mini_f, g1, is_father);
	if (mini_f != nullptr)
	{
		//这里可能会产生误解，不过is_father如果是true，说明mini_f是最小不平衡树。
		if (is_father)
		{
			Adjust(mini_f, nullptr, false);
		}
		else
		{
			if (mini_f->lchild != nullptr && (mini_f->lchild->GetAlpha() == 2 || mini_f->lchild->GetAlpha() == -2))
			{
				Adjust(mini_f->lchild, mini_f, true);
			}
			else if (mini_f->rchild != nullptr && (mini_f->rchild->GetAlpha() == 2 || mini_f->rchild->GetAlpha() == -2))
			{
				Adjust(mini_f->rchild, mini_f, false);
			}
		}
	}
}

Status XAVL::Search(xstring to_search, XAVL *& out_find)
{
	return Search_recursive(to_search, out_find);
}

void XAVL::Adjust(XAVL * mini, XAVL * mini_f, bool lc)
{
	if (mini->lchild != nullptr && mini->lchild->GetAlpha() == 1)	//右单旋转
	{
		XAVL* top_node;
		if (mini_f != nullptr)
		{
			if (lc)
			{
				mini_f->lchild = mini->lchild;
				mini->lchild = mini->lchild->rchild;
				mini_f->lchild->rchild = mini;
				top_node = mini_f->lchild;
			}
			else
			{
				mini_f->rchild = mini->lchild;
				mini->lchild = mini->lchild->rchild;
				mini_f->rchild->rchild = mini;
				top_node = mini_f->rchild;
			}
		}
		else
		{
			XAVL* temp = mini->lchild;
			mini->lchild = temp->rchild;
			temp->rchild = mini;
			swap(temp, false);
			top_node = this;
		}
		
		//Height management
		int lh = (top_node->rchild->lchild == nullptr ? 0 : top_node->rchild->lchild->height);
		int rh = (top_node->rchild->rchild == nullptr ? 0 : top_node->rchild->rchild->height);
		top_node->rchild->height = lh > rh ? lh : rh;
		top_node->rchild->height++;

		top_node->height = top_node->rchild->height + 1;

		if (mini_f != nullptr)
		{
			if (lc)
			{
				mini_f->height = top_node->height + 1 > mini_f->lchild->height ? top_node->height + 1 : mini_f->lchild->height;
			}
			else
			{
				mini_f->height = top_node->height + 1 > mini_f->rchild->height ? top_node->height + 1 : mini_f->rchild->height;
			}
		}
	}
	else if (mini->rchild != nullptr && mini->rchild->GetAlpha() == -1)	//左单旋转
	{
		XAVL* top_node;
		if (mini_f != nullptr)
		{
			if (lc)
			{
				mini_f->lchild = mini->rchild;
				mini->rchild = mini->rchild->lchild;
				mini_f->lchild->lchild = mini;
				top_node = mini_f->lchild;
			}
			else
			{
				mini_f->rchild = mini->rchild;
				mini->rchild = mini->rchild->lchild;
				mini_f->rchild->lchild = mini;
				top_node = mini_f->rchild;
			}
		}
		else
		{
			XAVL* temp = mini->rchild;
			mini->rchild = temp->lchild;
			temp->lchild = mini;
			swap(temp, true);
			top_node = this;
		}
		//Height management
		int lh = (top_node->lchild->lchild == nullptr ? 0 : top_node->lchild->lchild->height);
		int rh = (top_node->lchild->rchild == nullptr ? 0 : top_node->lchild->rchild->height);
		top_node->lchild->height = lh > rh ? lh : rh;
		top_node->lchild->height++;

		top_node->height = top_node->lchild->height + 1;

		if (mini_f != nullptr)
		{
			if (lc)
			{
				mini_f->height = top_node->height + 1 > mini_f->lchild->height ? top_node->height + 1 : mini_f->lchild->height;
			}
			else
			{
				mini_f->height = top_node->height + 1 > mini_f->rchild->height ? top_node->height + 1 : mini_f->rchild->height;
			}
		}
	}
	else if (mini->lchild != nullptr && mini->lchild->GetAlpha() == -1)	//先左后右双旋转
	{
		XAVL* top_node;
		if (mini_f != nullptr)
		{
			if (lc)
			{
				mini_f->lchild = mini->lchild->rchild;
				mini->lchild->rchild = mini_f->lchild->lchild;
				mini_f->lchild->lchild = mini->lchild;
				mini->lchild = mini_f->lchild->rchild;
				mini_f->lchild->rchild = mini;
				top_node = mini_f->lchild;
			}
			else
			{
				mini_f->rchild = mini->lchild->rchild;
				mini->lchild->rchild = mini_f->rchild->lchild;
				mini_f->rchild->lchild = mini->lchild;
				mini->lchild = mini_f->rchild->rchild;
				mini_f->rchild->rchild = mini;
				top_node = mini_f->rchild;
			}
		}
		else
		{
			XAVL* temp = mini->lchild->rchild;
			mini->lchild->rchild = temp->lchild;
			temp->lchild = mini->lchild;
			mini->lchild = temp->rchild;
			temp->rchild = mini;
			swap(temp, false);
			top_node = this;
		}

		//Height management
		int lh = (top_node->rchild->lchild == nullptr ? 0 : top_node->rchild->lchild->height);
		int rh = (top_node->rchild->rchild == nullptr ? 0 : top_node->rchild->rchild->height);
		top_node->rchild->height = lh > rh ? lh : rh;
		top_node->rchild->height++;

		lh = (top_node->lchild->lchild == nullptr ? 0 : top_node->lchild->lchild->height);
		rh = (top_node->lchild->rchild == nullptr ? 0 : top_node->lchild->rchild->height);
		top_node->lchild->height = lh > rh ? lh : rh;
		top_node->lchild->height++;

		top_node->height = top_node->lchild->height > top_node->rchild->height ?
			top_node->lchild->height : top_node->rchild->height;
		top_node->height++;

		if (mini_f != nullptr)
		{
			if (lc)
			{
				mini_f->height = top_node->height + 1 > mini_f->lchild->height ? top_node->height + 1 : mini_f->lchild->height;
			}
			else
			{
				mini_f->height = top_node->height + 1 > mini_f->rchild->height ? top_node->height + 1 : mini_f->rchild->height;
			}
		}
	}
	else if (mini->rchild != nullptr && mini->rchild->GetAlpha() == 1)	//先右后左双旋转
	{
		XAVL* top_node;
		if (mini_f != nullptr)
		{
			if (lc)
			{
				mini_f->lchild = mini->rchild->lchild;
				mini->rchild->lchild = mini_f->lchild->rchild;
				mini_f->lchild->rchild = mini->rchild;
				mini->rchild = mini_f->lchild->lchild;
				mini_f->lchild->lchild = mini;
				top_node = mini_f->lchild;
			}
			else
			{
				mini_f->rchild = mini->rchild->lchild;
				mini->rchild->lchild = mini_f->rchild->rchild;
				mini_f->rchild->rchild = mini->rchild;
				mini->rchild = mini_f->rchild->lchild;
				mini_f->rchild->lchild = mini;
				top_node = mini_f->rchild;
			}
		}
		else
		{
			XAVL* temp = mini->rchild->lchild;
			mini->rchild->lchild = temp->rchild;
			temp->rchild = mini->rchild;
			mini->rchild = temp->lchild;
			temp->lchild = mini;
			swap(temp, true);
			top_node = this;
		}

		//Height management
		int lh = (top_node->rchild->lchild == nullptr ? 0 : top_node->rchild->lchild->height);
		int rh = (top_node->rchild->rchild == nullptr ? 0 : top_node->rchild->rchild->height);
		top_node->rchild->height = lh > rh ? lh : rh;
		top_node->rchild->height++;

		lh = (top_node->lchild->lchild == nullptr ? 0 : top_node->lchild->lchild->height);
		rh = (top_node->lchild->rchild == nullptr ? 0 : top_node->lchild->rchild->height);
		top_node->lchild->height = lh > rh ? lh : rh;
		top_node->lchild->height++;

		top_node->height = top_node->lchild->height > top_node->rchild->height ?
			top_node->lchild->height : top_node->rchild->height;
		top_node->height++;

		if (mini_f != nullptr)
		{
			if (lc)
			{
				mini_f->height = top_node->height + 1 > mini_f->lchild->height ? top_node->height + 1 : mini_f->lchild->height;
			}
			else
			{
				mini_f->height = top_node->height + 1 > mini_f->rchild->height ? top_node->height + 1 : mini_f->rchild->height;
			}
		}
	}
}

int XAVL::length() const
{
	int result = 0;
	length_recursive(result);
	return result;
}

void XAVL::enumerate(function<void(XInvertedFile&data)> func)
{
	if (lchild != nullptr)
	{
		lchild->enumerate(func);
	}
	func(data);
	if (rchild != nullptr)
	{
		rchild->enumerate(func);
	}
}

XInvertedFile * XAVL::GetDataAddr()
{
	return &data;
}

void XAVL::Insert_recursive(const XInvertedFile & value, XAVL* & out_mini_, bool & grow, bool & isfather)
{
	if (value.word.data < data.word.data)
	{
		if (lchild != nullptr)
		{
			bool grow_get = false;
			bool isfather_get = false;
			lchild->Insert_recursive(value, out_mini_, grow_get, isfather_get);
			if (isfather_get)
			{
				out_mini_ = this;
				isfather = false;
			}
			if (out_mini_ == nullptr && (GetAlpha() == 2 || GetAlpha() == -2))
			{
				isfather = true;
				out_mini_ = this;
			}
			//Heigth management
			if (!grow_get)
			{
				grow = false;
			}
			else
			{
				if (rchild == nullptr)
				{
					height++;
				}
				else
				{
					if (rchild->height >= lchild->height)
					{
						grow = false;
					}
					else
					{
						height++;
					}
				}
			}
		}
		else
		{
			XAVL* temp = new XAVL;
			temp->data = value;

			lchild = temp;
			if (rchild == nullptr)
			{
				height++;
			}
			grow = true;
		}
	}
	else
	{
		if (rchild != nullptr)
		{
			bool grow_get = false;
			bool isfather_get = false;
			rchild->Insert_recursive(value, out_mini_, grow_get, isfather_get);
			if (isfather_get)
			{
				out_mini_ = this;
			}
			if (out_mini_ == nullptr && (GetAlpha() == 2 || GetAlpha() == -2))
			{
				isfather = true;
			}
			//Heigth management
			if (!grow_get)
			{
				grow = false;
			}
			else
			{
				if (lchild == nullptr)
				{
					height++;
					grow = true;
				}
				else
				{
					if (lchild->height >= rchild->height)
					{
						grow = false;
					}
					else
					{
						height++;
						grow = true;
					}
				}
			}
		}
		else
		{
			XAVL* temp = new XAVL;
			temp->data = value;

			rchild = temp;
			if (lchild == nullptr)
			{
				height++;
			}
			grow = true;
		}
	}
}

Status XAVL::Search_recursive(xstring to_search, XAVL *& out_find)
{
	if (to_search < data.word.data)
	{
		if (lchild != nullptr)
		{
			return lchild->Search(to_search, out_find);
		}
		else
		{
			out_find = this;
			return ERROR;
		}
	}
	else if (to_search > data.word.data)
	{
		if (rchild != nullptr)
		{
			return rchild->Search(to_search, out_find);
		}
		else
		{
			out_find = this;
			return ERROR;
		}
	}
	else
	{
		out_find = this;
		return OK;
	}
}

void XAVL::length_recursive(int& total) const
{
	if (lchild != nullptr)
	{
		lchild->length_recursive(total);
	}
	total++;
	if (rchild != nullptr)
	{
		rchild->length_recursive(total);
	}
}

int XAVL::GetAlpha() const
{
	return (lchild == nullptr ? 0 : lchild->height) - (rchild == nullptr ? 0 : rchild->height);
}

void XAVL::swap(XAVL * other, bool l)
{
	XAVL* temp = lchild;
	if (l)
	{
		lchild = other;
	}
	else
	{
		lchild = other->lchild;
	}
	other->lchild = temp;

	temp = rchild;
	if (l)
	{
		rchild = other->rchild;	
	}
	else
	{
		rchild = other;
	}
	other->rchild = temp;

	XInvertedFile tempif = data;
	data = other->data;
	other->data = tempif;

	int temp_int;
	temp_int = height;
	height = other->height;
	other->height = temp_int;
}
