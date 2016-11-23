/*
 * Utils.c
 *
 *  Created on: 16/01/2014
 *      Author: Iv�n
 */

#include "Utils.h"
#include <stdio.h>
#include <stdlib.h>

const char* loadShader(const char* filename) {
	FILE* shaderFile = fopen(filename, "r");
	const int BUFFER_SIZE = 256;
	char buffer[BUFFER_SIZE];
//	Count number of characters in source file
	int charCount = 0;
	int i = 0;
	while(!feof(shaderFile)) {
		if(fgets(buffer, BUFFER_SIZE, shaderFile) == NULL) continue;
		i = 0;
		while(buffer[i ++] != '\0') charCount ++;
	}
	charCount ++; // consider end-of-string character

//	Copy characters to new char array
	rewind(shaderFile);
	char* shaderData = (char*) malloc(sizeof(char) * charCount);
	int charIndex = 0;
	while(!feof(shaderFile)) {
		if(fgets(buffer, BUFFER_SIZE, shaderFile) == NULL) continue;
		i = 0;
		while(buffer[i] != '\0') shaderData[charIndex ++] = buffer[i ++];
	}
	shaderData[charIndex] = '\0';

	fclose(shaderFile);
	return shaderData;
}

GLuint compileShader(const char* filename, GLuint shaderType) {
	char const* source = loadShader(filename);
	GLuint shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &source, NULL);
	glCompileShader(shaderId);
	return shaderId;
}

bool shaderCompiled(GLuint shaderId) {
	GLint params;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &params);
	if(params == 1) return true;

	GLint maxLength = 0;
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);
	GLchar* errors = (GLchar*) malloc(sizeof(int) * maxLength);
	glGetShaderInfoLog(shaderId, maxLength, &maxLength, errors);
	puts(errors);
	return false;
}

bool loadBMP(const char* filename, unsigned char **pdata, unsigned int *width, unsigned int *height) {
    unsigned char header[54];
    FILE * file = fopen(filename, "rb");
    fread(header, 1, 54, file);
    if(header[0] != 'B' || header[1] != 'M' ) {
        printf("Not a correct BMP file\n");
        return false;
    }
    unsigned char *bytePointerW      = &(header[0x12]);
    unsigned int  *fourBytesPointerW = (unsigned int*) bytePointerW;
    *width = *fourBytesPointerW;
    unsigned char *bytePointerH      = &(header[0x16]);
    unsigned int  *fourBytesPointerH = (unsigned int*) bytePointerH;
    *height = *fourBytesPointerH;
    *pdata = (unsigned char*) malloc(sizeof(unsigned char) * (*width) * (*height) * 3);
    fread(*pdata, 1, (*width) * (*height) * 3, file);
    fclose(file);
    return true;
}

void loadTexture(const char* filename, GLuint texId) {
	unsigned char *data = NULL;
	unsigned int width = 0, height = 0;
	if(!loadBMP(filename, &data, &width, &height)) exit(0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
	// glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
//	printf("id: %d, width: %d, height: %d, data: %d\n", texId, width, height, data[500]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	free(data);
}
