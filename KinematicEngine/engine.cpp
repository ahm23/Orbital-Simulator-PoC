#pragma once
#include "engine.h"
#include "integrator.h"

ENGINE::ENGINE(int maxThreads, std::shared_mutex* m, std::condition_variable_any* cv, std::atomic<int>* status) {
	maxTh = maxThreads;
	threads.push_back(std::thread([this, m, cv, status] { ComputeWorker(1, m, cv, status, true); }));
	for (int i = 2; i <= maxTh; i++)
		threads.push_back(std::thread([this, i, m, cv, status] { ComputeWorker(i, m, cv, status, false); }));
	for (auto& th : threads) th.detach();
}

ENGINE::~ENGINE() {
	sig_terminate = true;
	threads.clear();
}

void ENGINE::ComputeWorker(int th_id, std::shared_mutex* m, std::condition_variable_any* cv, std::atomic<int>* status, bool lead) {
	Eigen::Vector3d temp_p;
	Eigen::Vector3d calc_p;
	Eigen::Vector3d calc_v;
	Eigen::Vector3d calc_a;
	uint64_t time;
	uint64_t time_diff;
	typeMap entry;
	object el;
	std::unordered_map<int, object> sys_new;
	bool ac = false;
	while (true) {
		time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		// Engine Toggle Check
		if (!toggle) {
			*status += 1;
			std::unique_lock<std::shared_mutex> lk(*m);
			cv->wait(lk, [&] { return toggle == true; });
			sys_new = sys_objects;
		}
		if (queue.empty()) {
			if (lead && working == 0) {
				// Update "celestial system" objects.
				if (sys_new.size() == sys_objects.size() && sys_objects.size() > 1) {
					sys_objects = sys_new;
				}
				sys_new.clear();
				// Check for actions check & graphics update.
				if (paused) {
					while (paused);
					//if (lead) working = 0;
					//std::unique_lock<std::shared_mutex> lk2(mut);
					//cv2.wait(lk2, [&] { return paused == false; });
					//std::cout << "nice!";
				}
				// Disable catchup processing & transition to real-time.
				if (sys_objects.size() > 1) {
					if (time < sys_objects[0].time) {
						if (catchup) catchup = false;
							std::this_thread::sleep_for(std::chrono::milliseconds(sys_objects[0].time - time));
							continue;
					}
					else {
						m->lock();
						queue.insert(queue.end(), obj_ids.begin(), obj_ids.end());
						m->unlock();
					}
				}
				//working = 0;
				// Fill queue with objects
			}
			else {
				//if (!catchup)
					//std::this_thread::sleep_for(std::chrono::milliseconds(time_increment / 2));
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
				entry = queue.back();
				el = (entry.type == ObjectType::SYSTEM) ? sys_objects[entry.id] : objects[entry.id];
				queue.pop_back();
			}
			m->unlock();
			// v2.1 - Efficiency upgrade.
			calc_a << 0, 0, 0;
			for (int i = 0; i < sys_objects.size(); i++) {
				if (el.id == sys_objects[i].id) continue;
				temp_p = el.p - sys_objects[i].p;
				//calc_a += (calculate_mu(el.mass, sys_objects[i].mass) / pow(temp_p.norm(), 3)) * temp_p;
				el.v += solve(temp_p, time_increment, el.time, &func_orbitAccel, std::vector<void*>{ &el, &sys_objects[i] });
				std::cout << "bruh";
			}
			std::cout << el.id << " Earth Velocity" << el.v << "@ T:" << el.time+time_increment << "\n";
			el.p = solve(temp_p, time_increment, el.time, &func_orbitVel, std::vector<void*>{});
			//el.v += calc_a * (double)(el.time + time_increment) / 1000 - calc_a * (double)el.time / 1000;
			//el.p += el.v * (double)(el.time + time_increment) / 1000 - el.v * (double)el.time / 1000;
			el.time += time_increment;

			//object_sm e = { el.id, el.time, el.mass, el.p, el.v };

			m->lock();
			if (el.astronomical) {
				sys_new.insert({ el.id, el });
			}
			else {
				objects.insert({ el.id, el });
			}
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
	std::cout << "what";
}

int ENGINE::addObject(int id, B_INIT o) {
	softPause();
	while (working);
	if (o.astronomical) {
		//sys_objects.push_back({ id, o.astronomical, o.mass, o.time, o.pos, o.vel });
		sys_objects.insert({ id, { id, o.astronomical, o.mass, o.time, o.pos, o.vel } });
		obj_ids.push_back({ id,ObjectType::SYSTEM });
	}
	else {
		//objects.push_back({ id, o.astronomical, o.mass, o.time, o.pos, o.vel });
		objects.insert({ id, { id, o.astronomical, o.mass, o.time, o.pos, o.vel } });
		obj_ids.push_back({ id,ObjectType::STANDARD });
	}
	unPause();
	return 0;
}

object ENGINE::getObject(int id) {
	return (sys_objects.count(id) > 0) ? sys_objects[id] : objects[id];
}
/*
Eigen::Vector3d ENGINE::getPosition(int id) {
	object* obj = sys_object_ids[id] < sys_objects.size() && sys_objects[sys_object_ids[id]].id == id ? &sys_objects[sys_object_ids[id]] : &objects[object_ids[id]];
	return (*obj).p;
}

Eigen::Vector3d ENGINE::getVelocity(int id) {
	object* obj = sys_object_ids[id] < sys_objects.size() && sys_objects[sys_object_ids[id]].id == id ? &sys_objects[sys_object_ids[id]] : &objects[object_ids[id]];
	return (*obj).v;
}
*/
void ENGINE::addToBuffer(std::mutex* m, object_sm* e, bool sys) {

}

int ENGINE::softPause() {
	paused++;
	return paused;
}

int ENGINE::unPause() {
	paused = 0;
	cv2.notify_all();
	return paused;
}