#include "aitMesh.h"
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <iostream>
#include <assert.h>

const unsigned int BUFFSIZE = 80; //each line should be less than 80 characters

/* Declan Hanley - A00218003@student.ait.ie
 * TODO: Change all C functions to C++ ones
 * Read in f properly, currently still has slashes
 */
bool aitMesh::loadFromObj(std::string path)
{
	// temporary vectors to store data until it's joined into the final vector
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec3> tempNormals;
	char temp[15];
	char buff[BUFFSIZE];
	float tempx, tempy, tempz; // used to create vertex and vertex normals
	unsigned int x, y; // used to pair up vertex and vertex normals

	std::ifstream infile(path);
	std::stringstream ss;
	std::stringstream ss2;

	while(true)
	{
		if(infile.eof()) // exits loop when it reaches the end of the file
			break;
		infile.getline( buff, BUFFSIZE);
		ss.clear();
		ss.str(""); // clears and sets the string stream buffer to empty
		ss << buff;
		if(buff[0]=='v') // if vector
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
		else if(buff[0]=='f') // if faces
		{
			ss >> temp;
			for (unsigned int i = 0; i < 15; i++)
			{
				if (temp[i] == '/')
					temp[i] = ' ';
			}
			ss2.clear();
			ss2.str(""); // clears and sets the string stream buffer to empty
			ss2 << temp;
			bool flipflop = true; // faces come in x//y, so flipflop alternates which is loaded
			while (ss2 >> temp) // example input: f 1//1 2//1 3//1
			{
				if (flipflop)
				{
					x = atoi(temp); // must be converted to c string to use atoi
					flipflop = !flipflop;
				}
				else
				{
					y = atoi(temp);
					flipflop = !flipflop;
					vertices.push_back(aitVertex(tempVertices[x-1], tempNormals[y-1])); // join the vertex and normal, then send to vertices
				}
			}
		}
		else if (buff[0] == 'o' || buff[0] == '#') // if comment or object name, ignore
		{
		} // example input: o Cube
		else if (buff[0] == 's') // TODO
		{
		} // example input: s off
	}
	return true;
}