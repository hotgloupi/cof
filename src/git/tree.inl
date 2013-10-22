#ifndef  GIT_TREE_INL
# define GIT_TREE_INL

# include "tree.hpp"

namespace git {

	tree::tree(git_tree* t) noexcept
		: _tree{t}
	{}

	tree::tree(object&& obj)
		: _tree{reinterpret_cast<git_tree*>(obj._object)}
	{
		if (obj.type() != object_type::tree)
			throw std::runtime_error{"Wrong object type"};
		obj._object = nullptr;
	}

	tree::~tree() { git_tree_free(_tree); }

	weak_oid tree::id() const noexcept
	{ return weak_oid{git_tree_id(_tree)}; }

	size_t tree::count() const noexcept
	{ return git_tree_entrycount(_tree); }

	class tree::weak_entry
	{
	private:
		git_tree_entry const* _entry;

	public:
		inline explicit
		weak_entry(git_tree_entry const* entry) noexcept
			: _entry{entry}
		{}

		inline
		object_type type() const noexcept
		{ return detail::from(git_tree_entry_type(_entry)); }

		inline
		filemode_type filemode() const noexcept
		{ return detail::from(git_tree_entry_filemode(_entry)); }
	};

	tree::iterator tree::begin() const noexcept
	{ return iterator{*this, 0}; }

	tree::iterator tree::end() const noexcept
	{ return iterator{*this, this->count()}; }

	tree::weak_entry tree::operator [](size_t const idx) const
	{
		auto ptr = git_tree_entry_byindex(_tree, idx);
		if (ptr == nullptr)
			throw std::range_error{"out of range"};
		return weak_entry{ptr};
	}

	tree::weak_entry tree::operator [](weak_oid const& id) const
	{
		auto ptr = git_tree_entry_byoid(_tree, id.native());
		if (ptr == nullptr)
			throw std::range_error{"id not found"};
		return weak_entry{ptr};
	}

	template<typename Stream>
	Stream& operator <<(Stream& out, tree const& t)
	{
		return out << "git::tree{"
		           << t.id() << ", "
		           << t.count() << (t.count() > 1 ? " entries" : " entry")
		           << "}";
	}

	template<typename Stream>
	Stream& operator <<(Stream& out, tree::weak_entry const& e)
	{
		return out << "git::tree::entry{"
		           << e.type() << ", "
		           << e.filemode() << ", "
		           << "}";
	}
}

#endif

