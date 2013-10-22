#ifndef  GIT_OBJECT_HPP
# define GIT_OBJECT_HPP

# include "common.hpp"
# include "oid.hpp"
# include "types.hpp"

# include <git2/object.h>

namespace git {

	class object
	{
	private:
		friend class commit;
		friend class blob;
		friend class tree;
		git_object* _object;

	private:
		friend class repository;
		explicit inline
		object(git_object* obj) noexcept
			: _object{obj}
		{}

	public:
		inline
		object(object&& other) noexcept
			: _object{other._object}
		{ other._object = nullptr; }

		inline
		~object()
		{ git_object_free(_object); }

	public:
		weak_oid id() const noexcept
		{ return weak_oid{git_object_id(_object)}; }

		object_type type() const noexcept
		{ return detail::from(git_object_type(_object)); }

		inline
		git_object* native() noexcept
		{ return _object; }
	};

	template<typename Stream>
	Stream& operator <<(Stream& out, object const& o)
	{
		return out << "object{" << o.id() << ", " <<  o.type() << "}";
	}
}

#endif
