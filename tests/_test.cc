#include "../pymemory.h"
#include <cassert>

namespace py = pyexc;

static PyMethodDef _test_methods[] =
{
	{NULL}
};

static py::object_ptr get_helloworld()
{
	auto s = py::new_shared(PyString_FromString, "Hello, world");

	assert(s.use_count() == 2);
	assert(PyString_Size(s.get()) == 12);

	return s;
}

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

	auto s = get_helloworld();

	assert(s.use_count() == 2);

	PyModule_AddObject(m.get(), "s", s.get());
}
