#ifndef  GIT_REPOSITORY_HPP
# define GIT_REPOSITORY_HPP

# include "common.hpp"
# include "config.hpp"
# include "reference.hpp"
# include "index.hpp"
# include "path.hpp"
# include "signature.hpp"
# include "odb.hpp"
# include "object.hpp"

# include <git2/repository.h>

# include <string>

namespace git {

	class repository
	{
	private:
		typedef git::path path_t;
		typedef git::config config_t;
		typedef git::index index_t;
		typedef git::reference reference_t;
		typedef git::odb odb_t;

	private:
		git_repository* _repository;

	public:
		inline explicit
		repository(path_t const& path);

		repository(repository const&) = delete;
		repository& operator =(repository const&) = delete;

		inline
		repository(repository&& other) noexcept;

		inline
		repository& operator =(repository&& other) noexcept;

		inline
		~repository();

		inline
		bool empty() const;

		inline
		path_t path() const;

		inline
		std::string working_directory() const;

		inline
		config_t config() const;

		inline
		reference_t head() const;

		inline
		index_t index() const;

		inline
		odb_t odb() const;

		inline
		weak_signature default_signature() const;

		inline
		git_repository* native() const noexcept;

	private:
		class reference_iterator;

	public:
		inline
		reference_iterator begin() const;

		inline
		reference_iterator end() const noexcept;

	public:
		inline
		object operator [](weak_oid const& id) const;
	};

}

# include "repository.inl"

#endif
