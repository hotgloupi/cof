#ifndef  GIT_COMMON_HPP
# define GIT_COMMON_HPP

# include "fwd.hpp"

# include <git2/errors.h>

# include <stdexcept>
# include <string>

namespace git {

	inline
	void check_result(int const result)
	{
		if (result != GIT_OK)
			throw std::runtime_error("error code: " );//+ std::to_string(result));
	}

}

#endif
