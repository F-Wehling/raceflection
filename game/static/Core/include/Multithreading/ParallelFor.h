#pragma once

#include <Multithreading/JobScheduler.h>

#include "Delegates/Delegate.h"

BEGINNAMESPACE

namespace detail {
	
	template <typename T, typename S>
	struct parallel_for_job_data
	{
		typedef T DataType;
		typedef S SplitterType;

		typedef void(*Function)(DataType*, uint32, void*);
		
		parallel_for_job_data(DataType* data, uint32 count, Function function, void* extraData, const SplitterType& splitter)
			: data(data)
			, count(count)
			, function(function)
			, splitter(splitter)
			, extraData(extraData)
		{
		}

		DataType* data;
		void* extraData;
		uint32 count;
		Function function;
		SplitterType splitter;
	};

	template <typename JobData>
	void parallel_for_job(Job* job, const void* jobData)
	{
		const JobData* data = static_cast<const JobData*>(jobData);
		const JobData::SplitterType& splitter = data->splitter;
		void* extraData = data->extraData;

		if (splitter.Split<JobData::DataType>(data->count))
		{
			// split in two
			const uint32 leftCount = data->count / 2u;
			const JobData leftData(data->data, leftCount, data->function, extraData, splitter);
			Job* left = JobScheduler::CreateChildJob(job, &detail::parallel_for_job<JobData>, leftData);
			JobScheduler::Run(left);

			const uint32 rightCount = data->count - leftCount;
			const JobData rightData(data->data + leftCount, rightCount, data->function, extraData, splitter);
			Job* right = JobScheduler::CreateChildJob(job, &detail::parallel_for_job<JobData>, rightData);
			JobScheduler::Run(right);
		}
		else
		{
			// execute the function on the range of data
			(data->function)(data->data, data->count, extraData);
		}
	}
}

class CountSplitter
{
public:
	explicit CountSplitter(uint32 count = 50u)
		: m_count(count)
	{
	}

	template <typename T>
	inline bool Split(uint32 count) const
	{
		return (count > m_count);
	}

private:
	uint32 m_count;
};

class DataSizeSplitter
{
public:
	explicit DataSizeSplitter(uint32 size)
		: m_size(size)
	{
	}

	template <typename T>
	inline bool Split(uint32 count) const
	{
		return (count*sizeof(T) > m_size);
	}

private:
	uint32 m_size;
};

template<typename T, typename S = CountSplitter>
Job* parallel_for(T* data, uint32 count, void(*function)(T*, uint32, void*), void* extraData = nullptr, const S& splitter = S()) {
	typedef detail::parallel_for_job_data<T, S> JobData;
	const JobData jobData(data, count, function, extraData, splitter);

	Job* job = JobScheduler::CreateJob(&detail::parallel_for_job<JobData>, jobData);

	JobScheduler::Run(job);

	return job;
}

ENDNAMESPACE