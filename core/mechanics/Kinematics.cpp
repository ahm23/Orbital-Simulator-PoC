#include "Kinematics.h"
#include <shared_mutex>


int Kinematics::initKinematicProcess(Eigen::Vector3d position, Eigen::Vector3d velocity) {
	if (initialized) {
		return 0;
	}
	else {
		p = position;
		v = velocity;
		std::thread kinematicThread([this] { this->kinematicProcess(); });
		kinematicThread.detach();
		initialized = true;
		//kinematic_cv.notify_one();
		return 1;
	}
}

void Kinematics::kinematicProcess() {
	std::shared_lock<shared_mutex> lk(system->kinematic_m);
	system->kinematic_cv.wait(lk, [&] {return system->toggle_kinematic; });
	while (true) {
		time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		//for ()
		a = (-1.326 * pow(10, 11) / pow(p.norm(), 3)) * p;
		v = v + a * (double)(time + system->update_freq) / (double)1000 - a * (double)time / (double)1000;
		p = p + v * (double)(time + system->update_freq) / (double)1000 - v * (double)time / (double)1000;
		//std::cout << std::setprecision(12) << std::setw(10) << config_obj.name << std::setw(20) << p.norm() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(system->update_freq));
	}
}

Kinematics::Kinematics(SolarSystem* sys) {
	system = sys;
	p << 0, 0, 0;
	v << 0, 0, 0;
	a << 0, 0, 0;
}
