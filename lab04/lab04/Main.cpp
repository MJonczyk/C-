#include <iostream>
#include <fstream>
#include <sstream>
#include "CTree.h"
using namespace std;
int main() 
{
	srand(time(NULL));
	int i_np = 1024; // size of population			i_np = 2^n
	int i_ni = 10;	 // number of iterations		i_ni = 1,2,3,...,10
	int i_pk = 20;	 // probability of crossing		P(k) = i_pk/100
	int i_pm = 5;	 // probability of mutation		P(m) = i_pm/100
	int i_pow = 2;
	string s_line;
	string s_filePath = "C:\\sin_x.txt";
	vector<CTree*> vc_treePopulation;
	vector<CTree*> vc_nextTreePopulation;
	CTree* pc_tree;
	//*******INITIALIZATION*******//
	for (int i = 0; i < i_np; i++)
	{
		pc_tree = new CTree();
		pc_tree->randomTree(10);
		vc_treePopulation.push_back(pc_tree);
	}
	//*******EVALUATION*******//
	ifstream f_dataFile = ifstream();
	for (int i = 0; i < i_np; i++)
	{
		f_dataFile.open(s_filePath);
		if (f_dataFile.is_open())
		{
			while (getline(f_dataFile, s_line))
			{
				vector<string> vs_values;
				string s_lineToAnalyze = s_line, s_currentValue;
				int i_spaceIndex = 0;
				while (i_spaceIndex != -1) {
					i_spaceIndex = s_lineToAnalyze.find(";");
					s_currentValue = s_lineToAnalyze.substr(0, i_spaceIndex);
					vs_values.push_back(s_currentValue);
					s_lineToAnalyze = s_lineToAnalyze.substr(i_spaceIndex + 1);
				}
				for (int i = 1; i < vs_values.size(); i++)
					vs_values[i].erase(0, 1);
				vs_values.pop_back();
				vc_treePopulation[i]->setComputeValue(vs_values);
				vc_treePopulation[i]->computeExpression();
				vc_treePopulation[i]->updateMeasure();
				vc_treePopulation[i]->zeroRoot();
			}
			f_dataFile.close();
		}
		else cout << "Unable to open file";
	}
	for (int j = 0; j < i_ni; j++)
	{
		for (int i = 0; i < (int)(i_np / i_pow); i++)
		{
			//*******SELECTION*******//
			//cout << vc_treePopulation.size() << endl;
			if (vc_treePopulation[i]->getMeasure() <= vc_treePopulation[i + (i_np / i_pow)]->getMeasure())
			{
				vc_nextTreePopulation.push_back(vc_treePopulation[i]);
				delete vc_treePopulation[i + (i_np / i_pow)];
			}
			else
			{
				vc_nextTreePopulation.push_back(vc_treePopulation[i + (i_np / i_pow)]);
				delete vc_treePopulation[i];
			}
		}
		i_pow *= 2;
		vc_treePopulation.swap(vc_nextTreePopulation);
		vc_nextTreePopulation.clear();
		//*******CROSSING*******//
		for (int i = 0; i < (int)(i_np / i_pow); i++) 
		{
			cout << i << ": "<< vc_treePopulation[i]->toString() << endl;
			cout << "wynik = " << vc_treePopulation[i]->getMeasure() << endl;
			vc_treePopulation[i]->crossTrees(vc_treePopulation[i + (i_np / i_pow)], i_pk);
		}
		//*******MUTATION*******//
		//*******EVALUATION*******//
		for (int i = 0; i < (int)(i_np / i_pow); i++)
		{
			vc_treePopulation[i]->mutateTree(i_pm);
			vc_treePopulation[i]->zeroMeasure();
			f_dataFile.open(s_filePath);
			if (f_dataFile.is_open())
			{
				while (getline(f_dataFile, s_line))
				{
					vector<string> vs_values;
					string s_lineToAnalyze = s_line, s_currentValue;
					int i_spaceIndex = 0;
					while (i_spaceIndex != -1) {
						i_spaceIndex = s_lineToAnalyze.find(";");
						s_currentValue = s_lineToAnalyze.substr(0, i_spaceIndex);
						vs_values.push_back(s_currentValue);
						s_lineToAnalyze = s_lineToAnalyze.substr(i_spaceIndex + 1);
					}
					for (int i = 1; i < vs_values.size(); i++)
						vs_values[i].erase(0, 1);
					vs_values.pop_back();
					vc_treePopulation[i]->setComputeValue(vs_values);
					vc_treePopulation[i]->computeExpression();
					vc_treePopulation[i]->updateMeasure();
					vc_treePopulation[i]->zeroRoot();
				}
			f_dataFile.close();
			}
			else
				cout << "Unable to open file";
		}
	}	
	i_pow /= 2;
	/*
	for (int i = 0; i < (int)(i_np / i_pow); i++)
	{
		cout << to_string(i) << "    "<< vc_treePopulation[i]->getMeasure() << "    " << vc_treePopulation[i]->toString() << endl;
	}
	*/
	ofstream f_resultFile("C:\\funkcja.txt");	
	if (f_resultFile.is_open())
	{
		
		f_resultFile << vc_treePopulation[0]->toString() << endl;
		f_resultFile << vc_treePopulation[0]->getMeasure();
		f_resultFile.close();
	}
	printf("wynik: %.17g\n", vc_treePopulation[0]->getMeasure());
	int p;
	cin >> p;
	return 0;
}