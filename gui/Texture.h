#pragma once
#include <iostream>
#include <GL/glew.h>

class Texture
{
public:
	Texture() = default;

	Texture(const std::string &path);
	~Texture();

	void Bind(unsigned int slot = 0);
	void Unbind();

private:
	unsigned int id;
	std::string filepath;
	unsigned char* localBuffer;
	int width, height, BPP;
};

