#include "CTree.h"

#define S_OPERATOR_ADD "+"
#define S_OPERATOR_SUB "-"
#define S_OPERATOR_MUL "*"
#define S_OPERATOR_DIV "/"
#define S_VARIABLE_X "x"
#define S_VARIABLE_Y "y"
#define S_FUNCTION_SIN "sin"
#define S_FUNCTION_COS "cos"
#define S_RESULT "wynik"

//default constructor
CTree::CTree()
{
	s_variable s_var1 = { S_VARIABLE_X , 0 };
	s_variable s_var2 = { S_VARIABLE_Y, 0 };
	s_variable s_var3 = { S_RESULT, 0 };
	vs_variables.push_back(s_var1);
	vs_variables.push_back(s_var2);
	vs_variables.push_back(s_var3);
	pc_root = NULL;
	b_isRepaired = false;
	d_measure = 0;
}
//copy constructor
CTree::CTree(const CTree &pc_other)
{
	s_variable s_var1 = { S_VARIABLE_X, 0 };
	s_variable s_var2 = { S_VARIABLE_Y, 0 };
	s_variable s_var3 = { S_RESULT, 0 };
	vs_variables.push_back(s_var1);
	vs_variables.push_back(s_var2);
	vs_variables.push_back(s_var3);
	enterExpression(ToString(pc_other.pc_root));
	b_isRepaired = pc_other.b_isRepaired;
	d_measure = pc_other.d_measure;
}
//destructor
CTree::~CTree()
{
	delete pc_root;
	vs_variables.clear();
}
CTree& CTree::operator=(const CTree& pc_other)
{
	enterExpression(ToString(pc_other.pc_root));
	preOrder();
	return (*this);
}
CTree& CTree::operator+(const CTree& pc_other)
{
	CTree* pc_first = new CTree();
	CTree* pc_second = new CTree();
	pc_first->enterExpression(this->toString());
	pc_first->preOrder();
	pc_second->enterExpression(ToString(pc_other.pc_root));
	pc_second->preOrder();
	pc_first->join(pc_second);
	return (*pc_first);
}
//enters an expression(in prefix notation) into a tree, returns if entering was successful,
bool CTree::enterExpression(string s_expression)
{
	vector<string> vs_expression;
	vs_expression.clear();
	string s_commandToAnalyze = s_expression, s_currentWord;
	int i_spaceIndex = 0;
	while (i_spaceIndex != -1) {
		i_spaceIndex = s_commandToAnalyze.find(" ");
		s_currentWord = s_commandToAnalyze.substr(0, i_spaceIndex);
		vs_expression.push_back(s_currentWord);
		s_commandToAnalyze = s_commandToAnalyze.substr(i_spaceIndex + 1);
	}
	string s_tmp;
	int i_index = 0;
	s_variable s_var;
	if (vs_expression[0].length() == 1)
	{
		if (CNode::isOperator(vs_expression[0][0]))
		{
			pc_root = new CNode(vs_expression[0], 2);
		}
		else if (CNode::isLetter(vs_expression[0][0]))
		{
			pc_root = new CNode(vs_expression[0], 0);
			s_var = { vs_expression[0], -1 };
			vs_variables.push_back(s_var);
		}
		else if (CNode::isDigit(vs_expression[0][0]))
		{
			pc_root = new CNode(vs_expression[0], 0);
			pc_root->d_computeValue = stoi(vs_expression[0]);
		}
	}
	else
	{
		if (CNode::isTrigFunction(vs_expression[0]))
		{
			pc_root = new CNode(vs_expression[0], 1);
		}
		else if (CNode::isNumber(vs_expression[0]))
		{
			pc_root = new CNode(vs_expression[0], 0);
			pc_root->d_computeValue = stoi(vs_expression[0]);
		}
		else
		{
			pc_root = new CNode(vs_expression[0], 0);
			s_var = { vs_expression[0], -1 };
			vs_variables.push_back(s_var);
		}
	}
	CNode *pc_current = pc_root;
	for (int i = 1; i < vs_expression.size() && pc_current != NULL; i++)
	{
		i_index = i;
		s_tmp = vs_expression[i];
		if (s_tmp.length() == 1)
		{
			if (CNode::isOperator(s_tmp[0]))
			{
				pc_current = pc_current->addNode(s_tmp, 2);
			}
			else if (CNode::isLetter(s_tmp[0]))
			{
				pc_current = pc_current->addNode(s_tmp, 0);
				if (!isVariableAlreadyInVector(s_tmp))
				{
					s_var = { s_tmp, -1 };
					vs_variables.push_back(s_var);
				}
			}
			else if (CNode::isDigit(s_tmp[0]))
			{
				pc_current = pc_current->addNode(s_tmp, 0);
			}
		}
		else
		{
			if (CNode::isTrigFunction(s_tmp))
			{
				pc_current = pc_current->addNode(s_tmp, 1);
			}
			else if (CNode::isNumber(s_tmp))
			{
				pc_current = pc_current->addNode(s_tmp, 0);
			}
			else
			{
				pc_current = pc_current->addNode(s_tmp, 0);
				if (!isVariableAlreadyInVector(s_tmp))
				{
					s_var = { s_tmp, -1 };
					vs_variables.push_back(s_var);
				}
			}
		}
	}
	repair();
	if (b_isRepaired)
		return false;
	if (i_index == (vs_expression.size() - 1))
		return true;
	else
		return false;
}
//repairs tree if the expression consists only of operators
void CTree::repair()
{
	Repair(pc_root);
}
//repairs tree if the expression consists only of operators (repair helper)
void CTree::Repair(CNode* pc_node)
{
	for (int i = 0; i < pc_node->vc_children.size(); i++)
		Repair(pc_node->vc_children[i]);
	if (pc_node->s_value.length() == 1 || pc_node->s_value.length() == 3)
		if (CNode::isOperator(pc_node->s_value[0]) || CNode::isTrigFunction(pc_node->s_value))
		{
			while (pc_node->getCurrChildren() < pc_node->getMaxChildren())
			{
				string s_leaf = GenerateLeaf();
				CNode* pc_new = new CNode(s_leaf, 0);
				pc_node->vc_children.push_back(pc_new);
				pc_new->setParent(pc_node);
				if (CNode::isDigit(s_leaf[0]))
					pc_node->vc_children[pc_node->getCurrChildren()]->d_computeValue = stoi(s_leaf);
				pc_node->i_currChildren++;
				b_isRepaired = true;
			}
		}
}
//adds two trees (parameter tree becomes a subtree of the main tree)
void CTree::join(CTree* pc_subtree)
{
	CNode *pc_leafsParent = getLeafsParent();
	pc_leafsParent->vc_children[0] = pc_subtree->pc_root;
	vs_variables.clear();
	//UpdateVariables(pc_root);
}
//returns pointer to a leafs parent
CNode* CTree::getLeafsParent()
{
	CNode *pc_current = pc_root;
	while (pc_current->vc_children[0]->getMaxChildren() != 0)
		pc_current = pc_current->vc_children[0];
	return pc_current;
}
//prints tree in a preOrder
void CTree::preOrder()
{
	PreOrder(pc_root);
}
//prints tree in a preOrder (preOrder helper)
void CTree::PreOrder(CNode* pc_node)
{
	cout << pc_node->getValue() << " ";
	for (int i = 0; i < pc_node->getMaxChildren(); i++)
		PreOrder(pc_node->vc_children[i]);
}
//prints tree in a inOrder
void CTree::inOrder() 
{
	InOrder(pc_root);
}
//prints tree in a inOrder (inOrder helper)
void CTree::InOrder(CNode* pc_node) 
{
	if (pc_node->getMaxChildren() == 1)
	{
		cout << pc_node->getValue() << " ";
		InOrder(pc_node->vc_children[0]);
	}
	else {
		if (pc_node->getMaxChildren() > 0)
			InOrder(pc_node->vc_children[0]);
		cout << pc_node->getValue() << " ";
		if (pc_node->getMaxChildren() == 2)
			InOrder(pc_node->vc_children[1]);
	}
}
// prints variables
void CTree::printVariables()
{
	for (int i = 0; i < vs_variables.size(); i++)
		cout << vs_variables[i].s_value  << " = " << to_string(vs_variables[i].d_value) << ", ";
	cout << endl;
}
//checks if the tree contains any variable
bool CTree::isAnyVariable()
{
	return vs_variables.size() > 0;
}
//sets compute values to variables
void CTree::setComputeValue(vector<string> vs_command)
{
	for (int i = 0; i < vs_variables.size(); i++)
	{
		vs_variables[i].d_value = stod(vs_command[i]);
	}
	AssignValues(pc_root);
}
//sets compute values to variables (setComputeValue helper)
void CTree::AssignValues(CNode* pc_node)
{
	if(CNode::isLetter(pc_node->s_value[0]))
		pc_node->d_computeValue = GetValue(pc_node->s_value);
	for (int i = 0; i < pc_node->vc_children.size(); i++)
		AssignValues(pc_node->vc_children[i]);
}
//returns integer value of given variable
double CTree::GetValue(string s_var)
{
	for (int i = 0; i < vs_variables.size(); i++)
		if (vs_variables[i].s_value == s_var)
			return vs_variables[i].d_value;
	return DBL_MAX;
}
//computes entered expression with given variables values
void CTree::computeExpression()
{
	while (pc_root->d_computeValue == DBL_MAX)
		ComputeExpression(pc_root);
}
//computes entered expression with given variables values (computeExpression helper)
void CTree::ComputeExpression(CNode* pc_node)
{
	if (pc_node->getMaxChildren() == 2)
	{
		if (pc_node->vc_children[0]->d_computeValue != DBL_MAX && pc_node->vc_children[1]->d_computeValue != DBL_MAX)
		{
			if (pc_node->s_value == S_OPERATOR_ADD) pc_node->d_computeValue = pc_node->vc_children[0]->d_computeValue + pc_node->vc_children[1]->d_computeValue;
			else if (pc_node->s_value == S_OPERATOR_SUB) pc_node->d_computeValue = pc_node->vc_children[0]->d_computeValue - pc_node->vc_children[1]->d_computeValue;
			else if (pc_node->s_value == S_OPERATOR_MUL) pc_node->d_computeValue = pc_node->vc_children[0]->d_computeValue * pc_node->vc_children[1]->d_computeValue;
			else if (pc_node->s_value == S_OPERATOR_DIV)
				if (pc_node->vc_children[1]->d_computeValue != 0)
					pc_node->d_computeValue = pc_node->vc_children[0]->d_computeValue / pc_node->vc_children[1]->d_computeValue;
				else
					pc_node->d_computeValue = pc_node->vc_children[0]->d_computeValue / 1;
		}
	}
	else if (pc_node->getMaxChildren() == 1)
	{
		if (pc_node->vc_children[0]->d_computeValue != DBL_MAX)
		{
			//double d_radians = (pc_node->vc_children[0]->d_computeValue * M_PI) / 180.0f;
			double d_radians = pc_node->vc_children[0]->d_computeValue;
			if (pc_node->s_value == S_FUNCTION_SIN) pc_node->d_computeValue = sin(d_radians);
			else if (pc_node->s_value == S_FUNCTION_COS) pc_node->d_computeValue = cos(d_radians);
		}
	}
	for (int i = 0; i < pc_node->getMaxChildren(); i++)
		ComputeExpression(pc_node->vc_children[i]);
}
//returns current variables number
int CTree::getVariablesNumber()
{
	return vs_variables.size();
}
//checks if given variable is already in the vector of variables
bool CTree::isVariableAlreadyInVector(string s_var)
{
	for (int i = 0; i < vs_variables.size(); i++)
		if (vs_variables[i].s_value == s_var)
			return true;
	return false;
}
//adds missing variables to a variables vector
void CTree::UpdateVariables(CNode* pc_node)
{
	if (pc_node->isLetter(pc_node->s_value[0]) && pc_node->getMaxChildren() == 0)
		if (!isVariableAlreadyInVector(pc_node->s_value))
		{
			s_variable s_var = { pc_node->s_value, -1 };
			vs_variables.push_back(s_var);
		}
	for (int i = 0; i < pc_node->vc_children.size(); i++)
		UpdateVariables(pc_node->vc_children[i]);
}
//returns result of the expression
double CTree::getResult()
{
	return pc_root->d_computeValue;
}
//returns the tree in pre order in a string
string	CTree::toString()
{
	return ToString(pc_root);
}
//returns the tree in pre order in a string (toString helper)
string	CTree::ToString(CNode* pc_node)
{
	string s_result = "";
	s_result += pc_node->s_value + " ";
	for (int i = 0; i < pc_node->getMaxChildren(); i++)
		s_result += ToString(pc_node->vc_children[i]);
	return s_result;
}
//enters random expression into a tree
void CTree::randomTree(int i_maxLength) 
{
	enterExpression(GenerateTree(i_maxLength));
}
//generates random expression and returns in a string
string CTree::GenerateTree(int i_maxLength) 
{
	string s_operators[4] = { "+", "-", "*", "/" };
	string s_trigFunctions[2] = { "sin", "cos" };
	string s_variables[2] = { "x", "y" };
	string s_digits[9] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	if (i_maxLength < 3)
		i_maxLength = 3;
	string s_result = "";
	int i_random = rand() % 101;
	if (i_random < 75)
	{
		s_result += s_operators[rand() % 4] + " ";
	}
	else
	{
		s_result += s_trigFunctions[rand() % 2] + " ";
	}
	for (int i = 0; i < (i_maxLength - 3); i++)
	{
		i_random = rand() % 101;
		if (i_random < 31)
		{
			s_result += s_variables[rand() % 2] + " ";
		}
		else if (i_random >=31 && i_random < 51) 
		{
			s_result += s_digits[rand() % 9] + " ";
		}
		else if (i_random >= 51 && i_random < 91) 
		{
			s_result += s_operators[rand() % 4] + " ";
		}
		else  if(i_random >= 91 && i_random < 101)
		{
			s_result += s_trigFunctions[rand() % 2] + " ";
		}
	}
	s_result.pop_back();
	return s_result;
}
//replaces one of this tree subtrees with one of pc_other subtrees
void CTree::crossTrees(CTree* pc_other, int i_pc) 
{
	if (rand() % 100 < i_pc)
	{
		if (this->pc_root->getMaxChildren() == 2 && (pc_other->pc_root->getMaxChildren() == 2 || pc_other->pc_root->getMaxChildren() == 1))
		{
			CNode* pc_othersLeftSubtree = pc_other->pc_root->vc_children[0];
			pc_other->pc_root->vc_children[0] = this->pc_root->vc_children[1];
			pc_other->pc_root->vc_children[0]->setParent(pc_other->pc_root);
			this->pc_root->vc_children[1] = pc_othersLeftSubtree;
			this->pc_root->vc_children[1]->setParent(this->pc_root);
		}
		else if (this->pc_root->getMaxChildren() == 1 && pc_other->pc_root->getMaxChildren() == 2)
		{
			CNode* pc_othersLeftSubtree = pc_other->pc_root->vc_children[1];
			pc_other->pc_root->vc_children[1] = this->pc_root->vc_children[0];
			pc_other->pc_root->vc_children[1]->setParent(pc_other->pc_root);
			this->pc_root->vc_children[0] = pc_othersLeftSubtree;
			this->pc_root->vc_children[0]->setParent(this->pc_root);
		}
		else
		{
			CNode* pc_othersLeftSubtree = pc_other->pc_root->vc_children[0];
			pc_other->pc_root->vc_children[0] = this->pc_root->vc_children[0];
			pc_other->pc_root->vc_children[0]->setParent(pc_other->pc_root);
			this->pc_root->vc_children[0] = pc_othersLeftSubtree;
			this->pc_root->vc_children[0]->setParent(this->pc_root);
		}
	}
}
//generates random leaf and returns it in a string
string CTree::GenerateLeaf() 
{
	int i_random = rand() % 101;
	if (i_random < 50)
		if (i_random < 25)
			return S_VARIABLE_X;
		else
			return S_VARIABLE_Y;
	else return to_string(rand() % 10);
}
//returns pointer to a leaf
child CTree::GetRandomLeaf()
{
	CNode *pc_current = pc_root;
	int i_index;
	while (pc_current->getMaxChildren() != 0)
		if (pc_current->getMaxChildren() == 2)
		{
			if (rand() % 100 < 50)
			{
				pc_current = pc_current->vc_children[1];
				i_index = 1;
			}
			else
			{
				pc_current = pc_current->vc_children[0];
				i_index = 0;
			}
		}
		else
		{
			pc_current = pc_current->vc_children[0];
			i_index = 0;
		}
	child c_node = { pc_current, i_index };
	return c_node;
}
//returns pointer to a leaf
child CTree::GetRandomNode()
{
	child c_node = {pc_root, -1};
	CNode *pc_current = pc_root;
	while (pc_current->vc_children[0]->getMaxChildren() != 0)
	{
		if (pc_current->getMaxChildren() == 2)
		{
			if (pc_current->vc_children[1]->getMaxChildren() != 0)
			{
				if (rand() % 100 < 50)
				{
					pc_current = pc_current->vc_children[1];
					c_node = { pc_current, 1 };
				}
				else
				{
					pc_current = pc_current->vc_children[0];
					c_node = { pc_current, 0};
				}
			}
			else 
			{
				pc_current = pc_current->vc_children[0];
				c_node = { pc_current, 0 };
			}
		}
		else
		{
			pc_current = pc_current->vc_children[0];
			c_node = { pc_current, 0 };
		}
		if (rand() % 100 < 33)
			return c_node;
	}
	return c_node;
}
//one of the nodes or leafes transforms into random node or leaf
void CTree::mutateTree(int i_pm) 
{
	if (rand() % 100 < i_pm) 
	{
		int i_random = rand() % 100;
		if (i_random < 14)// leaf -> new leaf
		{ 
			CNode* pc_newLeaf = new CNode(GenerateLeaf(),0);
			child pc_randomLeaf = GetRandomLeaf();
			pc_newLeaf->setParent(pc_randomLeaf.pc_node->pc_parent);
			pc_randomLeaf.pc_node->pc_parent->vc_children[pc_randomLeaf.i_index] = pc_newLeaf;
			delete pc_randomLeaf.pc_node;
		}
		else if(i_random < 28)// leaf -> new node
		{
			CTree* pc_newNode = new CTree();
			pc_newNode->enterExpression(GenerateTree(0));
			child pc_randomLeaf = GetRandomLeaf();
			pc_newNode->pc_root->setParent(pc_randomLeaf.pc_node->pc_parent);
			pc_randomLeaf.pc_node->pc_parent->vc_children[pc_randomLeaf.i_index] = pc_newNode->pc_root;
			delete pc_randomLeaf.pc_node;
		}
		else if (i_random < 42)// node -> new leaf
		{
			CNode* pc_newLeaf = new CNode(GenerateLeaf(), 0);
			child pc_randomNode = GetRandomNode();
			if (pc_randomNode.i_index != -1)
			{
				pc_newLeaf->setParent(pc_randomNode.pc_node->pc_parent);
				pc_randomNode.pc_node->pc_parent->vc_children[pc_randomNode.i_index] = pc_newLeaf;
				delete pc_randomNode.pc_node;
			}
		}
		else if (i_random < 56)// node -> new node
		{
			CTree* pc_newNode = new CTree();
			pc_newNode->enterExpression(GenerateTree(0));
			child pc_randomNode = GetRandomNode();
			if (pc_randomNode.i_index != -1)
			{
				pc_newNode->pc_root->setParent(pc_randomNode.pc_node->pc_parent);
				pc_randomNode.pc_node->pc_parent->vc_children[pc_randomNode.i_index] = pc_newNode->pc_root;
				delete pc_randomNode.pc_node;
			}
		}
		else if (i_random < 70)// node <-> node
		{
			child pc_firstRandomNode = GetRandomNode();
			child pc_secondRandomNode = GetRandomNode();
			if (pc_firstRandomNode.pc_node != pc_secondRandomNode.pc_node && pc_firstRandomNode.i_index != -1 && pc_secondRandomNode.i_index != -1)
			{
				pc_firstRandomNode.pc_node->pc_parent->vc_children[pc_firstRandomNode.i_index] = pc_secondRandomNode.pc_node;
				pc_secondRandomNode.pc_node->pc_parent->vc_children[pc_secondRandomNode.i_index] = pc_firstRandomNode.pc_node;
				pc_firstRandomNode.pc_node->setParent(pc_secondRandomNode.pc_node->pc_parent);
				pc_secondRandomNode.pc_node->setParent(pc_firstRandomNode.pc_node->pc_parent);
			}
		}
		else if (i_random < 84)// leaf <-> leaf
		{
			child pc_firstRandomLeaf = GetRandomLeaf();
			child pc_secondRandomLeaf = GetRandomLeaf();
			if (pc_firstRandomLeaf.pc_node != pc_secondRandomLeaf.pc_node)
			{
				pc_firstRandomLeaf.pc_node->pc_parent->vc_children[pc_firstRandomLeaf.i_index] = pc_secondRandomLeaf.pc_node;
				pc_secondRandomLeaf.pc_node->pc_parent->vc_children[pc_secondRandomLeaf.i_index] = pc_firstRandomLeaf.pc_node;
				pc_firstRandomLeaf.pc_node->setParent(pc_secondRandomLeaf.pc_node->pc_parent);
				pc_secondRandomLeaf.pc_node->setParent(pc_firstRandomLeaf.pc_node->pc_parent);
			}
		}
		else  if (i_random < 100)
		{// node <-> leaf
			child pc_randomNode = GetRandomNode();
			child pc_randomLeaf = GetRandomLeaf();
			if (pc_randomNode.i_index != -1)
			{
				pc_randomNode.pc_node->pc_parent->vc_children[pc_randomNode.i_index] = pc_randomLeaf.pc_node;
				pc_randomLeaf.pc_node->pc_parent->vc_children[pc_randomLeaf.i_index] = pc_randomNode.pc_node;
				pc_randomNode.pc_node->setParent(pc_randomLeaf.pc_node->pc_parent);
				pc_randomLeaf.pc_node->setParent(pc_randomNode.pc_node->pc_parent);
			}
		}
	}
}
//returns measure of tree
double	CTree::getMeasure() 
{
	return d_measure;
}
//updates measure of tree
void	CTree::updateMeasure() 
{
	d_measure += pow((pc_root->d_computeValue - vs_variables[2].d_value),2);
}
//zeros root of tree
void	CTree::zeroRoot() 
{
	pc_root->d_computeValue = DBL_MAX;
}
//zeros measure of tree
void	CTree::zeroMeasure() 
{
	d_measure = 0;
}