/*-
 * Copyright (c) 2013 Zhihao Yuan.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _PYEXC_PYMEMORY_H
#define _PYEXC_PYMEMORY_H 1

#include <Python.h>
#include <utility>

namespace pyexc {

template <typename T>
struct shared_ptr
{
	using element_type = T;

public:
	shared_ptr() noexcept : p_(nullptr)
	{}

	shared_ptr(std::nullptr_t) noexcept : shared_ptr()
	{}

	explicit shared_ptr(element_type* p) : p_(p)
	{
		Py_XINCREF(p_);
	}

	shared_ptr(shared_ptr&& r) noexcept : p_(r.p_)
	{
		r.p_ = nullptr;
	}

	shared_ptr(shared_ptr const& r) noexcept : p_(r.p_)
	{
		Py_XINCREF(p_);
	}

	~shared_ptr() noexcept
	{
		Py_XDECREF(p_);
	}

	shared_ptr& operator=(shared_ptr r) noexcept
	{
		swap(r);
		return *this;
	}

	void reset() noexcept
	{
		Py_CLEAR(p_);
	}

	void reset(element_type* p)
	{
		shared_ptr(p).swap(*this);
	}

	void swap(shared_ptr& r) noexcept
	{
		std::swap(p_, r.p_);
	}

	element_type* get() const noexcept
	{
		return p_;
	}

	element_type& operator*() const noexcept
	{
		return *p_;
	}

	element_type* operator->() const noexcept
	{
		return p_;
	}

	Py_ssize_t use_count() const noexcept
	{
		return Py_REFCNT(p_);
	}

	bool unique() const noexcept
	{
		return use_count() == 1;
	}

	explicit operator bool() const noexcept
	{
		return get() != nullptr;
	}

private:
	element_type* p_;
};

namespace detail {

template <typename F, typename... Args>
struct shared_result_of
{
	using type = shared_ptr
		<
		    typename std::remove_pointer
		    <
			typename std::result_of<F(Args...)>::type
		    >
		    ::type
		>;
};

}

template <typename F, typename... Args>
auto new_shared(F f, Args... args)
	-> typename detail::shared_result_of<F, Args...>::type
{
	using P = typename detail::shared_result_of<F, Args...>::type;

	return P(f(args...));
}

template <typename T, typename U>
inline
bool operator==(shared_ptr<T> const& x, shared_ptr<U> const& y) noexcept
{
	return x.get() == y.get();
}

template <typename T, typename U>
inline
bool operator!=(shared_ptr<T> const& x, shared_ptr<U> const& y) noexcept
{
	return !(x == y);
}

template <typename T>
inline
bool operator==(std::nullptr_t, shared_ptr<T> const& x) noexcept
{
	return !x;
}

template <typename T>
inline
bool operator==(shared_ptr<T> const& x, std::nullptr_t) noexcept
{
	return !x;
}

template <typename T>
inline
bool operator!=(std::nullptr_t, shared_ptr<T> const& x) noexcept
{
	return bool(x);
}

template <typename T>
inline
bool operator!=(shared_ptr<T> const& x, std::nullptr_t) noexcept
{
	return bool(x);
}

template <typename T>
inline
void swap(shared_ptr<T>& x, shared_ptr<T>& y) noexcept
{
	x.swap(y);
}

using object_ptr = shared_ptr<PyObject>;

}

#endif
