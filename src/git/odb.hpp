#ifndef  GIT_ODB_HPP
# define GIT_ODB_HPP

# include "common.hpp"

# include <git2/odb.h>

namespace git {

	class odb
	{
	private:
		git_odb* _odb;

	private:
		friend class repository;
		inline explicit
		odb(git_odb* odb) noexcept;

	public:
		inline
		~odb();

	public:
		template<typename Visitor>
		void visit(Visitor&& cb);
	};

}

# include "odb.inl"

#endif
