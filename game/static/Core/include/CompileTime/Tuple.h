#pragma once

#include <tuple>

BEGINNAMESPACE

template<typename ...T>
using Tuple = std::tuple<T...>;

namespace detail {
	template<int... Is>
	struct seq { };

	template<int N, int... Is>
	struct gen_seq : gen_seq<N - 1, N - 1, Is...> { };

	template<int... Is>
	struct gen_seq<0, Is...> : seq<Is...> { };


	template<typename T, typename F, int... Is, typename ...Args>
	void for_each(T&& t, F f, seq<Is...>, Args... args) {
		auto l = { (f(std::get<Is>(t,args...)), 0)... };
	}

	template<typename T, typename F, int... Is, typename ...Args>
	void for_each_on(T&& t, F f, seq<Is...>, Args... args) {
		auto l = { ((std::get<Is>(t).*f)(args...), 0)... };
	}
}

template<typename ... Ts, typename F, typename ...Args>
void for_each_in_tuple(Tuple<Ts...>& t, F f, Args... args) {
	detail::for_each(t, f, detail::gen_seq<sizeof...(Ts)>(), args...);
}

template<typename ... Ts, typename F, typename ...Args>
void for_each_on_tuple(Tuple<Ts...>& t, F f, Args... args) {
	detail::for_each_on(t, f, detail::gen_seq<sizeof...(Ts)>(), args...);
}

ENDNAMESPACE