/*
 * propertypp - Objective-C-like properties for C++ classes.
 *
 * Copyright (c) 2011 Cesar L. B. Silveira
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef PROPERTY_H
#define PROPERTY_H

#include <functional>

namespace propertypp
{

    template <typename return_type, typename argument_type>
    class vholder
    {
        public:
            virtual return_type operator()(argument_type) = 0;
    };

    template <typename fobj_type,
              typename return_type, typename argument_type>
    class holder : public vholder<return_type, argument_type>
    {
        public:
            holder(const fobj_type& f) : f_(f) { }

            return_type operator()(argument_type a) { return f_(a); }

        private:
            fobj_type f_;
    };

    template <typename signature_type> class ftor;
    template <typename return_type, typename argument_type>
    class ftor<return_type (argument_type)>
    {
        public:
            ftor()
                : f_(0)
            { }

            template <typename fobj_type>
            ftor(const fobj_type& f)
                : f_(new holder<fobj_type, return_type, argument_type>(f))
            { }

            return_type operator()(argument_type a) { return (*f_)(a); }

        private:
            vholder<return_type, argument_type>* f_;
    };

    template <typename property_type>
    class property_base
    {
        public:
            property_base() { }

            template <typename class_type, typename get_type, typename set_type>
            property_base(class_type* obj, get_type get, set_type set)
                : get_(std::bind1st(std::mem_fun(get), obj)),
                  set_(std::bind1st(std::mem_fun(set), obj))
            { }

            operator property_type() { return this->get_(0); }

            property_type operator=(property_type rhs)
            { return this->set_(rhs); }

            property_type operator++()
            { return this->set_(this->get_(0) + 1); }

            property_type operator++(int)
            {
                property_type retVal = this->get_(0);
                this->set_(this->get_(0) + 1);
                return retVal;
            }

            property_type operator--()
            { return this->set_(this->get_(0) - 1); }

            property_type operator--(int)
            {
                property_type retVal = this->get_(0);
                this->set_(this->get_(0) - 1);
                return retVal;
            }

            template <typename T>
            property_type operator+=(const T& rhs)
            { return this->set_(this->get_(0) + rhs); }

            template <typename T>
            property_type operator-=(const T& rhs)
            { return this->set_(this->get_(0) - rhs); }

            template <typename T>
            property_type operator*=(const T& rhs)
            { return this->set_(this->get_(0) * rhs); }

            template <typename T>
            property_type operator/=(const T& rhs)
            { return this->set_(this->get_(0) / rhs); }

            template <typename T>
            property_type operator%=(const T& rhs)
            { return this->set_(this->get_(0) % rhs); }

            template <typename T>
            property_type operator&=(const T& rhs)
            { return this->set_(this->get_(0) & rhs); }

            template <typename T>
            property_type operator|=(const T& rhs)
            { return this->set_(this->get_(0) | rhs); }

            template <typename T>
            property_type operator^=(const T& rhs)
            { return this->set_(this->get_(0) ^ rhs); }

            template <typename T>
            property_type operator<<=(const T& rhs)
            { return this->set_(this->get_(0) << rhs); }

            template <typename T>
            property_type operator>>=(const T& rhs)
            { return this->set_(this->get_(0) >> rhs); }

        protected:
            propertypp::ftor<property_type (int)> get_;
            propertypp::ftor<property_type (property_type)> set_;
    };

    template <typename property_type>
    class property : public property_base<property_type>
    {
        public:
            typedef property_type type;

            property() { }

            template <typename class_type, typename get_type, typename set_type>
            property(class_type* obj, get_type get, set_type set)
                : property_base<property_type>(obj, get, set)
            { }

            using property_base<property_type>::operator property_type;
            using property_base<property_type>::operator=;
            using property_base<property_type>::operator++;
            using property_base<property_type>::operator--;
    };

    template <typename property_type>
    class property<property_type*> : public property_base<property_type*>
    {
        public:
            typedef property_type* type;

            property() { }

            template <typename class_type, typename get_type, typename set_type>
            property(class_type* obj, get_type get, set_type set)
                : property_base<property_type*>(obj, get, set)
            { }

            using property_base<property_type*>::operator property_type*;
            using property_base<property_type*>::operator=;
            using property_base<property_type*>::operator++;
            using property_base<property_type*>::operator--;

            property_type* operator->() { return this->get_(0); }

            property_type& operator*() { return *this->get_(0); }
    };

} // namespace propertypp

#define PROPERTYPP_DECLARE(property_type, property_name, member_name)   \
    private:                                                            \
    property_type _ppp_get ## property_name(int) const                  \
    {                                                                   \
        return get ## property_name();                                  \
    }                                                                   \
    property_type _ppp_set ## property_name(property_type rhs)          \
    {                                                                   \
        set ## property_name(rhs);                                      \
        return member_name;                                             \
    }                                                                   \
    template<typename class_type>                                       \
    void _ppp_init_ ## property_name(class_type* obj)                   \
    {                                                                   \
        property_name = propertypp::property<property_type>(            \
            obj,                                                        \
            &class_type::_ppp_get ## property_name,                     \
            &class_type::_ppp_set ## property_name);                    \
        member_name = 0;                                                \
    }                                                                   \
    template <typename class_type>                                      \
    void _ppp_copy_ ## property_name(class_type* obj, const class_type& rhs) \
    {                                                                   \
        _ppp_init_ ## property_name(obj);                               \
        member_name = rhs.member_name;                                  \
    }                                                                   \
    property_type member_name;                                          \
public:                                                                 \
property_type get ## property_name() const;                             \
void set ## property_name(property_type);                               \
propertypp::property<property_type> property_name;

#define PROPERTYPP_SYNTHESIZE(class, type, property_name, member_name)\
    type class::get ## property_name() const { return member_name; } \
    void class::set ## property_name(type rhs) { member_name = rhs; }

#define PROPERTYPP_SYNTHESIZE_GETTER(class, type, property_name, member_name) \
    type class::get ## property_name() const { return member_name; }

#define PROPERTYPP_SYNTHESIZE_SETTER(class, type, property_name, member_name) \
    void class::set ## property_name(type rhs) { member_name = rhs; }

#define PROPERTYPP_INIT(property_name) _ppp_init_ ## property_name(this);

#define PROPERTYPP_COPY(property_name, rhs) \
    _ppp_copy_ ## property_name(this, rhs)

#endif // PROPERTY_H
