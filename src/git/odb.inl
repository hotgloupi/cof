#ifndef  GIT_ODB_INL
# define GIT_ODB_INL

# include "odb.hpp"

namespace git {

	odb::odb(git_odb* odb) noexcept
		: _odb{odb}
	{}

	odb::~odb()
	{ git_odb_free(_odb); }

	namespace {

		template<typename Visitor>
		struct odb_foreach_visitor
		{
			Visitor            visitor;
			std::exception_ptr exception;

			odb_foreach_visitor(Visitor visitor)
				: visitor(std::forward<Visitor>(visitor))
				, exception{}
			{}
		};

		template<typename Visitor>
		int odb_foreach_visitor_cb(git_oid const* id, void* payload)
		{
			auto v = reinterpret_cast<odb_foreach_visitor<Visitor>*>(payload);

			try {
				v->visitor(weak_oid{id});
			} catch (...) {
				v->exception = std::current_exception();
				return -1;
			}
			return 0;
		}
	}

	template<typename Visitor>
	void odb::visit(Visitor&& cb)
	{
		odb_foreach_visitor<Visitor> v{std::forward<Visitor>(cb)};
		int res = git_odb_foreach(_odb, &odb_foreach_visitor_cb<Visitor>, &v);
		if (res == GIT_EUSER)
			std::rethrow_exception(v.exception);
		check_result(res);
	}
}

#endif
