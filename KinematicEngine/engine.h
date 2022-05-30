#include <vector>
#include <condition_variable>
#include <shared_mutex>
#include <Eigen/Dense>

#include "../commons/ipc.h"

struct object {
	int id;
	bool astronomical;
	Eigen::Vector3d p;
	Eigen::Vector3d v;
};

class ENGINE {
public:
	ENGINE(int, std::shared_mutex*, std::condition_variable_any*);
	~ENGINE();

	int addObject(int, B_INIT);

private:
	int maxThreads = 0;
	std::vector<std::thread> threads;
	
	void ComputeWorker(std::shared_mutex* m, std::condition_variable_any* cv);
	

	bool q_busy = false;
	std::vector<object> sys_objects;
	std::vector<object> objects;
	std::vector<object> queue;


	// Engine toggle for pause/resume.
	bool toggle = false;
	bool sig_terminate = false;
};