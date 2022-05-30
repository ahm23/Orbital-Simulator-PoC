#pragma once
#include "engine.h"

ENGINE::ENGINE(int maxThreads, std::mutex* m, std::condition_variable_any* cv, int* status) {
	maxTh = maxThreads;
	threads.push_back(std::thread([this, m, cv, status] { ComputeWorker(1, m, cv, status, true); }));
	for (int i = 1; i < maxTh; i++)
		threads.push_back(std::thread([this, i, m, cv, status] { ComputeWorker(i + 2, m, cv, status, false); }));
	for (auto& th : threads) th.detach();
}

ENGINE::~ENGINE() {
	sig_terminate = true;
	threads.clear();
}

void ENGINE::ComputeWorker(int th_id, std::mutex* m, std::condition_variable_any* cv, int* status, bool lead) {
	Eigen::Vector3d temp_p;
	Eigen::Vector3d calc_p;
	Eigen::Vector3d calc_v;
	Eigen::Vector3d calc_a;
	uint64_t time;
	object el;
	std::vector<object> sys_compare;
	bool ac = false;
	while (true) {
		time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		// Engine Toggle Check
		if (!toggle) {
			if (lead) *status = 2;
			std::unique_lock<std::mutex> lk(*m);
			cv->wait(lk, [&] { if (!toggle) toggle = true;  return toggle; });
		}
		if (queue.empty()) {
			if (lead && working == 0) {
				if (!objects.size()) continue;
				if (buffer_objects.size() > buffer_capacity) {
					buffer_objects.erase(buffer_objects.begin());
					buffer_sys_objects.erase(buffer_sys_objects.begin());
					if (time < buffer_objects[1][0].time)
						continue;
				}
				buffer_objects.push_back({});
				buffer_sys_objects.push_back({});
				working = 0;
				queue = objects;
			}
			continue;
		}
		while (!queue.empty()) {
			if (!ac) {
				working++;
				ac = true;
			}
			/*if (q_busy) continue;
			q_busy = th_id;

			if (q_busy != th_id) continue;
			//std::shared_lock<std::shared_mutex> lock(*m);
			//sys_compare = sys_objects;*/
			m->lock();
			if (queue.empty()) {
				m->unlock();
				continue;
			}
			else {
				el = queue.back();
				queue.pop_back();
				//q_busy = 0;
			}
			m->unlock();
			calc_a << 0, 0, 0;
			for (int i = 0; i < sys_objects.size(); i++) {
				if (el.id == sys_objects[i].id) continue;
				temp_p = el.p - sys_objects[i].p;
				calc_a += (calculate_mu(el.mass, sys_objects[i].mass) / pow(temp_p.norm(), 3)) * temp_p;
			}
			el.v = el.v + calc_a * (double)(el.time + time_increment) / (double)1000 - calc_a * (double)el.time / (double)1000;
			el.p = el.p + el.v * (double)(el.time + time_increment) / (double)1000 - el.v * (double)el.time / (double)1000;
			el.time = el.time + time_increment;

			object_sm e = { el.id, el.time, *el.p.data(), *el.v.data() };

			m->lock();
			(buffer_objects.back()).push_back(e);
			if (el.astronomical)
				(buffer_sys_objects.back()).push_back(e);
			m->unlock();

			if (catchup && time <= el.time - time_increment * buffer_capacity) catchup = false;
		}
		if (ac) {
			ac = false;
			working--;
		}
		/*if (lead && queue.empty())
			q_busy = -1;*/

		if (sig_terminate)
			return;
		// not necessary
		if (!catchup)
			std::this_thread::sleep_for(std::chrono::milliseconds(time_increment));
	}
}

int ENGINE::addObject(int id, B_INIT o) {
	objects.push_back({ id, o.astronomical, o.mass, o.time, o.pos, o.vel });
	if (o.astronomical)
		sys_objects.push_back({ id, o.astronomical, o.mass, o.time, o.pos, o.vel });
	return 0;
}

void ENGINE::addToBuffer(std::mutex* m, object_sm* e, bool sys) {

}
