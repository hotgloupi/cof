#ifndef  GIT_PATH_HPP
# define GIT_PATH_HPP

# include <string>

namespace git {

	class path
	{
	private:
		std::string _path;

	public:
		inline
		path(std::string path)
			: _path{std::move(path)}
		{}

		inline
		path(char const* str)
			: _path{str}
		{}

		inline
		char const* c_str() const noexcept
		{ return _path.c_str(); }
	};

}

#endif
