#ifndef  GIT_REFERENCE_HPP
# define GIT_REFERENCE_HPP

# include "common.hpp"

# include <git2/refs.h>

# include <cassert>

namespace git {

	enum class reference_type
	{
		oid = GIT_REF_OID,
		symbolic = GIT_REF_SYMBOLIC,
	};

	class weak_reference
	{
	protected:
		git_reference const* _reference;

	public:
		inline explicit
		weak_reference(git_reference const* ref) noexcept
			: _reference{ref}
		{ assert(ref != nullptr); }

		inline
		weak_reference(weak_reference&&) noexcept = default;
		inline
		weak_reference& operator =(weak_reference&&) noexcept = default;

		inline
		reference_type type() const noexcept
		{ return static_cast<reference_type>(git_reference_type(_reference)); }

		inline
		std::string name() const
		{ return git_reference_name(_reference); }

		inline
		git_reference const* native() const noexcept
		{ return _reference; }

		inline
		bool operator ==(weak_reference const& other) const noexcept
		{
			return git_reference_cmp(
				const_cast<git_reference*>(_reference),
				const_cast<git_reference*>(other._reference)
			);
		}

		inline
		bool operator !=(weak_reference const& other) const noexcept
		{ return !(*this == other); }
	};

	class reference
		: public weak_reference
	{
	private:
		friend class repository;
		inline explicit
		reference(git_reference* ref) noexcept
			: weak_reference{ref}
		{}

	public:
		inline
		reference(reference&& other)
			: weak_reference{std::move(other)}
		{ other._reference = nullptr; }

		reference(reference const&) = delete;
		reference& operator =(reference const&) = delete;

		inline
		~reference()
		{ git_reference_free(this->native()); }

		using weak_reference::native;
		inline
		git_reference* native() noexcept
		{ return const_cast<git_reference*>(_reference); }
	};

}

#endif
