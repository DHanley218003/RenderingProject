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
	std::string temp;
	char buff[BUFFSIZE];
	float tempx, tempy, tempz;
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
		if(buff[0]=='o' || buff[0] == '#')
		{}
		else if(buff[0]=='v')
		{
			if(buff[1]=='n')
			{
				ss >> temp >> tempx >> tempy >> tempz;
				tempNormals.push_back(glm::vec3(tempx, tempy, tempz));
			}
			else
			{
				std::cout << ss.str() << "/n";
				ss >> temp >> tempx >> tempy >> tempz;
				tempVertices.push_back(glm::vec3(tempx, tempy, tempz));
			}
		}
		else if(buff[0]=='s')
		{}
		else if(buff[0]=='f')
		{
			bool flipflop = true;
			while (ss >> temp)
			{
				if (temp != "" && temp != "/" && temp != "//")
				{
					if (flipflop)
					{
						x = atoi(temp.c_str());
						flipflop = !flipflop;
					}
					else
					{
						y = atoi(temp.c_str());
						flipflop = !flipflop;
					}
				}
				vertices.push_back(aitVertex(tempVertices[x], tempNormals[y]));
			}
		}
	}
	return true;
}