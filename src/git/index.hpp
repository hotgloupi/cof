#ifndef  GIT_INDEX_HPP
# define GIT_INDEX_HPP

# include "common.hpp"
# include "path.hpp"
# include "oid.hpp"
# include "detail/iterator.hpp"

#include <git2/index.h>

namespace git {

	class index
	{
	private:
		git_index* _index;

	private:
		friend repository;
		inline explicit
		index(git_index* idx) noexcept;

	public:
		inline
		~index();

		inline
		git::path path() const;

		inline
		void read();

		inline
		void write();

		inline
		void clear() noexcept;

	public:
		class weak_entry;

	public:
		/// Entry count.
		inline
		size_t count() const noexcept;

		/// Entry by index.
		inline
		weak_entry operator [](size_t const idx) const;

		/// Entry by path.
		inline
		weak_entry operator [](class path const& path) const;

		typedef detail::iterator<index, weak_entry> iterator;

		inline
		iterator begin() const;
		inline
		iterator end() const;

	};

}

# include "index.inl"

#endif
