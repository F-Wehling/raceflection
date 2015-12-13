#pragma once

BEGINNAMESPACE

typedef void(*JobFunction)(struct Job*, const void*);

extern thread_local size_type ThreadID;

struct Job {
	static const size_type ExtraBytes = (2 * CACHE_LINE_SIZE) - (sizeof(JobFunction) + sizeof(Job*) + sizeof(std::atomic<int32>));
	JobFunction function;
	Job* parent;
	std::atomic<int32> unfinishedJobs;
	
	Byte data[ExtraBytes];
};


class JobScheduler {
	JobScheduler();
public:
	static const size_type NumWorker = 8;
public:
	static void Initialize();
	static void Shutdown();
	//Empty Jobs only execute a noop operation.
	//To avoid a check wheter the function is nullptr or not we assign an empty job to nothing-executing jobs (parent-only e.g.)
	static Job* CreateEmptyJob();
	static Job* CreateJob(JobFunction function);
	static Job* CreateChildJob(Job* parent, JobFunction function);
	
	template<typename JobData>
	static Job *CreateJob(JobFunction function, JobData data) {
		ASSERT(sizeof(JobData) <= Job::ExtraBytes, "Each job can store at most %d bytes per job", Job::ExtraBytes);
		Job* job = AllocateJob();
		job->function = function;
		job->parent = nullptr;
		job->unfinishedJobs = 1;

		std::memcpy(&job->data, &data, sizeof(JobData));

		return job;
	}

	template<typename JobData>
	static Job* CreateChildJob(Job* parent, JobFunction function, JobData data) {
		ASSERT(sizeof(JobData) <= Job::ExtraBytes, "Each job can store at most %d bytes per job", Job::ExtraBytes);

		parent->unfinishedJobs++;
		
		Job* job = AllocateJob();
		job->function = function;
		job->parent = parent;
		job->unfinishedJobs = 1;

		std::memcpy(&job->data, &data, sizeof(JobData));

		return job;
	}

	static inline void Run(Job* job) { 
		PushJob(job);
	}

	static void Wait(const Job* job);
	static bool HasJobCompleted(const Job* job);
protected:
	static void Noop_Job(Job* job, const void* data);
	static void MainWorker(size_type idx);
	static void Execute(Job* job);
	static void Finish(Job* job);
	static Job* GetJob();
	static void PushJob(Job* job);
	static Job* AllocateJob();
};

ENDNAMESPACE