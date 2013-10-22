#ifndef  GIT_DETAIL_ITERATOR_HPP
# define GIT_DETAIL_ITERATOR_HPP

# include <git2/types.h>

# include <utility>

namespace git { namespace detail {

	template<typename Container, typename Entry>
	class iterator
	{
	private:
		Container const& _container;
		size_t           _entry_index;

	public:
		iterator(Container const& container,
		         size_t const entry_index) noexcept
			: _container(container)
			, _entry_index{entry_index}
		{}

		iterator& operator ++() noexcept
		{ ++_entry_index; return *this; }

		iterator& operator ++(int) noexcept
		{ ++_entry_index; return *this; }

		bool operator ==(iterator const& other) const noexcept
		{ return _entry_index == other._entry_index; }

		bool operator !=(iterator const& other) const noexcept
		{ return !(*this == other); }

	private:
		class weak_entry_ptr
		{
		private:
			Entry _entry;

		public:
			explicit
			weak_entry_ptr(Entry&& entry) noexcept
				: _entry{std::move(entry)}
			{}

			Entry* operator ->() noexcept { return &_entry; }
		};

	public:
		weak_entry_ptr operator ->()
		{ return weak_entry_ptr{_container[_entry_index]};}

		Entry operator *()
		{ return _container[_entry_index]; }
	};

}}

#endif
