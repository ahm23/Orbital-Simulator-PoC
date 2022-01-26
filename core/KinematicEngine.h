#pragma once
#include <vector>
#include <condition_variable>
#include "Element.h"
#include "utils/utils_planetary.h"
#include <shared_mutex>

class KinematicEngine {
public:
	KinematicEngine(std::vector<Element*>*, int, std::shared_mutex*, std::condition_variable_any*);
	void run(std::shared_mutex*, std::condition_variable_any*);
	~KinematicEngine();

private:
	std::vector<Element*>* el_ptr;
	int maxThreads = 0;
	void ComputeWorker(std::shared_mutex* m, std::condition_variable_any* cv);

	bool q_busy = false;
	std::vector<Element*> queue;
	

	// Engine toggle for pause/resume.
	bool toggle = false;


	void ComputePerturbations();

};

