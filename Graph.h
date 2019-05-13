#pragma once
#include<vector>
#include<iostream>

using namespace std;

class Graph
{
private:

	class Vertex
	{
	private:
		int mNumber;
		int value;
		vector<int> mNextVertices;

	public:
		Vertex(int number)
		{
			mNumber = number;
		}

		Vertex(int number, vector<int> nextVertices)
		{
			mNumber = number;
			mNextVertices = nextVertices;
		}

		void setNextVertices(vector<int> nextVertices)
		{
			mNextVertices = nextVertices;
		}

		void addNextVertex(int nextVertex)
		{
			mNextVertices.push_back(nextVertex);
		}

		int getNumber() 
		{
			return mNumber;
		}

		vector<int> getNextVertices()
		{
			return mNextVertices;
		}

		int getNumberOfNextVerices()
		{
			return mNextVertices.size();
		}
		
		void show()
		{
			cout << mNumber << ": ";
			for (int i = 0; i < mNextVertices.size(); ++i)
			{
				cout << mNextVertices[i] << " ";
			}
			cout << endl;
		}

		void increaseValue(int valueInc) {
			value += valueInc;
		}

		int getNumberOfTransmitersByModularity(int modularity) {
			if(value%modularity != 0) {
				return value/modularity + 1;
			} else {
				return value/modularity;
			}
		}
	};

	int mNumberOfEdges;
	int mNumberOfVertices;
	vector<Vertex> mEdges;

	void createEdge(int vertex1, int vertex2);
	void loadGraphFormFile(string vertexFile, string edgesFile);
	int getNumberOfNextVerticesForVertex(int vertex);
	vector<int> getNextVerticesForVertex(int vertex);
	bool isVertexVisitedInPath(int vertex, vector<int> path);
	int getVertexIndexByNumber(int vertexNumber);
public:
	Graph();
	Graph(string vertexFile, string edgesFile);
	vector<vector<int>> getPathsForDemand(int srcVertexNum, int dstVertexNum);
	void applyDemand(vector<int> path, int demandValue);
	void show();
	int getNumberOfTansmiters(int modularity);
};
