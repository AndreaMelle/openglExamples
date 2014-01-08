#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>

GLint getFileLength(std::ifstream& file)
{
	if (!file.good()) {
		return 0;
	}
	
	unsigned long pos = file.tellg();
	file.seekg(0, std::ios::end);
	unsigned long len = file.tellg();
	file.seekg(std::ios::beg);
	
	return (GLint)len;
}

GLint loadShaderSource(const char* filename, GLchar*& shaderSource)
{
	std::ifstream file;
	file.open(filename, std::ios::in); // opens as ASCII!
	if(!file) {
		return -1;
	}
	
	GLint len = getFileLength(file);
	
	if (len==0) return -2;   // Error: Empty File 
	
	shaderSource = (GLchar*) new char[len+1];
	if (shaderSource == 0) {
		return -3;   // can't reserve memory
	}
	
	// len isn't always strlen cause some characters are stripped in ascii read...
	// it is important to 0-terminate the real length later, len is just max possible value... 
	shaderSource[len] = 0; 
	
	unsigned int i=0;
	while (file.good())
	{
		shaderSource[i] = file.get();       // get character from file.
		if (!file.eof())
		i++;
	}
	
	shaderSource[i] = 0;  // 0-terminate it at the correct position
	
	file.close();
	  
	return len; // No Error
}

GLint unloadShaderSource(GLchar*& shaderSource)
{
	if (shaderSource != 0) {
		delete[] shaderSource;
	}
	shaderSource = 0;
	return 0;
}