#include <Multithreading/JobScheduler.h>
#include <Multithreading/ThreadGroup.h>
#include <Container/WorkStealingQueue.h>

#include <Utilities/Random.h>

#include <cstring>

BEGINNAMESPACE

thread_local size_type ThreadID = size_type(-1); //invalid at start

//
/// pre configured parameter 
static const size_type JOB_QUEUE_SIZE = 4096;
static const size_type JOB_SCHEDULER_THREADS = JobScheduler::NumWorker;
static const size_type JOB_MASK = JOB_QUEUE_SIZE - 1;
///
//

//
///Thread local and global storage
struct ThreadLocalJobQueueStorage {
	Job storage[JOB_QUEUE_SIZE];
	size_type idx;
	size_type numJobs;
};

typedef WorkStealingQueue<Job*, JOB_QUEUE_SIZE> JobQueue_t;

struct JobQueueGlobal {
	ThreadGroup threads;
	JobQueue_t queue[JOB_SCHEDULER_THREADS + 1];
	std::atomic_bool m_running;
};

thread_local ThreadLocalJobQueueStorage gtl_JobQueue; //Thread local parameter
JobQueueGlobal g_JobQueue; //Global parameter

JobQueue_t* GetWorkerThreadQueue() {
	return g_JobQueue.queue + gtl_JobQueue.idx;
}

JobQueue_t* GetWorkerThreadQueue(size_type idx) {
	return g_JobQueue.queue + idx;
}

///
//


//
/// 
JobScheduler::JobScheduler() {}

Job * JobScheduler::CreateEmptyJob() {
	return CreateJob(&JobScheduler::Noop_Job);
}

Job * JobScheduler::CreateJob(JobFunction function)
{
	Job* job = AllocateJob();
	job->function = function;
	job->parent = nullptr;
	job->unfinishedJobs = 1;
	return job;
}

Job * JobScheduler::CreateChildJob(Job * parent, JobFunction function) {
	parent->unfinishedJobs++;

	Job* job = AllocateJob();
	job->function = function;
	job->parent = parent;
	job->unfinishedJobs = 1;

	return job;
}

void JobScheduler::Noop_Job(Job * job, const void * data){
	NO_OP();
}

void JobScheduler::Initialize() {
	//The initializing thread will also be able to work within the JobScheduler
	ThreadID = 0;
	gtl_JobQueue.idx = 0; 
	gtl_JobQueue.numJobs = 0;
	g_JobQueue.m_running = true;
	for (size_type i = 0; i < JOB_SCHEDULER_THREADS; ++i) {
		g_JobQueue.threads.attach(std::bind(&JobScheduler::MainWorker, i + 1));
		gtl_JobQueue.numJobs = 0;
	}
}

void JobScheduler::Shutdown() {
	g_JobQueue.m_running = false; 
	g_JobQueue.threads.joinAll();
}

void JobScheduler::MainWorker(size_type idx) {
	ThreadID = idx;
	gtl_JobQueue.idx = idx;
	while(g_JobQueue.m_running) { //insert valid condition
		Job* job = GetJob(); //get or steal a job
		if (job) {
			Execute(job);
		}
	}
}

void JobScheduler::Execute(Job * job) {
	(job->function)(job, job->data);
	Finish(job);
}

void Yield() {
	std::this_thread::yield();
}

void Sleep() {
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void JobScheduler::Wait(const Job * job) {
	while (!HasJobCompleted(job)) {
		Job* nextJob = GetJob();
		if (nextJob) Execute(nextJob);
	}
}

bool JobScheduler::HasJobCompleted(const Job * job) {
	return job->unfinishedJobs == 0;
}

void JobScheduler::Finish(Job * job) {
	const int32 unfinished_jobs = --job->unfinishedJobs;
	if (unfinished_jobs == 0 && job->parent) {
		Finish(job->parent);
	}
}

Job * JobScheduler::GetJob() {
	typedef Random<size_type> RandomGenerator;
	static RandomGenerator GenerateRandomNumber(RandomGenerator::Parameter(0, JOB_SCHEDULER_THREADS));

	JobQueue_t * queue = GetWorkerThreadQueue();
	Job* job = queue->pop();
	if (job == nullptr) {
		size_type randomIndex = GenerateRandomNumber();
		JobQueue_t* stealQueue = GetWorkerThreadQueue(randomIndex);
		if (queue == stealQueue) {
			Yield();
			return nullptr;
		}

		Job* stolenJob = stealQueue->steal();
		if (stolenJob == nullptr) {
			//Yield(); //We could use Yield() here too, but Sleep() will prevent the CPU to run on 100% if there are no jobs in the queue
			Sleep();
			return nullptr;
		}
		return stolenJob;
	}
	return job;
}

void JobScheduler::PushJob(Job * job)
{
	g_JobQueue.queue[gtl_JobQueue.idx].push(job);
}

Job * JobScheduler::AllocateJob() {
	const size_type index = gtl_JobQueue.numJobs++;
	Job* job = gtl_JobQueue.storage + (index & JOB_MASK);

	std::memset(job->data, 0, Job::ExtraBytes);
	return job;
}

ENDNAMESPACE
