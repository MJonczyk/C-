#pragma once
#include "CNode.h"
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>      
#define M_PI 3.14159265358979323846

struct s_variable
{
	string s_value;
	double d_value;
};

struct child 
{
	CNode* pc_node;
	int i_index;
};

class CTree
{
public:
	CTree();
	CTree(const CTree &pc_other);
	~CTree();
	CTree& operator=(const CTree &pc_other);
	CTree& operator+(const CTree &pc_other);
	bool	enterExpression(string s_expression);
	void	repair();
	void	join(CTree* pc_subtree);
	CNode*	getLeafsParent();
	void	preOrder();
	void	inOrder();
	void	printVariables();
	bool	isAnyVariable();
	void	setComputeValue(vector<string> vs_command);
	void	computeExpression();
	int		getVariablesNumber();
	bool	isVariableAlreadyInVector(string s_var);
	double	getResult();
	double	getMeasure();
	void	updateMeasure();
	string	toString();
	void	randomTree(int i_maxLength);
	void	crossTrees(CTree* pc_other, int i_pc);
	void	mutateTree(int i_pm);
	child	GetRandomLeaf();
	child	GetRandomNode();
	void	zeroRoot();
	void	zeroMeasure();

private:
	void	Repair(CNode* pc_node);
	void	PreOrder(CNode* pc_node);
	void	InOrder(CNode* pc_node);
	void	AssignValues(CNode* pc_node);
	double	GetValue(string s_var);
	void	ComputeExpression(CNode* pc_root);
	void	UpdateVariables(CNode* pc_node);
	string	ToString(CNode* pc_node);
	string	GenerateTree(int i_maxLength);
	string	GenerateLeaf();
	
	double				d_measure;
	CNode*				pc_root;
	vector<s_variable>	vs_variables;
	bool				b_isRepaired;
};