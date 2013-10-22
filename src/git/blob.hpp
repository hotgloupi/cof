#ifndef  GIT_BLOB_HPP
# define GIT_BLOB_HPP

# include "common.hpp"
# include "object.hpp"

# include <git2/blob.h>

namespace git {

	class blob
	{
	private:
		git_blob* _blob;

	private:
		friend class repository;
		inline explicit
		blob(git_blob* blob) noexcept
			: _blob{blob}
		{}

	public:
		inline /* implicit */
		blob(object&& other)
			: _blob{reinterpret_cast<git_blob*>(other._object)}
		{
			if (other.type() != object_type::blob)
				throw std::runtime_error{"Wrong object type"};
			other._object = nullptr;
		}

		inline
		blob(blob&& other) noexcept
			: _blob{other._blob}
		{ other._blob = nullptr; }

		inline
		~blob() { git_blob_free(_blob); }

	public:
		inline
		weak_oid id() const noexcept
		{ return weak_oid{git_blob_id(_blob)}; }

		inline
		void const* content() const noexcept
		{ return git_blob_rawcontent(_blob); }

		inline
		size_t size() const noexcept
		{ return git_blob_rawsize(_blob); }

		inline
		bool binary() const noexcept
		{ return git_blob_is_binary(_blob); }
	};

	template<typename Stream>
	Stream& operator <<(Stream& out, blob const& b)
	{
		return out << "git::blob{"
		           << b.id() << ", " << b.size() << " bytes of "
		           << (b.binary() ? "binary" : "text") << " data"
		           << "}";
	}
}

#endif
