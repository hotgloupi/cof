#ifndef  GIT_SIGNATURE_HPP
# define GIT_SIGNATURE_HPP

# include "common.hpp"
# include "time.hpp"

# include <git2/signature.h>

# include <string>
# include <utility>

namespace git {

	class weak_signature
	{
	protected:
		git_signature const* _signature;

	public:
		inline explicit
		weak_signature(git_signature const* signature) noexcept
			: _signature{signature}
		{}

		std::string name() const
		{ return _signature->name; }

		std::string email() const
		{ return _signature->email; }

		time when() const
		{ return time{_signature->when}; }
	};

	class signature
		: public weak_signature
	{
	public:
		inline explicit
		signature(std::string const& name,
		          std::string const& email,
		          time t = time::now())
			: weak_signature{nullptr}
		{
			git_signature* out = nullptr;
			check_result(
				git_signature_new(
					&out,
					name.c_str(),
					email.c_str(),
					t.stamp(),
					t.offset()
				)
			);
			_signature = out;
		}

		inline
		~signature()
		{ git_signature_free(this->native()); }

	public:
		inline
		git_signature* native() noexcept
		{ return const_cast<git_signature*>(_signature); }
	};

	template<typename Stream>
	Stream& operator <<(Stream& out, weak_signature const& sig)
	{
		return out << "git::signature{"
		           << sig.name() << ", "
		           << sig.email() << ", "
		           << sig.when()
		           << "}";
	}
}

#endif
