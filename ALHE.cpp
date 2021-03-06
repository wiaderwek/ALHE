// PSZT_Ciastka.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <ctime>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include "Graph.h"
#include <climits>
#include <time.h>

#define DEFAULT_NUM_OF_ITERATIONS 100
#define SIZE_OF_POPULATION 1000
#define NUM_OF_BITS_FOR_DEMAND_VALUE 8
#define REPRODUCTION_PROBABILITY 99
#define REPRODUCTION_POINT_DEVIDER 10
#define MUTATE_MEMBER_PROBABILITY 1
#define MUTATE_GEN_PROBABILITY 1
#define MODULARITY_DIVIDER 1.8

using namespace std;

bool contain(int index, vector<int> tab) {
	for (int j = 0; j < tab.size(); ++j) {
		if (tab[j] == index)
			return true;
	}
	return false;
}

int power(int a, int b) {
	if (b == 0) {
		return 1;
	}

	return a * power(a, b - 1);
}

int abs(int value) {
	if(value < 0) {
		return value*(-1);
	}

	return value;
}

vector<bool> genPopulationMember(const int N) {
	vector<bool> member;

	for (int i = 0; i < N * NUM_OF_BITS_FOR_DEMAND_VALUE; ++i) {
		member.push_back(((int)rand()) % 2);
	}

	return member;
}

void printFenotype(vector<bool> member, vector<pair<int, vector<vector<int>>>>& demandWithAvailablePaths, int modularity, Graph graph) {
	
	
	int valueOfAdaptationFunction = 0;
	int offset = 0;
	for(pair<int, vector<vector<int>>> pairDemanAndPaths : demandWithAvailablePaths) {
		int valueOfAllPathsForDemand = 0;
		for(int i = 0; i < pairDemanAndPaths.second.size(); ++i) {
			int valueFromPathForDemand = 0;
			for(int j = 0; j < NUM_OF_BITS_FOR_DEMAND_VALUE; ++j) {
				valueFromPathForDemand += power(2, NUM_OF_BITS_FOR_DEMAND_VALUE - 1 - j)*member[offset + j];
			}
			offset+=NUM_OF_BITS_FOR_DEMAND_VALUE;

			//cout << "demand: " << demand << " path: ";
			for(int k = 0; k < pairDemanAndPaths.second[i].size(); ++k) {
				cout << pairDemanAndPaths.second[i][k] << " -> ";
			}
			cout << " value: " << valueFromPathForDemand << endl;
			valueOfAllPathsForDemand += valueFromPathForDemand;
		}
	}
}


int adaptationFunction(vector<bool> member, vector<pair<int, vector<vector<int>>>>& demandWithAvailablePaths, int modularity, Graph graph) {
	int valueOfAdaptationFunction = 0;
	int offset = 0;
	for(pair<int, vector<vector<int>>> pairDemanAndPaths : demandWithAvailablePaths) {
		int valueOfAllPathsForDemand = 0;
		for(int i = 0; i < pairDemanAndPaths.second.size(); ++i) {
			int valueFromPathForDemand = 0;
			for(int j = 0; j < NUM_OF_BITS_FOR_DEMAND_VALUE; ++j) {
				valueFromPathForDemand += power(2, NUM_OF_BITS_FOR_DEMAND_VALUE - 1 - j)*member[offset + j];
			}
			offset += NUM_OF_BITS_FOR_DEMAND_VALUE;

			if(valueFromPathForDemand != 0) {
				graph.applyDemand(pairDemanAndPaths.second[i], valueFromPathForDemand);
			}
			valueOfAllPathsForDemand += valueFromPathForDemand;
		}
		valueOfAdaptationFunction += abs(pairDemanAndPaths.first-valueOfAllPathsForDemand);
		if (pairDemanAndPaths.first-valueOfAllPathsForDemand > 0) {
			valueOfAdaptationFunction += power(2, NUM_OF_BITS_FOR_DEMAND_VALUE);
		}
	}

	valueOfAdaptationFunction += (modularity / MODULARITY_DIVIDER) * graph.getNumberOfTansmiters(modularity);
	return valueOfAdaptationFunction;
}

