#include "KinematicEngine.h"

KinematicEngine::KinematicEngine(std::vector<Element*>* elements, int maxThreadCount, std::shared_mutex* lock, std::condition_variable_any* cv) : el_ptr{ elements }, maxThreads{ maxThreadCount } {
	// Start engine main thread
	std::thread kinematicThread([this, lock, cv] { run(lock, cv); });
	kinematicThread.detach(); 
}

void KinematicEngine::run(std::shared_mutex* m, std::condition_variable_any* cv) {
	while (true) {
		// Engine Toggle Check
		if (!toggle) {
			std::shared_lock<std::shared_mutex> lk(*m);
			cv->wait(lk, [&] { toggle = !toggle;  return toggle; });
		}
		// Create element update queue for threaded workers
		queue = *el_ptr;
		// Generate threads
		std::vector<std::thread> threads;
		for (int i = 0; i < maxThreads; i++)
			threads.push_back(std::thread([this, m, cv] { ComputeWorker(m, cv); }));
		for (auto& th : threads) th.join();
	}
}

void KinematicEngine::ComputeWorker(std::shared_mutex* m, std::condition_variable_any* cv) {
	while (true) {
		// Engine Toggle Check
		if (!toggle) {
			std::shared_lock<std::shared_mutex> lk(*m);
			cv->wait(lk, [&] { toggle = !toggle;  return toggle; });
		}
		while (!queue.empty()) {
			if (q_busy)
				continue;
			q_busy = true;

			if (queue.empty()) {
				q_busy = false;
				continue;
			}

			Element* el = queue.back();
			queue.pop_back();
			q_busy = false;

			Eigen::Vector3d p_calc;
			/*
			for (int i = 0; i < el->perturbers.size(); i++) {
				p_calc = getVectorBetweenObject(el, (*el_ptr)[el->perturbers[i]->getID()]);
				//a_calc += (calculate_mu(obj->getMass(), system->starList[i]->obj->getMass()) / pow(p.norm(), 3)) * p;
			}*/

			for (int i = 0; i < el_ptr->size(); i++) {
				p_calc = getVectorBetweenObject(el, (*el_ptr)[i]);
				//a_calc += (calculate_mu(obj->getMass(), system->starList[i]->obj->getMass()) / pow(p.norm(), 3)) * p;
			}
		}
	}
		return;
}

void KinematicEngine::ComputePerturbations() {

}
