#pragma once
#include "engine.h"

ENGINE::ENGINE(int maxThreads, std::mutex* m, std::condition_variable_any* cvar, int* status) {
	maxTh = maxThreads;
	cv = cvar;
	threads.push_back(std::thread([this, m, status] { ComputeWorker(1, m, status, true); }));
	for (int i = 1; i < maxTh; i++)
		threads.push_back(std::thread([this, i, m, status] { ComputeWorker(i + 2, m, status, false); }));
	for (auto& th : threads) th.detach();
}

ENGINE::~ENGINE() {
	sig_terminate = true;
	threads.clear();
}

void ENGINE::ComputeWorker(int th_id, std::mutex* m, int* status, bool lead) {
	Eigen::Vector3d temp_p;
	Eigen::Vector3d calc_p;
	Eigen::Vector3d calc_v;
	Eigen::Vector3d calc_a;
	uint64_t time;
	uint64_t time_diff;
	object el;
	std::vector<object> sys_new;
	bool ac = false;
	while (true) {
		time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		// Engine Toggle Check
		if (!toggle) {
			if (lead) *status = 2;
			std::unique_lock<std::mutex> lk(*m);
			cv->wait(lk, [&] { if (!toggle) toggle = true;  return toggle; });
			sys_new = sys_objects;
		}
		if (queue.empty()) {
			if (lead && working == 0) {
				// Update "celestial system" objects.
				if (sys_new.size()) {
					sys_objects = sys_new;
					sys_new.clear();
				}
				// Check for actions check & graphics update.
				if (paused) {
					std::unique_lock<std::mutex> lk(*m);
					cv->wait(lk, [&] { if (paused) paused = false;  return paused; });
				}
				// Disable catchup processing & transition to real-time.
				if (sys_objects.size() && time < sys_objects[0].time) {
					if (catchup) catchup = false;
					std::this_thread::sleep_for(std::chrono::milliseconds(sys_objects[0].time - time));
					continue;
				}
				working = 0;
				// Fill queue with objects
				m->lock();
				queue.insert(queue.end(), sys_objects.begin(), sys_objects.end());
				queue.insert(queue.end(), objects.begin(), objects.end());
				m->unlock();
			}
			else {
				if (!catchup)
					std::this_thread::sleep_for(std::chrono::milliseconds(time_increment / 2));
				// might add second condition variable for lead thread to dispatch secondary compute threads, better than sleep.
				continue;
			}
		}
		while (!queue.empty()) {
			if (!ac) {
				working++;
				ac = true;
			}
			/*if (q_busy) continue;
			q_busy = th_id;

			if (q_busy != th_id) continue;
			//std::shared_lock<std::shared_mutex> lock(*m);*/
			//sys_compare = sys_objects;
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
			// v2.1 - Efficiency upgrade.
			calc_a << 0, 0, 0;
			for (int i = 0; i < sys_objects.size(); i++) {
				if (i > sys_objects.size() - 1)
					std::cout << "wack";
				if (el.id == sys_objects[i].id) continue;
				temp_p = el.p - sys_objects[i].p;
				calc_a += (calculate_mu(el.mass, sys_objects[i].mass) / pow(temp_p.norm(), 3)) * temp_p;
			}
			el.v += calc_a * (double)(el.time + time_increment) / 1000 - calc_a * (double)el.time / 1000;
			el.p += el.v * (double)(el.time + time_increment) / 1000 - el.v * (double)el.time / 1000;
			el.time += time_increment;

			//object_sm e = { el.id, el.time, el.mass, el.p, el.v };

			m->lock();
			if (el.astronomical)
				sys_new.push_back(el);
			else
				objects.push_back(el);
			m->unlock();
			time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - time;
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
		//if (!catchup)
	}
}

int ENGINE::addObject(int id, B_INIT o) {
	softPause();
	while (working);
	if (o.astronomical) {
		sys_objects.push_back({ id, o.astronomical, o.mass, o.time, o.pos, o.vel });
		sys_object_ids.insert({ id, sys_objects.size()-1 });
	}
	else {
		objects.push_back({ id, o.astronomical, o.mass, o.time, o.pos, o.vel });
		object_ids.insert({ id, objects.size()-1 });
	}
	cv->notify_all();
	return 0;
}

object ENGINE::getObject(int id) {
	return sys_object_ids[id] < sys_objects.size() && sys_objects[sys_object_ids[id]].id == id ? sys_objects[sys_object_ids[id]] : objects[object_ids[id]];
}

Eigen::Vector3d ENGINE::getPosition(int id) {
	object* obj = sys_object_ids[id] < sys_objects.size() && sys_objects[sys_object_ids[id]].id == id ? &sys_objects[sys_object_ids[id]] : &objects[object_ids[id]];
	return (*obj).p;
}

Eigen::Vector3d ENGINE::getVelocity(int id) {
	object* obj = sys_object_ids[id] < sys_objects.size() && sys_objects[sys_object_ids[id]].id == id ? &sys_objects[sys_object_ids[id]] : &objects[object_ids[id]];
	return (*obj).v;
}

void ENGINE::addToBuffer(std::mutex* m, object_sm* e, bool sys) {

}

int ENGINE::softPause() {
	paused++;
	return paused;
}