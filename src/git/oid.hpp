#ifndef  GIT_OID_HPP
# define GIT_OID_HPP

# include "common.hpp"

# include <string>

# include <git2/oid.h>

namespace git {

	class weak_oid
	{
	public:
		static size_t const raw_size = GIT_OID_RAWSZ;
		static size_t const hex_size = GIT_OID_HEXSZ;

	private:
		git_oid const* _oid;

	public:
		inline explicit
		weak_oid(git_oid const* oid)
			: _oid{oid}
		{}

		inline
		std::string string() const
		{
			std::string res;
			res.resize(hex_size);
			git_oid_tostr(&res[0], hex_size + 1, _oid);
			return res;
		}

		inline
		bool operator ==(weak_oid const& other) const noexcept
		{ return git_oid_equal(_oid, other._oid); }

		inline
		bool operator <(weak_oid const& other) const noexcept
		{ return git_oid_cmp(_oid, other._oid) < 0; }

		inline
		git_oid const* native() const noexcept
		{ return _oid; }
	};

	class oid
		: public weak_oid
	{

	private:
		git_oid _oid;

	private:
		inline
		oid()
			: weak_oid{&_oid}
		{}

	public:
		inline explicit
		oid(std::string const& str)
			: oid{}
		{ check_result(git_oid_fromstrn(&_oid, str.c_str(), str.size())); }

		inline
		oid(oid const& other) noexcept
			: oid{}
		{ git_oid_cpy(&_oid, &other._oid); }

		inline
		oid& operator =(oid const& other) noexcept
		{ git_oid_cpy(&_oid, &other._oid); return *this; }

		using weak_oid::operator ==;
		using weak_oid::operator <;
	};

	template<typename Stream>
	Stream& operator <<(Stream& out, weak_oid const& o)
	{ return out << o.string(); }

}

#endif
