#pragma once
#include <vector>
#include "Element.h"
#include "utils/utils_planetary.h"

class KinematicEngine {
public:
	KinematicEngine(std::vector<Element*>*, int, std::mutex*, std::condition_variable*);
	void run(std::mutex*, std::condition_variable*);
	~KinematicEngine();




private:
	std::vector<Element*>* el_ptr;
	int maxThreads = 0;
	void ComputeWorker();

	bool q_busy = false;
	std::vector<Element*> queue;
	

	// Engine toggle for pause/resume.
	bool toggle = false;


	void ComputePerturbations();

};

