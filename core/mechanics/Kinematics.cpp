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
	if (obj->getType() == ObjectTypes::STAR)
		return;
	std::shared_lock<shared_mutex> lk(system->kinematic_m);
	system->kinematic_cv.wait(lk, [&] {return system->toggle_kinematic; });
	Eigen::Vector3d a_calc;
	Eigen::Vector3d p_calc;
	while (true) {
		time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		a_calc << 0, 0, 0;
		//p_calc << 0, 0, 0;
		for (int i = 0; i < system->planetList.size(); i++) {
			if (system->planetList[i]->obj == obj) continue;
			p_calc = system->getVectorBetweenObjects(system->planetList[obj->getID()], system->planetList[i]);
			//a_calc += (calculate_mu(obj->getMass(), system->starList[i]->obj->getMass()) / pow(p.norm(), 3)) * p;
		}
		a = (-1.326 * pow(10, 11) / pow(p.norm(), 3)) * p;
		v = v + a * (double)(time + system->update_freq) / (double)1000 - a * (double)time / (double)1000;
		p = p + v * (double)(time + system->update_freq) / (double)1000 - v * (double)time / (double)1000;
		//std::cout << std::setprecision(12) << std::setw(10) << obj->getName() << std::setw(20) << p.norm() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(system->update_freq));
	}
}

Kinematics::Kinematics(SolarSystem* sys, Object* object) {
	system = sys;
	p << 0, 0, 0;
	v << 0, 0, 0;
	a << 0, 0, 0;
	obj = object;
}
