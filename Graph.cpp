#include "Graph.h"
#include <fstream>
#include <iostream>
#include <queue>

Graph::Graph()
{
	mNumberOfEdges = 0;
	mNumberOfVertices = 0;
}

Graph::Graph(string vertexFile, string edgesFile)
{
	mNumberOfEdges = 0;
	mNumberOfVertices = 0;
	loadGraphFormFile(vertexFile, edgesFile);
}

void Graph::loadGraphFormFile(string vertexFile, string edgesFile)
{
	fstream fileWithVerices;
	fileWithVerices.open(vertexFile, ios::in);

	if (fileWithVerices.good())
	{
		std::cout << "Get access to the file!" << std::endl;
		int vertexNumber;
		fileWithVerices >> vertexNumber;
		while (!fileWithVerices.eof())
		{
            Vertex ver(vertexNumber);
			mEdges.push_back(ver);
			fileWithVerices >> vertexNumber;
			++mNumberOfVertices;
		}
		fileWithVerices.close();
	}

	fstream fileWithEdges;
	fileWithEdges.open(edgesFile, ios::in);

	if (fileWithEdges.good())
	{
		std::cout << "Get access to the file!" << std::endl;
		int vertex1, vertex2;
		fileWithEdges >> vertex1;
		fileWithEdges >> vertex2;

		while (!fileWithEdges.eof())
		{
			createEdge(vertex1, vertex2);

			fileWithEdges >> vertex1;
			fileWithEdges >> vertex2;

			++mNumberOfEdges;
		}
		fileWithEdges.close();
	}
}

void Graph::createEdge(int vertex1, int vertex2)
{
	int added = 0;
	for (int i = 0; i < mEdges.size(); ++i)
	{
		if (mEdges[i].getNumber() == vertex1)
		{
			mEdges[i].addNextVertex(vertex2);
			++added;
		}
		else if (mEdges[i].getNumber() == vertex2)
		{
			mEdges[i].addNextVertex(vertex1);
			++added;
		}
		else if (added == 2)
		{
			return;
		}
	}
}

int Graph::getNumberOfNextVerticesForVertex(int vertex)
{
	for (int i = 0; i < mEdges.size(); ++i)
	{
		if (mEdges[i].getNumber() == vertex)
		{
			return mEdges[i].getNumberOfNextVerices();
		}
	}
}

vector<int> Graph::getNextVerticesForVertex(int vertex)
{
	for (int i = 0; i < mEdges.size(); ++i)
	{
		if (mEdges[i].getNumber() == vertex)
		{
			return mEdges[i].getNextVertices();
		}
	}
}

bool Graph::isVertexVisitedInPath(int vertex, vector<int> path)
{
	for (int i = 0; i < path.size(); ++i)
	{
		if (path[i] == vertex)
		{
			return true;
		}
	}

	return false;
}

void Graph::show()
{
	for (int i = 0; i < mEdges.size(); ++i)
	{
		cout << mEdges[i].getNumber() << " : ";
		for (auto vertex : getNextVerticesForVertex(mEdges[i].getNumber()))
		{
			cout << vertex << " ";
		}
		cout << endl;
	}
}

vector<vector<int>> Graph::getPathsForDemand(int srcVertexNum, int dstVertexNum) {
    vector<vector<int>> pathsForDemand;
    queue<vector<int>> pathsToAnalize;

	vector<int> path;
	path.push_back(srcVertexNum);

	pathsToAnalize.push(path);

	while (!pathsToAnalize.empty())
	{
		path = pathsToAnalize.front();

		pathsToAnalize.pop();

		int last = path[path.size() - 1];

		if (last == dstVertexNum)
		{
			pathsForDemand.push_back(path);
		}

		for (int nextVertex : getNextVerticesForVertex(last))
		{
			if (!isVertexVisitedInPath(nextVertex, path))
			{
				vector<int> newPath(path);
				newPath.push_back(nextVertex);
				pathsToAnalize.push(newPath);
			}
		}
	}

    cout <<  "Paths number for demand: " << srcVertexNum << " - " << dstVertexNum << " :   " << pathsForDemand.size() << endl;
    return pathsForDemand;
}

int Graph::getVertexIndexByNumber(int vertexNumber) {
    for(int i = 0; i < mEdges.size(); ++i) {
        if (mEdges[i].getNumber() == vertexNumber) {
            return i;
        }
    }
}

void Graph::applyDemand(vector<int> path, int demandValue) {
    for(int vertexNumber : path) {
        //cout << vertexNumber << " inc " << demandValue << endl;
        mEdges[getVertexIndexByNumber(vertexNumber)].increaseValue(demandValue);
    }
}

int Graph::getNumberOfTansmiters(int modularity) {
    int numberOfTransmiters = 0;
    for(int i = 0; i < mEdges.size(); ++i) {
        numberOfTransmiters += mEdges[i].getNumberOfTransmitersByModularity(modularity);
    }
    return numberOfTransmiters;
}
