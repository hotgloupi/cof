#ifndef  GIT_TYPES_HPP
# define GIT_TYPES_HPP

# include <git2/types.h>

namespace git {

	enum class filemode_type
	{
		new_ = GIT_FILEMODE_NEW,
		tree = GIT_FILEMODE_TREE,
		blob = GIT_FILEMODE_BLOB,
		blob_executable = GIT_FILEMODE_BLOB_EXECUTABLE,
		link = GIT_FILEMODE_LINK,
		commit = GIT_FILEMODE_COMMIT,
	};

	template<typename Stream>
	Stream& operator <<(Stream& out, filemode_type const type)
	{
		switch (type)
		{
		case filemode_type::blob:
			return out << "filemode_type::blob";
		case filemode_type::blob_executable:
			return out << "filemode_type::blob_executable";
		case filemode_type::commit:
			return out << "filemode_type::commit";
		case filemode_type::link:
			return out << "filemode_type::link";
		case filemode_type::new_:
			return out << "filemode_type::new";
		case filemode_type::tree:
			return out << "filemode_type::tree";
		}
		return out << "filemode_type::INVALID";
	}

	enum class object_type
	{
		commit = GIT_OBJ_COMMIT,
		tree = GIT_OBJ_TREE,
		blob = GIT_OBJ_BLOB,
		tag = GIT_OBJ_TAG,
		offset_delta = GIT_OBJ_OFS_DELTA,
		reference_delta = GIT_OBJ_REF_DELTA,
		invalid
	};

	template<typename Stream>
	Stream& operator <<(Stream& out, object_type const type)
	{
		switch (type)
		{
		case object_type::blob:
			return out << "object_type::blob";
		case object_type::commit:
			return out << "object_type::commit";
		case object_type::offset_delta:
			return out << "object_type::offset_delta";
		case object_type::reference_delta:
			return out << "object_type::reference_delta";
		case object_type::tag:
			return out << "object_type::tag";
		case object_type::tree:
			return out << "object_type::tree";
		default:
			return out << "object_type::INVALID";
		}
	}

	namespace detail {

		inline
		filemode_type from(git_filemode_t const m) noexcept
		{ return static_cast<filemode_type>(m); }

		inline
		object_type from(git_otype const type) noexcept
		{
			auto t = static_cast<object_type>(type);
			switch (t)
			{
			case object_type::blob:
			case object_type::commit:
			case object_type::offset_delta:
			case object_type::reference_delta:
			case object_type::tag:
			case object_type::tree:
				return t;
			default:
				return object_type::invalid;
			}
		}

	}

}

#endif
