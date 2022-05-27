#pragma once

#define MAX_BUFSIZE_BODY 1024
#define MAX_BUFSIZE 256 + MAX_BUFSIZE_BODY


enum REQ_TYPE { INIT, GET, MOD, RES, DEL };

struct PACKET {
	int id;
	REQ_TYPE type;
	int status;
	char body[MAX_BUFSIZE_BODY];
};

union P_INIT {
	struct INIT {
		double pos_x;
		double pos_y;
		double pos_z;
		double vel_x;
		double vel_y;
		double vel_z;
	} body;
	unsigned char buffer[sizeof(INIT)];
};