#pragma once

BEGINNAMESPACE

typedef void(*JobFunction)(struct Job*, const void*);

extern thread_local size_type ThreadID;

struct Job {
	static const size_type ExtraBytes = (2 * CACHE_LINE_SIZE) - (sizeof(JobFunction) + sizeof(Job*) + sizeof(std::atomic<int32>));
	JobFunction function;
	Job* parent;
	std::atomic<int32> unfinishedJobs;
	union {
		Byte padding[ExtraBytes];
		const void* padding_asPtr;
		size_type padding_asSizeType[ExtraBytes / sizeof(size_type)];
		size_type padding_asUint32[ExtraBytes / sizeof(uint32)];
	};
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
	static Job* CreateJob(JobFunction function, Byte extra[Job::ExtraBytes]);
	static Job* CreateJob(JobFunction function, const void* ptr);
	static Job* CreateJob(JobFunction function, void* ptr, size_type size);

	static Job* CreateChildJob(Job* parent, JobFunction function);
	static Job* CreateChildJob(Job* parent, JobFunction function, Byte extra[Job::ExtraBytes]);

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