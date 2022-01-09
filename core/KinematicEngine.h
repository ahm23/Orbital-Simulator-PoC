#pragma once
#include <vector>
#include "Element.h"

class KinematicEngine {
public:
	KinematicEngine(std::vector<Element*>*, int, std::mutex*, std::condition_variable*);
	void run(std::mutex*, std::condition_variable*);
	~KinematicEngine();




private:
	std::vector<Element*>* el_pointer;
	int maxThreads = 0;
	void ComputeProcess();

	// Engine toggle for pause/resume.
	bool toggle = false;

};

