#ifndef  GIT_TIME_HPP
# define GIT_TIME_HPP

# include <git2/types.h>

namespace git {

	class time
	{
	private:
		git_time _time;

	private:
		time()
		{ /*XXX TODO*/ throw false;}

	public:
		inline explicit
		time(git_time const& t) noexcept
			: _time{t.time, t.offset}
		{}

		inline explicit
		time(git_time_t t, int offset)
			: _time{t, offset}
		{}


	public:
		inline
		git_time_t stamp() const noexcept
		{ return _time.time; }

		inline
		int offset() const noexcept
		{ return _time.offset; }

	public:
		static inline
		time now()
		{ return time(); }
	};

	template<typename Stream>
	Stream& operator <<(Stream& out, time const& t)
	{
		return out << "git::time{" << t.stamp() << ", " << t.offset() << "}";
	}

}

#endif
