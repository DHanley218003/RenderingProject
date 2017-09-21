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
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	aitVertex tempVertex(glm::vec3(0.000000, 1.000000, 0.000000), glm::vec3(0.000000, 0.000000, 1.000000));
	char buff[BUFFSIZE];
	float tempx;
	float tempy;
	float tempz;

	std::ifstream infile(path);
	std::stringstream ss;

	while(true)
	{
		if(infile.eof())
			break;
		infile.getline( buff, BUFFSIZE);
		ss << buff;
		ss.getline( buff, 9, ' ' );
		if(buff[0]=='o' || buff[0] == '#')
		{}
		if(buff[0]=='v')
		{
			if(buff[1]=='n')
			{
				ss.get(buff, 9, ' ');
				tempx = atof(buff);
				ss.get(buff, 9, ' ');
				tempy = atof(buff);
				ss.get(buff, 9, ' ');
				tempz = atof(buff);
				normals.push_back(glm::vec3(tempx, tempy, tempz));
			}
			else
			{
				ss.get(buff, 9, ' ');
				tempx = atof(buff);
				ss.get(buff, 9, ' ');
				tempy = atof(buff);
				ss.get(buff, 9, ' ');
				tempz = atof(buff);
				vertices.push_back(glm::vec3(tempx, tempy, tempz));
			}
		}
		if(buff[0]=='s')
		{}
		if(buff[0]=='f')
		{}
	}
	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		tempVertex = (vertices[i], normals[i]);
		vertices.push_back(tempVertex);
	}
	return true;
}