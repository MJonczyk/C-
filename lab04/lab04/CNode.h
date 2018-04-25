#pragma once
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class CNode
{
	friend class CTree;
public:
	CNode(string s_val, int i_maxChildr);
	CNode(char c_val, int i_maxChildr);
	CNode(const CNode &pc_other);
	~CNode();
	CNode*		addNode(string s_partOfExpression, int i_maxChildr);
	string		getValue();
	int			getMaxChildren();
	int			getCurrChildren();
	void		setParent(CNode *pc_newParent);
	static bool isLetter(char c_char);
	static bool isOperator(char c_char);
	static bool isDigit(char c_char);
	static bool isTrigFunction(string s_var);
	static bool isNumber(string s_var);
	string		toString();

private:
	string			s_value;
	vector<CNode*>	vc_children;
	double			d_computeValue;
	int				i_maxChildren;
	int				i_currChildren;
	CNode*			pc_parent;
};