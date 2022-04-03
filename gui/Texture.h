#pragma once
#include <iostream>
using namespace std;

class Texture
{
public:
	Texture(const string &path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

private:
	unsigned int id;
	string filepath;
	unsigned char* localBuffer;
	int width, height, BPP;
};

