#include "KinematicEngine.h"

KinematicEngine::KinematicEngine(std::vector<Element*>* elements, int maxThreadCount, std::mutex* lock, std::condition_variable* cv) : el_pointer{ elements }, maxThreads{ maxThreadCount } {
	// Start engine main thread
	std::thread kinematicThread([this, lock, cv] { run(lock, cv); });
	kinematicThread.detach(); 
}

void KinematicEngine::run(std::mutex* m, std::condition_variable* cv) {
	while (true) {
		// Engine Toggle Check
		if (!toggle) {
			std::unique_lock<mutex> lk(*m);
			cv->wait(lk, [&] {return toggle; });
		}
		// Create element update queue for threaded workers
		std::vector<Element*> queue = *el_pointer;
		// Generate threads
		std::vector<std::thread> threads;
		for (int i = 0; i < maxThreads; i++)
			threads.push_back(std::thread([this] { ComputeProcess(); }));
		for (auto& th : threads) th.join();
	}
}

void KinematicEngine::ComputeProcess() {
	
}


