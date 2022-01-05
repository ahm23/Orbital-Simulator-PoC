#include "Object.h"

long Object::update_freq = 1000;

Object::Object(ObjectConfig config_obj) {
	this->config_obj = config_obj;
	kinematics.p << 0, 0, 0;
	kinematics.v << 0, 0, 0;
	kinematics.a << 0, 0, 0;
}

Object::~Object() {
	std::cout << "Destructing Object: " << config_obj.name << std::endl;
}

void Object::initKinematicProcess(Vector3d position, Vector3d velocity) {
	if (toggle_kinematic) {
		std::cout << "WARN: Attempt to Initialize Kinematic Process Failed for Object: " << config_obj.name << std::endl;
		std::cout << "Reason | Kinematic Process Already Initialized" << std::endl;
	}
	else {
		kinematics.p = position;
		kinematics.v = velocity;
		std::thread kinematicThread([this] { this->kinematicProcess(); });
		kinematicThread.detach();
		toggle_kinematic = true;
		kinematic_cv.notify_one();
	}
}

void Object::kinematicProcess() {
	std::unique_lock<std::mutex> lk(kinematic_m);
	kinematic_cv.wait(lk, [&] {return toggle_kinematic; });
	while (true) {
		time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		kinematics.a = (-1.326 * pow(10, 11) / pow(kinematics.p.norm(), 3)) * kinematics.p;
		kinematics.v = kinematics.v + kinematics.a * (double)(time + update_freq) / (double)1000 - kinematics.a * (double)time / (double)1000;
		kinematics.p = kinematics.p + kinematics.v * (double)(time + update_freq) / (double)1000 - kinematics.v * (double)time / (double)1000;
		std::cout << std::setprecision(12) << std::setw(10) << config_obj.name << std::setw(20) << kinematics.p.norm() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(update_freq));
	}
}