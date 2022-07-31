#pragma once
#define MAX_BUFSIZE_BODY 256
#include <Eigen/Dense>

enum REQ_TYPE { INIT, GET, MOD, RES, DEL };

struct PACKET {
	int id;
	REQ_TYPE type;
	int status;
	char body[MAX_BUFSIZE_BODY];
};

union P_PACKET {
	PACKET p;
	unsigned char buffer[sizeof(PACKET)];
};

struct B_INIT {
	bool astronomical;
	double mass;
	uint64_t time;
	Eigen::Vector3d pos;
	Eigen::Vector3d vel;
};

union P_INIT {
	B_INIT body;
	unsigned char buffer[sizeof(B_INIT)];
};

struct B_MOD {
	double vel[3];
};

union P_MOD {
	B_MOD body;
	unsigned char buffer[sizeof(MOD)];
};

struct B_GET {
	bool r_vel = true;
	bool r_pos = true;
	bool r_acc = false;
};

union P_GET {
	B_GET body;
	unsigned char buffer[sizeof(B_GET)];
};

struct B_RES {
	uint64_t ts;
	Eigen::Vector3d pos;
	Eigen::Vector3d vel;
};

union P_RES {
	B_RES body;
	unsigned char buffer[sizeof(B_RES)];
};