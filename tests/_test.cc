#include "../pymemory.h"
#include <cassert>

namespace py = pyexc;

static PyMethodDef _test_methods[] =
{
	{NULL}
};

PyMODINIT_FUNC
init_test(void)
{
	py::object_ptr m(Py_InitModule3("_test", _test_methods,
		    "Example module."));

	assert(m);
	assert(nullptr != m and m != nullptr);
	assert(m.use_count() == 2);
	assert(not m.unique());

	auto m2 = m;

	assert(m == m2);
	assert(m.use_count() == 3);
	assert(m2.use_count() == 3);
	assert((*m).ob_refcnt == m2->ob_refcnt);

	m2.reset();

	assert(!m2);
	assert(m != m2);
	assert(m.use_count() == 2);

	swap(m, m2);

	assert(nullptr == m and m == nullptr);
	assert(m2.use_count() == 2);

	m = m2;

	assert(m == m2);
	assert(m.use_count() == 3);
	assert(m2.use_count() == 3);
}
