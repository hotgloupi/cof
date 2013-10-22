#ifndef  GIT_CONFIG_HPP
# define GIT_CONFIG_HPP

# include "common.hpp"

# include <git2/config.h>

namespace git {

	class config
	{
	private:
		git_config* _config;

	private:
		friend class repository;
		inline explicit
		config(git_config* cfg) noexcept
			: _config{cfg}
		{}

	public:
		config(config const&) = delete;
		config& operator =(config const&) = delete;

		inline
		config(config&& other) noexcept
			: _config{other._config}
		{ other._config = nullptr; }

		inline
		config& operator =(config&& other) noexcept
		{
			_config = other._config;
			other._config = nullptr;
			return *this;
		}

		inline
		~config()
		{ git_config_free(_config); }
	};

}

#endif
