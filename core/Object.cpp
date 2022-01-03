#include "Object.h"

long Object::update_freq = 100;

Object::Object(ObjectConfig config) {
	this->config = config;
	kinetics.p << 0, 0, 0;
	kinetics.v << 0, 0, 0;
	kinetics.a << 0, 0, 0;
}

Object::~Object() {
	std::cout << "Destructing Object: " << config.name << std::endl;
}

void Object::initKineticProcess(Vector3d position, Vector3d velocity) {
	if (toggle_kinetic) {
		std::cout << "WARN: Attempt to Initialize Kinetic Process Failed for Object: " << config.name << std::endl;
		std::cout << "Reason | Kinetic Process Already Initialized" << std::endl;
	}
	else {
		kinetics.p = position;
		kinetics.v = velocity;
		std::thread kineticThread([this] { this->kineticProcess(); });
		kineticThread.detach();
	}
}

void Object::kineticProcess() {
	std::unique_lock<std::mutex> lk(kinetic_m);
	kinetic_cv.wait(lk, [&] {return toggle_kinetic; });
	while (true) {
		time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		//system("cls");
		kinetics.a = (-config.mu / pow(kinetics.p.norm(), 3)) * kinetics.p;
		kinetics.v = kinetics.v + kinetics.a * (double)(time + update_freq + 1.0) / (double)1000 - kinetics.a * (double)time / (double)1000;
		kinetics.p = kinetics.p + kinetics.v * (double)(time + update_freq + 1.0) / (double)1000 - kinetics.v * (double)time / (double)1000;
		//std::cout << setprecision(12) << kinetics.p.norm() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}