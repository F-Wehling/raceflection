#pragma once

#include <Multithreading/JobScheduler.h>

BEGINNAMESPACE

namespace detail {

	template<typename F, typename Iterator, size_type bucket_size = 50>
	class _ParallelFor {
		typedef struct {
			F f;
			Iterator begin;
			Iterator  end;
		} iterators;

		static const size_type iteratorSize = sizeof(iterators);

		static_assert(Job::ExtraBytes >= iteratorSize, "The ExtraBytes of a job-object aren't large enough");

		static void _for(Job* job, const void* data) {
			const iterators* it_s = reinterpret_cast<const iterators*>(data);
			Iterator it = it_s->begin;
			for (; it != it_s->end; ++it) {
				it_s->f(*it);
			}
		}
	public:
		_ParallelFor(Iterator begin, Iterator end, F f ) {
			size_type N = std::distance(begin, end);
			size_type numJobs = N / bucket_size;

			Job* parent_for = JobScheduler::CreateEmptyJob();

			Iterator it = begin;
			for (size_type j = 0; j < numJobs; ++j) {
				Job* job = JobScheduler::CreateChildJob(parent_for, &_ParallelFor::_for);
				iterators* it_s = reinterpret_cast<iterators*>(job->padding);
				it_s->f = f;
				it_s->begin = it;
				it += bucket_size;
				it_s->end = it;
				NO_OP();
			}
			if (it != end) {
				Job* job = JobScheduler::CreateChildJob(parent_for, _ParallelFor::_for);
				iterators* it_s = reinterpret_cast<iterators*>(job->padding);
				it_s->f = f;
				it_s->begin = it;
				it_s->end = end;
			}
			JobScheduler::Wait(parent_for); //The parent will be finished when all sub-jobs (all for-loops) be finished
		}
	};
}

template<size_type bucket_size = 50, typename F, typename Iterator>
void parallel_for(Iterator begin, Iterator end, F f) {
	detail::_ParallelFor<F, Iterator, bucket_size>(begin, end, f);
}

template<size_type bucket_size = 50, typename F, typename Container>
void parallel_for_each(Container& container, F f) {
	detail::_ParallelFor<F, typename Container::iterator, bucket_size>(container.begin(), container.end(), f);
}

template<size_type bucket_size = 50, typename F, typename Container>
void parallel_for_each(const Container& container, F f) {
	detail::_ParallelFor<F, typename Container::const_iterator, bucket_size>(container.begin(), container.end(), f);
}

ENDNAMESPACE