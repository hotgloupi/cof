#ifndef  GIT_REFLOG_HPP
# define GIT_REFLOG_HPP

# include "common.hpp"
# include "reference.hpp"
# include "oid.hpp"
# include "detail/iterator.hpp"
# include "signature.hpp"

# include <git2/reflog.h>

namespace git {

	class reflog
	{
	private:
		git_reflog* _reflog;

	public:
		reflog(weak_reference const& ref)
			: _reflog{nullptr}
		{ check_result(git_reflog_read(&_reflog, ref.native())); }

		~reflog()
		{ git_reflog_free(_reflog); }

	public:
		class weak_entry
		{
		private:
			git_reflog_entry const* _entry;

		public:
			explicit
			weak_entry(git_reflog_entry const* entry) noexcept
				: _entry{entry}
			{}

			weak_oid id_old() const
			{ return weak_oid{git_reflog_entry_id_old(_entry)}; }

			weak_oid id_new() const
			{ return weak_oid{git_reflog_entry_id_new(_entry)}; }

			weak_signature committer() const
			{ return weak_signature{git_reflog_entry_committer(_entry)}; }

			std::string message() const
			{ return std::string{git_reflog_entry_message(_entry)}; }
		};

	public:
		size_t count() const noexcept
		{ return git_reflog_entrycount(_reflog); }

		weak_entry operator [](size_t const idx) const
		{
			if (auto res = git_reflog_entry_byindex(_reflog, idx))
				return weak_entry{res};
			throw std::range_error{"Index out of range"};
		}

		typedef detail::iterator<reflog, weak_entry> iterator;

		iterator begin() const noexcept
		{ return iterator{*this, 0}; }

		iterator end() const noexcept
		{ return iterator{*this, this->count()}; }
	};


	template<typename Stream>
	Stream& operator <<(Stream& out, reflog::weak_entry const& entry)
	{
		return out << "git::reflog::entry{"
		           << entry.message() << ", "
		           //<< entry.id_old() << ", "
		           << entry.id_new() << ", "
		           << entry.committer()
		           << "}";
	}

}

#endif
