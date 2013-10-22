#ifndef  GIT_COMMIT_HPP
# define GIT_COMMIT_HPP

# include "common.hpp"
# include "object.hpp"
# include "time.hpp"
# include "signature.hpp"
# include "tree.hpp"

# include <git2/commit.h>

namespace git {

	class commit
	{
	private:
		git_commit* _commit;

	public:
		/* implicit */ inline
		commit(object&& object)
			: _commit{reinterpret_cast<git_commit*>(object.native())}
		{
			if (object.type() != object_type::commit)
				throw std::runtime_error{"Invalid cast from object to tree"};
			object._object = nullptr;
		}

		inline
		~commit()
		{ git_commit_free(_commit); }

		inline
		weak_oid id() const noexcept
		{ return weak_oid{git_commit_id(_commit)}; }

		inline
		std::string message() const
		{ return std::string{git_commit_message(_commit)}; }

		inline
		git::time time() const noexcept
		{
			return git::time{
				git_commit_time(_commit),
				git_commit_time_offset(_commit),
			};
		}

		inline
		weak_signature committer() const noexcept
		{ return weak_signature{git_commit_committer(_commit)}; }

		inline
		weak_signature author() const noexcept
		{ return weak_signature{git_commit_author(_commit)}; }

		inline
		git::tree tree() const
		{
			git_tree* out = nullptr;
			check_result(git_commit_tree(&out, _commit));
			return git::tree{out};
		}
	};

	template<typename Stream>
	Stream& operator <<(Stream& out, commit const& c)
	{
		return out << "git::commit{" << c.id() << ", " << c.author() << "}";
	}
}

#endif
