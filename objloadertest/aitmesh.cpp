#include "aitMesh.h"
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <iostream>
#include <assert.h>

const int BUFFSIZE = 80;

bool aitMesh::loadFromObj(std::string path)
{
	/*aitVertex v1(glm::vec3(0.000000, 1.000000, 0.000000), glm::vec3(0.000000, 0.000000, 1.000000));
	aitVertex v2(glm::vec3(-1.000000, -1.000000, 0.000000), glm::vec3(0.000000, 0.000000, 1.000000));
	aitVertex v3(glm::vec3(1.000000, -1.000000, 0.000000), glm::vec3(0.000000, 0.000000, 1.000000));
	
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);*/
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec3> tempNormals;
	char buff[BUFFSIZE];
	char buff2[BUFFSIZE];
	float tempx;
	float tempy;
	float tempz;
	unsigned int x, y;

	std::ifstream infile(path);
	std::stringstream ss;
	std::stringstream ss2;

	while(true)
	{
		if(infile.eof())
			break;
		infile.getline( buff, BUFFSIZE);
		ss.clear();
		ss.str("");
		ss << buff;
		ss >> buff2;
		if(buff2[0]=='o' || buff2[0] == '#')
		{}
		if(buff2[0]=='v')
		{
			if(buff2[1]=='n')
			{
				ss >> tempx >> tempy >> tempz;
				tempNormals.push_back(glm::vec3(tempx, tempy, tempz));
			}
			else
			{
				ss >> tempx >> tempy >> tempz;
				tempVertices.push_back(glm::vec3(tempx, tempy, tempz));
			}
		}
		if(buff2[0]=='s')
		{}
		if(buff2[0]=='f')
		{
			while (ss.rdbuf()->in_avail())
			{
				std::getline(buff, 9, "//");
				vertices.push_back(aitVertex(tempVertices[x], tempNormals[y]));
			}
		}
	}
	return true;
}