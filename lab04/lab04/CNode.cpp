#include "CNode.h"

#define I_ZERO 0
#define I_DEFAULT_COMPUTE_VALUE -1

//parametrized constructor
CNode::CNode(string s_val, int i_maxChildr)
{
	s_value = s_val;
	d_computeValue = I_DEFAULT_COMPUTE_VALUE;
	pc_parent = NULL;
	i_maxChildren = i_maxChildr;
	i_currChildren = I_ZERO;
}
//parametrized constructor
CNode::CNode(char c_val, int i_maxChildr)
{
	s_value = c_val;
	d_computeValue = I_DEFAULT_COMPUTE_VALUE;
	pc_parent = NULL;
	i_maxChildren = i_maxChildr;
	i_currChildren = I_ZERO;
}
//copy constructor
CNode::CNode(const CNode &pc_other)
{
	s_value = pc_other.s_value;
	d_computeValue = pc_other.d_computeValue;
	pc_parent = pc_other.pc_parent;
	i_maxChildren = pc_other.i_maxChildren;
	i_currChildren = pc_other.i_currChildren;
	for (int i = 0; i < i_maxChildren; i++)
		vc_children.push_back(pc_other.vc_children[i]);
}
//destructor
CNode::~CNode()
{
	for (int i = 0; i < i_maxChildren; i++)
		delete vc_children[i];
}
//adds node to this node
CNode* CNode::addNode(string s_partOfExpression, int i_maxChildr)
{
	CNode *pc_current = this;
	CNode *pc_new = new CNode(s_partOfExpression, i_maxChildr);
	while (pc_current->getCurrChildren() == pc_current->getMaxChildren() && pc_current->pc_parent != NULL)
		pc_current = pc_current->pc_parent;
	if (pc_current->getCurrChildren() < pc_current->getMaxChildren())
	{
		pc_current->vc_children.push_back(pc_new);
		pc_current->i_currChildren++;
		pc_new->setParent(pc_current);
		if (isDigit(pc_new->s_value[0]) || isNumber(pc_new->s_value))
			pc_new->d_computeValue = stoi(pc_new->s_value);
		return pc_new;
	}
	else
	{
		return NULL;
	}
}
//returns this nodes value as a string
string CNode::getValue()
{
	return s_value;
}
//returns number of maximum children this node can have
int CNode::getMaxChildren()
{
	return i_maxChildren;
}
//returns number of current children
int CNode::getCurrChildren()
{
	return i_currChildren;
}
//sets given node as a parent of this
void CNode::setParent(CNode *pc_newParent)
{
	pc_parent = pc_newParent;
}
//returns if c_char is a letter,
bool CNode::isLetter(char c_char)
{
	if ((int(c_char) >= 65 && int(c_char) <= 90) || (int(c_char) >= 97 && int(c_char) <= 122))
		return true;
	return false;
}
//returns if c_char is an operator,
bool CNode::isOperator(char c_char)
{
	if (c_char == '+' || c_char == '-' || c_char == '/' || c_char == '*')
		return true;
	return false;
}
//returns if c_char is a digit,
bool CNode::isDigit(char c_char)
{
	if (int(c_char) >= 48 && int(c_char) <= 57)
		return true;
	return false;
}
//returns if s_var is a name of trigonometric function(sin and cos),
bool CNode::isTrigFunction(string s_var)
{
	if (s_var == "sin" || s_var == "cos")
		return true;
	return false;
}
//returns if s_var is a name of trigonometric function(sin and cos),
bool CNode::isNumber(string s_var)
{
	for (int i = 0; i < s_var.length(); i++)
		if (!isDigit(s_var[i]))
			return false;
	return true;
}
//returns information about node in a string
string CNode::toString() 
{
	string s_result = "[";
	s_result += s_value + "] || " + "ComputingValue: " + to_string(d_computeValue) + ", Maximum Children: " + to_string(i_maxChildren);
	if (i_maxChildren != 0)
		s_result += " Left Child: [" + vc_children[0]->s_value + "]";
	if (i_maxChildren == 2)
		s_result += " Right Child: [" + vc_children[1]->s_value + "]";
	return s_result;
}