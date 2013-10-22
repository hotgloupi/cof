#ifndef  GIT_TREE_HPP
# define GIT_TREE_HPP

# include "common.hpp"
# include "object.hpp"
# include "detail/iterator.hpp"

# include <git2/tree.h>

namespace git {

	class tree
	{
	private:
		git_tree* _tree;

	private:
		friend class commit;
		inline explicit
		tree(git_tree* t) noexcept;

	public:
		inline /* implicit */
		tree(object&& obj);

		inline
		~tree();

	public:
		inline
		weak_oid id() const noexcept;

		inline
		size_t count() const noexcept;

	private:
		class weak_entry;
		typedef detail::iterator<tree, weak_entry> iterator;

	public:
		inline
		iterator begin() const noexcept;

		inline
		iterator end() const noexcept;

	public:
		inline
		weak_entry operator [](size_t const idx) const;

		inline
		weak_entry operator [](weak_oid const& id) const;
	};

}

# include "tree.inl"

#endif
