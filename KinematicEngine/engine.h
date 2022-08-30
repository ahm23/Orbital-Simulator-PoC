#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <condition_variable>
#include <shared_mutex>
#include <Eigen/Dense>
#include <chrono>

#include "../commons/ipc.h"
#include "../commons/constants.h"

enum ObjectType {
	SYSTEM,
	STANDARD
};

struct object {
	int id;
	bool astronomical;
	double mass;
	uint64_t time;
	Eigen::Vector3d p;
	Eigen::Vector3d v;
};

struct typeMap {
	int id;
	ObjectType type;
};

struct object_sm {
	unsigned int id;
	double mass;
	uint64_t time;
	Eigen::Vector3d p;
	Eigen::Vector3d v;
};

class ENGINE {
public:
	ENGINE(int, std::shared_mutex*, std::condition_variable_any*, std::atomic<int>*);
	~ENGINE();

	int addObject(int, B_INIT);
	object getObject(int);
	Eigen::Vector3d getPosition(int);
	Eigen::Vector3d getVelocity(int);

	int softPause();
	int unPause();
	std::atomic<bool> toggle = false;

private:
	int maxThreads = 0;
	std::vector<std::thread> threads;
	std::condition_variable_any cv2;
	std::shared_mutex mut;
	
	const int time_increment = 100;

	void ComputeWorker(int, std::shared_mutex* m, std::condition_variable_any*, std::atomic<int>*, bool);
	
	void addToBuffer(std::mutex*, object_sm*, bool);

	std::atomic<int> working = 0;
	bool catchup = true;
	unsigned int buffer_capacity = 86400;
	std::atomic<int> paused = 0;


	std::unordered_map<int, object> sys_objects;
	std::unordered_map<int, object> objects;
	std::vector<typeMap> obj_ids;

	std::vector<typeMap> queue;

	int b_count = -1;


	// Engine toggle for pause/resume.
	bool sig_terminate = false;
	int maxTh;
};

static double calculate_mu(double mass, double ref_mass) {
	return GRAV_KM * (mass + ref_mass);
}

static Eigen::Vector3d func_orbitAccel(double t, Eigen::Vector3d r, std::vector<void*> param) {
	return -(calculate_mu(((object*)param[0])->mass, ((object*)param[1])->mass) / pow(r.norm(), 3)) * r;
}

static Eigen::Vector3d func_orbitVel(double t, Eigen::Vector3d v, std::vector<void*> param) {
	return v;
}