int calculateNumberOfTransmitersForMember(vector<bool> member, vector<pair<int, vector<vector<int>>>>& demandWithAvailablePaths, int modularity, Graph graph) {
	int offset = 0;
	for(pair<int, vector<vector<int>>> pairDemanAndPaths : demandWithAvailablePaths) {
		for(int i = 0; i < pairDemanAndPaths.second.size(); ++i) {
			int valueFromPathForDemand = 0;
			for(int j = 0; j < NUM_OF_BITS_FOR_DEMAND_VALUE; ++j) {
				valueFromPathForDemand += power(2, NUM_OF_BITS_FOR_DEMAND_VALUE - 1 - j)*member[offset + j];
			}
			offset += NUM_OF_BITS_FOR_DEMAND_VALUE;
			if(valueFromPathForDemand != 0) {
				graph.applyDemand(pairDemanAndPaths.second[i], valueFromPathForDemand);
			}
		}
	}
	return graph.getNumberOfTansmiters(modularity);
}

bool isCorrectPopulation(vector<bool> member, vector<pair<int, vector<vector<int>>>>& demandWithAvailablePaths) {
	int offset = 0;
	for(pair<int, vector<vector<int>>> pairDemanAndPaths : demandWithAvailablePaths) {
		int valueOfAllPathsForDemand = 0;
		for(int i = 0; i < pairDemanAndPaths.second.size(); ++i) {
			int valueFromPathForDemand = 0;
			for(int j = 0; j < NUM_OF_BITS_FOR_DEMAND_VALUE; ++j) {
				valueFromPathForDemand += power(2, NUM_OF_BITS_FOR_DEMAND_VALUE - 1 - j)*member[offset + j];
			}
			valueOfAllPathsForDemand += valueFromPathForDemand;
			offset += NUM_OF_BITS_FOR_DEMAND_VALUE;
		}
		if (pairDemanAndPaths.first-valueOfAllPathsForDemand > 0) {
			return false;
		}
	}

	return true;
}

void reproduction(vector<bool> firstMember, vector<bool> secondMember, vector<vector<bool>> &reproducedMembers, vector<pair<int, vector<vector<int>>>>& demandWithAvailablePaths) {
	vector<bool> reproducedMember1(firstMember.size());
	vector<bool> reproducedMember2(firstMember.size());

	vector<int> indexTab;
	int numberOfPaths = firstMember.size()/NUM_OF_BITS_FOR_DEMAND_VALUE;
  	if (rand() % 100 <= REPRODUCTION_PROBABILITY) {
		while (true) {
			int index = (int)(rand() % firstMember.size());
			if (!contain(index, indexTab)) {
				indexTab.push_back(index);
			}
			if (indexTab.size() >= (int)(firstMember.size()/REPRODUCTION_POINT_DEVIDER)) {
				break;
			}
		}

		sort(indexTab.begin(), indexTab.end());
		int iteration = 0;
		for (int i = 0; i < firstMember.size(); i++)
		{
			if (i < indexTab[iteration] || iteration == indexTab.size() - 1) {
				if (iteration % 2 == 0) {
					reproducedMember1[i] = firstMember[i];
					reproducedMember2[i] = secondMember[i];
				}
				else {
					reproducedMember1[i] = secondMember[i];
					reproducedMember2[i] = firstMember[i];
				}
			}
			else
			{
				++iteration;
				--i;
			}
		}
	}
	else
	{
		reproducedMember1 = firstMember;
		reproducedMember2 = secondMember;
	}

	reproducedMembers.push_back(reproducedMember1);
	reproducedMembers.push_back(reproducedMember2);
}

void mutate(vector<vector<bool>> &reproducedMembers) {
	for (int k = 0; k < reproducedMembers.size(); ++k) {
		if (rand() % 100 <= MUTATE_MEMBER_PROBABILITY) {
			for (int i = 0; i < reproducedMembers[k].size(); i += NUM_OF_BITS_FOR_DEMAND_VALUE) {
				if (rand() % 100 <= MUTATE_GEN_PROBABILITY) {
					for (int j = 0; j < NUM_OF_BITS_FOR_DEMAND_VALUE; ++j) {
						reproducedMembers[k][i+j] = ((int)rand()) % 2;
					}
				}
			}
		}
	}
}

