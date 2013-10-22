#ifndef  GIT_REPOSITORY_INL
# define GIT_REPOSITORY_INL

# include "repository.hpp"

namespace git {

	repository::repository(path_t const& path)
		: _repository{nullptr}
	{
		check_result(git_repository_open(&_repository, path.c_str()));
	}

	repository::repository(repository&& other) noexcept
		: _repository{other._repository}
	{ other._repository = nullptr; }

	repository& repository::operator =(repository&& other) noexcept
	{
		_repository = other._repository;
		other._repository = nullptr;
		return *this;
	}

	repository::~repository()
	{
		git_repository_free(_repository);
	}

	bool repository::empty() const
	{
		int res = git_repository_is_empty(_repository);
		if (res > 1)
			check_result(res);
		return res;
	}

	repository::path_t repository::path() const
	{ return path_t{git_repository_path(_repository)}; }

	std::string repository::working_directory() const
	{
		if (char const* path = git_repository_workdir(_repository))
			return {path};
		throw std::runtime_error(
			"Bare repository has no working directory"
		);
	}

	repository::config_t repository::config() const
	{
		git_config* out = nullptr;
		check_result(git_repository_config(&out, _repository));
		return config_t{out};
	}

	repository::reference_t repository::head() const
	{
		git_reference* out = nullptr;
		check_result(git_repository_head(&out, _repository));
		return reference{out};
	}

	repository::index_t repository::index() const
	{
		git_index* out = nullptr;
		check_result(git_repository_index(&out, _repository));
		index_t res{out};
		res.read();
		return res;
	}

	repository::odb_t repository::odb() const
	{
		git_odb* out = nullptr;
		check_result(git_repository_odb(&out, _repository));
		return odb_t{out};
	}

	weak_signature repository::default_signature() const
	{
		git_signature* out;
		check_result(git_signature_default(&out, _repository));
		return weak_signature{out};
	}

	git_repository* repository::native() const noexcept { return _repository; }

	class repository::reference_iterator
	{
	private:
		git_reference_iterator* _it;
		git_reference*          _ref;

	public:
		inline explicit
		reference_iterator(git_reference_iterator* it) noexcept
			: _it{it}
			, _ref{nullptr}
		{}

		inline
		reference_iterator(reference_iterator&& other) noexcept
			: _it{other._it}
			, _ref{other._ref}
		{ other._it = nullptr; other._ref = nullptr; }

		inline
		~reference_iterator()
		{
			if (_it != nullptr) git_reference_iterator_free(_it);
			if (_ref != nullptr) git_reference_free(_ref);
		}

	private:
		inline
		void _next()
		{
			if (_ref == nullptr)
			{
				auto res = git_reference_next(&_ref, _it);
				if (res == GIT_ITEROVER)
				{
					git_reference_iterator_free(_it);
					_it = nullptr;
				}
				else
					check_result(res);
			}
		}

	public:
		inline
		bool operator ==(reference_iterator const& other) const noexcept
		{ return _it == other._it; }

		inline
		bool operator !=(reference_iterator const& other) const noexcept
		{ return !(*this == other); }

		inline
		reference_iterator& operator ++()
		{ git_reference_free(_ref); _ref = nullptr; _next(); return *this; }

		inline
		reference_iterator& operator ++(int)
		{ git_reference_free(_ref); _ref = nullptr; _next(); return *this; }

	private:
		class weak_reference_ptr
		{
		private:
			weak_reference _ref;
		public:
			inline
			weak_reference_ptr(git_reference const* ref) noexcept
				: _ref{ref}
			{}

			inline
			weak_reference* operator ->() noexcept
			{ return &_ref; }
		};

	public:
		inline
		weak_reference_ptr operator ->() noexcept
		{ _next(); return weak_reference_ptr{_ref}; }

		inline
		weak_reference operator *() noexcept
		{ _next(); return weak_reference{_ref}; }
	};

	repository::reference_iterator repository::begin() const
	{
		git_reference_iterator* out = nullptr;
		check_result(git_reference_iterator_new(&out, _repository));
		return reference_iterator{out};
	}

	repository::reference_iterator repository::end() const noexcept
	{ return reference_iterator{nullptr}; }

	object repository::operator [](weak_oid const& id) const
	{
		git_object* out = nullptr;
		check_result(
			git_object_lookup(
			    &out,
			    _repository,
			    id.native(),
			    GIT_OBJ_ANY
			)
		);
		return object{out};
	}
}

#endif
