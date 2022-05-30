#pragma once
#include "engine.h"

ENGINE::ENGINE(int maxThreads, std::shared_mutex* m, std::condition_variable_any* cv) {
	for (int i = 0; i < maxThreads; i++)
		threads.push_back(std::thread([this, m, cv] { ComputeWorker(m, cv); }));
	for (auto& th : threads) th.detach();
}

ENGINE::~ENGINE() {
	sig_terminate = true;
	threads.clear();
}

void ENGINE::ComputeWorker(std::shared_mutex* m, std::condition_variable_any* cv) {
	while (true) {
		// Engine Toggle Check
		if (!toggle) {
			std::shared_lock<std::shared_mutex> lk(*m);
			cv->wait(lk, [&] { if (!toggle) toggle = !toggle;  return toggle; });
		}
		if (queue.empty()) queue = objects;

		while (!queue.empty()) {
			if (q_busy) continue;
			q_busy = true;

			if (queue.empty()) {
				q_busy = false;
				continue;
			}

			object el = queue.back();
			queue.pop_back();
			q_busy = false;

			Eigen::Vector3d calc_a;

			for (int i = 0; i < sys_objects.size(); i++) {
				if (el.id == sys_objects[i].id) continue;
				//p_calc = getVectorBetweenObject(el, (*el_ptr)[i]);
				/*if ((*el_ptr)[i]->obj->getName() == "Moon" && el->obj->getName() == "Earth") {
					system("cls");
					std::cout << p_calc << std::endl;
				}*/
				//a_calc += (calculate_mu(obj->getMass(), system->starList[i]->obj->getMass()) / pow(p.norm(), 3)) * p;
			}
		}
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));

		if (sig_terminate)
			return;
	}
}

int ENGINE::addObject(int id, B_INIT o) {
	objects.push_back({ id, o.astronomical, o.pos, o.vel });
	if (o.astronomical)
		sys_objects.push_back({ id, o.astronomical, o.pos, o.vel });
	return 0;
}
