#include "aitMesh.h"
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <iostream>
#include <assert.h>

const unsigned int BUFFSIZE = 80; //each line should be less than 80 characters

bool aitMesh::loadFromObj(std::string path)
{
	// temporary vectors to store data until it's joined into the final vector
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec3> tempNormals;
	std::string temp;
	char buff[BUFFSIZE];
	float tempx, tempy, tempz; // used to create vertex and vertex normals
	unsigned int x, y; // used to pair up vertex and vertex normals

	std::ifstream infile(path);
	std::stringstream ss;

	while(true)
	{
		if(infile.eof()) // exits loop when it reaches the end of the file
			break;
		infile.getline( buff, BUFFSIZE);
		ss.clear();
		ss.str(""); // clears and sets the string stream buffer to empty
		ss << buff;
		if(buff[0]=='o' || buff[0] == '#') // if comment or object name, ignore
		{} // example input: o Cube
		else if(buff[0]=='v') // if vector
		{
			if(buff[1]=='n') // if vector normal
			{
				ss >> temp >> tempx >> tempy >> tempz; // example input: v -1.000000 -1.000000 1.000000
				tempNormals.push_back(glm::vec3(tempx, tempy, tempz)); 
			}
			else
			{
				std::cout << ss.str() << "/n";
				ss >> temp >> tempx >> tempy >> tempz;
				tempVertices.push_back(glm::vec3(tempx, tempy, tempz));
			}
		}
		else if(buff[0]=='s') // TODO
		{} // example input: s off
		else if(buff[0]=='f') // if faces
		{
			bool flipflop = true; // faces come in x//y, so flipflop alternates which is loaded
			while (ss >> temp) // example input: f 1//1 2//1 3//1
			{
				if (temp != "" && temp != "/" && temp != "//") // makes sure only numbers are in it
				{
					if (flipflop)
					{
						x = atoi(temp.c_str()); // must be converted to c string to use atoi
						flipflop = !flipflop;
					}
					else
					{
						y = atoi(temp.c_str());
						flipflop = !flipflop;
						vertices.push_back(aitVertex(tempVertices[x], tempNormals[y])); // join the vertex and normal, then send to vertices
					}
				}
				
			}
		}
	}
	return true;
}