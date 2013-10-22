#ifndef  GIT_INDEX_INL
# define GIT_INDEX_INL

# include "index.hpp"

namespace git {

	index::index(git_index* idx) noexcept
		: _index{idx}
	{}

	index::~index()
	{ git_index_free(_index); }

	path index::path() const
	{
		if (auto path = git_index_path(_index))
			return {path};
		throw std::runtime_error{"In-memory index has no path"};
	}

	void index::read()
	{ check_result(git_index_read(_index)); }

	void index::write()
	{ check_result(git_index_write(_index)); }

	void index::clear() noexcept
	{ git_index_clear(_index); }

	class index::weak_entry
	{
	private:
		git_index_entry const* _entry;

	private:
		friend class index;
		explicit
		weak_entry(git_index_entry const* entry) noexcept
			: _entry{entry}
		{}
	public:
		weak_entry(weak_entry&& other)
			: _entry{other._entry}
		{}

	public:
		git_index_time const& ctime() const { return _entry->ctime; }
		git_index_time const& mtime() const { return _entry->mtime; }
		unsigned int dev() const { return _entry->dev; }
		unsigned int ino() const { return _entry->ino; }
		unsigned int mode() const { return _entry->mode; }
		unsigned int uid() const { return _entry->uid; }
		unsigned int gid() const { return _entry->gid; }
		git_off_t file_size() const { return _entry->file_size; }

		weak_oid oid() const { return weak_oid{&_entry->oid}; }

		unsigned short flags() { return _entry->flags; }
		unsigned short flags_extended() { return _entry->flags_extended; }

		git::path path() { return {_entry->path}; }
	};

	size_t index::count() const noexcept
	{ return git_index_entrycount(_index); }

	index::weak_entry index::operator [](size_t const idx) const
	{
		if (auto ptr = git_index_get_byindex(_index, idx))
			return weak_entry{ptr};
		throw std::out_of_range{"No entry found"};
	}

	index::weak_entry index::operator [](class path const& path) const
	{
		if (auto ptr = git_index_get_bypath(_index,
		                                    path.c_str(),
		                                    GIT_INDEX_STAGE_ANY))
			return weak_entry{ptr};
		throw std::out_of_range{"No entry found"};
	}

	index::iterator index::begin() const
	{ return iterator(*this, 0); }

	index::iterator index::end() const
	{ return iterator(*this, this->count()); }

}

#endif