void loadPathsForDemands(vector<pair<int, vector<vector<int>>>>& demandWithAvailablePaths, string demandsFileName, Graph graph) {
	fstream demandFile;

	demandFile.open(demandsFileName, ios::in);

	if (demandFile.good()) {
		cout << "Get access to the file!" << '\n';

		int src, dst, demandValue;

		demandFile >> src;
		demandFile >> dst;
		demandFile >> demandValue;

		while (!demandFile.eof()) {

			if(demandValue > 0 && demandValue <= power(2, NUM_OF_BITS_FOR_DEMAND_VALUE)) {
				pair<int, vector<vector<int>>> pairDemandValueAvailablePaths = make_pair(demandValue, graph.getPathsForDemand(src, dst));
				demandWithAvailablePaths.push_back(pairDemandValueAvailablePaths);
			}

			demandFile >> src;
			demandFile >> dst;
			demandFile >> demandValue;
		}
		demandFile.close();
	}
}

int getNumberOfAvailablePaths(vector<pair<int, vector<vector<int>>>>& demandsWithAvailablePaths) {
	int numberOfAvailablePaths = 0;
	for(pair<int, vector<vector<int>>> pair : demandsWithAvailablePaths) {
		numberOfAvailablePaths += pair.second.size();
	}

	return numberOfAvailablePaths;
}

int main(int argc, char *argv[])
{
	if (argc < 5)
	{
		perror("Not enough file given!");
		exit(1);
	}

	string verticesFile(argv[1]);
	string edgesFile(argv[2]);
	string demandsFileName(argv[3]);

	int modularity = stoi(argv[4]);
	int numberOfInterations;

	if(argc > 5) {
		numberOfInterations = stoi(argv[5]);
	} else {
		numberOfInterations = DEFAULT_NUM_OF_ITERATIONS;
	}

	clock_t tStart = clock();

	time_t t;
	srand(time(&t));

	Graph graph(verticesFile, edgesFile);
	vector<pair<int, vector<vector<int>>>> demandsWithAvailablePaths;
	loadPathsForDemands(demandsWithAvailablePaths, demandsFileName, graph);

	int numberOfAvailablePaths = getNumberOfAvailablePaths(demandsWithAvailablePaths);

	vector<vector<bool>> population;

	vector <pair<int, vector<bool>>> memberWithMarkMap;

	for (int i = 0; i < SIZE_OF_POPULATION; ++i) {
		population.push_back(genPopulationMember(numberOfAvailablePaths));

		memberWithMarkMap.push_back(pair<int, vector<bool>>(adaptationFunction(population[i], demandsWithAvailablePaths, modularity, graph), population[i]));
	}

	int bestResultForIteration = INT_MAX;
	vector<bool> theBestResult;
	int iteration = 0;

	while (true) {

		vector<vector<bool>> reproducedMembers;
		for (int i = 0; i < SIZE_OF_POPULATION - 1; ++i) {
			reproduction(memberWithMarkMap[i].second, memberWithMarkMap[i + 1].second, reproducedMembers, demandsWithAvailablePaths);
		}

		mutate(reproducedMembers);
		for (int i = 0; i < reproducedMembers.size(); ++i) {
			memberWithMarkMap.push_back(pair<int, vector<bool>>(adaptationFunction(reproducedMembers[i], demandsWithAvailablePaths, modularity, graph), reproducedMembers[i]));
		}

		sort(memberWithMarkMap.begin(), memberWithMarkMap.end());
		memberWithMarkMap.resize(SIZE_OF_POPULATION);

		for (int i = 0; i < memberWithMarkMap.size(); ++i) {

			if (isCorrectPopulation(memberWithMarkMap[i].second, demandsWithAvailablePaths)) {
				int result = memberWithMarkMap[i].first;
				if (result < bestResultForIteration) {
					bestResultForIteration = result;
					iteration = 0;
					std::cout << "Number of needed transmiters: " << 
							calculateNumberOfTransmitersForMember(memberWithMarkMap[i].second, demandsWithAvailablePaths, modularity, graph) << endl;
					i = memberWithMarkMap.size();
					break;
				}
				else if (result == bestResultForIteration) {
					++iteration;
					if (iteration == numberOfInterations) {
						theBestResult = memberWithMarkMap[i].second;
						printFenotype(theBestResult, demandsWithAvailablePaths, modularity, graph);
						std::cout << "Finall solution after "<<iteration<<" iterations: " << endl;
						std::cout << "Number of needed transmiters: " << 
							calculateNumberOfTransmitersForMember(memberWithMarkMap[i].second, demandsWithAvailablePaths, modularity, graph) << endl;
						
						printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

						return 0;
					}
				}
			}
		}
	}
}
