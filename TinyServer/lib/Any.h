//
// Created by echo-djx on 2021/8/18.
//
//any类设计要点
//1.any类不能是一个模板类形如int i;any<int>anyValue=i;无意义，还不如直接写int anyValue =i;
//2.any必须提供模板构造函数（赋值操作不必是模板的），才能完成如下操作:
//  int i; long j; struct X; X x;any anyValue(i); anyValue=j; anyValue =x;
//3.必须提供某些有关它所保存的对象型别的信息。
//4.它必须能保存数值且提供某种方法将它保存的数值“取出来”。
//5.数据不能放在any类里，这会使any类成为模板类，不符合1的要求。
//  数据应该动态存放，即动态分配一个数据的容器来存放数据，而any类中则保存指向这个容器的指针。
//  明确地说，是指向这个容器的基类的指针，这是因为容器本身必须为模板，而any类中的指针成员又必须不是泛型的。
//  因为any不能是泛型的，所以any中所有数据成员都不能是泛型的。
//  所以，结论是：为容器准备一个非泛型的基类，而让指针指向该基类。
#pragma once
#include <new>
#include <typeinfo>
#include <utility>

namespace detail {

    template <bool B>
    using _bool_constant = std::integral_constant<bool, B>;

    // _or

    template<typename...>
    struct _or;

    template<>
    struct _or<>
            : public std::false_type
            { };

    template<typename _B1>
    struct _or<_B1>
            : public _B1
            { };

    template<typename _B1, typename _B2>
    struct _or<_B1, _B2>
            : public std::conditional<_B1::value, _B1, _B2>::type
            { };

    template<typename _B1, typename _B2, typename _B3, typename... _Bn>
    struct _or<_B1, _B2, _B3, _Bn...>
            : public std::conditional<_B1::value, _B1, _or<_B2, _B3, _Bn...>>::type
            { };

    // _and

    template<typename...>
    struct _and;

    template<>
    struct _and<>
            : public std::true_type
            { };

    template<typename _B1>
    struct _and<_B1>
            : public _B1
            { };

    template<typename _B1, typename _B2>
    struct _and<_B1, _B2>
            : public std::conditional<_B1::value, _B2, _B1>::type
            { };

    template<typename _B1, typename _B2, typename _B3, typename... _Bn>
    struct _and<_B1, _B2, _B3, _Bn...>
            : public std::conditional<_B1::value, _and<_B2, _B3, _Bn...>, _B1>::type
            { };

    // _and_v

    template<typename... _Bn>
    constexpr bool _and_v() noexcept
    {
        return _and<_Bn...>::value;
    }

    // _not

    template<typename _Pp>
    struct _not
            : public _bool_constant<!bool(_Pp::value)>
            { };

    // _in_place_type_t

    template<typename _Tp>
    struct _in_place_type_t
            {
        explicit _in_place_type_t() = default;
            };

    // _is_in_place_type

    template<typename>
    struct _is_in_place_type_impl : std::false_type
            { };

    template<typename _Tp>
    struct _is_in_place_type_impl<_in_place_type_t<_Tp>> : std::true_type
            { };

    template<typename _Tp>
    struct _is_in_place_type
            : public _is_in_place_type_impl<_Tp>
            { };

} // namespace detail

/**
 * \class bad_any_cast
 * \brief Exception thrown by the value-returning forms of any_cast on a type mismatch.
 *
 * Defines a type of object to be thrown by the value-returning forms of any_cast on failure.
 */
class bad_any_cast : public std::bad_cast
        {
        public:
            /**
             * Returns an explanatory string.
             */
            virtual const char* what() const noexcept;
        };

[[noreturn]] inline void throw_bad_any_cast()
{
    throw bad_any_cast{};
}

/**
 * \class any
 * \brief The class any describes a type-safe container for single values of any type.
 *
 * An object of class any stores an instance of any type that satisfies the constructor requirements or is empty,
 * and this is referred to as the state of the class any object. The stored instance is called the contained object.
 * Two states are equivalent if they are either both empty or if both are not empty and if the contained objects are equivalent.
 *
 * The non-member any_cast functions provide type-safe access to the contained object.
 */
class any
        {
    // Holds either pointer to a heap object or the contained object itself.
    union Storage
            {
        constexpr Storage()
        : m_ptr(nullptr) {}

        // Prevent trivial copies of this type, buffer might hold a non-POD.
        Storage(const Storage&) = delete;
        Storage& operator=(const Storage&) = delete;

        void* m_ptr;
        std::aligned_storage<sizeof(m_ptr), alignof(void*)>::type m_buffer;
            };

    template<typename _Tp, typename _Safe = std::is_nothrow_move_constructible<_Tp>,
            bool _Fits = (sizeof(_Tp) <= sizeof(Storage)) && (alignof(_Tp) <= alignof(Storage))>
                    using Internal = std::integral_constant<bool, _Safe::value && _Fits>;

    template<typename _Tp>
    struct Manager_internal; // uses small-object optimization

    template<typename _Tp>
    struct Manager_external; // creates contained object on the heap

    template<typename _Tp>
    using Manager = typename std::conditional<Internal<_Tp>::value,
    Manager_internal<_Tp>, Manager_external<_Tp>>::type;

    template<typename _Tp, typename _Decayed = typename std::decay<_Tp>::type>
            using Decay = typename std::enable_if<!std::is_same<_Decayed, any>::value, _Decayed>::type;

    /** Emplace with an object created from \a args as the contained object. */
    template <typename _Tp, typename... _Args, typename Mgr = Manager<_Tp>>
            void _do_emplace(_Args&&... args)
            {
                reset();
                Mgr::S_create(m_storage, std::forward<_Args>(args)...);
                m_manager = &Mgr::S_manage;
            }

            /** Emplace with an object created from \a list and \a list as the contained object. */
            template <typename _Tp, typename _Up, typename... _Args,
                    typename Mgr = Manager<_Tp>>
                            void _do_emplace(std::initializer_list<_Up> list, _Args&&... args)
                            {
                                reset();
                                Mgr::S_create(m_storage, list, std::forward<_Args>(args)...);
                                m_manager = &Mgr::S_manage;
                            }

        public:
            //
            //  construct/destruct
            //

            /**
             * Default constructor, creates an empty object.
             */
            constexpr any() noexcept;
            /**
             * Copy constructor, copies the state of \a other
             */
            any(const any& other);
            /**
             * Move constructor, transfer the state from \a other.
             */
            any(any&& other) noexcept;

            template <typename _Res, typename _Tp, typename... _Args>
            using _any_constructible = std::enable_if<detail::_and<std::is_copy_constructible<_Tp>,
            std::is_constructible<_Tp, _Args...>>::value, _Res>;

            template <typename _Tp, typename... _Args>
            using _any_constructible_t = typename _any_constructible<bool, _Tp, _Args...>::type;

            /**
             * Construct with a copy of \a value as the contained object.
             */
            template <typename _ValueType, typename _Tp = Decay<_ValueType>,
                    typename Mgr = Manager<_Tp>,
                    _any_constructible_t<_Tp, _ValueType&&> = true,
                    typename std::enable_if<!detail::_is_in_place_type<_Tp>::value, bool>::type = true>
                            any(_ValueType&& value)
                            : m_manager(&Mgr::S_manage)
                            {
                                Mgr::S_create(m_storage, std::forward<_ValueType>(value));
                            }

                            /**
                             * Construct with a copy of \a value as the contained object.
                             */
                            template <typename _ValueType, typename _Tp = Decay<_ValueType>,
                                    typename Mgr = Manager<_Tp>,
                                    typename std::enable_if<detail::_and_v<std::is_copy_constructible<_Tp>,
                                    detail::_not<std::is_constructible<_Tp, _ValueType&&>>,
                                    detail::_not<detail::_is_in_place_type<_Tp>>>(), bool>::value = false>
                                            any(_ValueType&& value)
                                            : m_manager(&Mgr::S_manage)
                                            {
                                                Mgr::S_create(m_storage, value);
                                            }

                                            /**
                                             * Construct with an object created from @p __args as the contained object.
                                             */
                                            template <typename _ValueType, typename... _Args,
                                                    typename _Tp = Decay<_ValueType>,
                                                    typename Mgr = Manager<_Tp>,
                                                    _any_constructible_t<_Tp, _Args&& ... > = false>
                                                            explicit any(detail::_in_place_type_t<_ValueType>, _Args&&... args)
                                                            : m_manager(&Mgr::S_manage)
                                                            {
                                                                Mgr::S_create(m_storage, std::forward<_Args>(args)...);
                                                            }

                                                            /**
                                                             * Construct with an object created from \a list and \a args as the contained object.
                                                             */
                                                            template <typename _ValueType, typename _Up, typename... _Args,
                                                                    typename _Tp = Decay<_ValueType>, typename Mgr = Manager<_Tp>,
                                                                    _any_constructible_t<_Tp, std::initializer_list<_Up>, _Args&&...> = false>
                                                                            explicit any(detail::_in_place_type_t<_ValueType>, std::initializer_list<_Up> list, _Args&&... args)
                                                                            : m_manager(&Mgr::S_manage)
                                                                            {
                                                                                Mgr::S_create(m_storage, list, std::forward<_Args>(args)...);
                                                                            }

                                                                            /**
                                                                             * Destructor, calls \c reset()
                                                                             */
                                                                            ~any();

                                                                            //
                                                                            //  assignments
                                                                            //

                                                                            /**
                                                                             * Copy the state of another object.
                                                                             */
                                                                            any& operator=(const any& rhs);

                                                                            /**
                                                                             * Move assignment operator
                                                                             *
                                                                             * \note \c !rhs.has_value() (not guaranteed for other implementations)
                                                                             */
                                                                            any& operator=(any&& rhs) noexcept;

                                                                            /**
                                                                             * Store a copy of \a rhs as the contained object.
                                                                             */
                                                                            template<typename _ValueType>
                                                                            typename std::enable_if<std::is_copy_constructible<Decay<_ValueType>>::value, any&>::type
                                                                            operator=(_ValueType&& rhs)
                                                                                    {
                                                                                *this = any(std::forward<_ValueType>(rhs));
                                                                                return *this;
                                                                                    }

                                                                                    /**
                                                                                     * Emplace with an object created from @p __args as the contained object.
                                                                                     */
                                                                                    template <typename _ValueType, typename... _Args>
                                                                                    typename _any_constructible<Decay<_ValueType>&, Decay<_ValueType>, _Args&&...>::type
                                                                                    emplace(_Args&&... args)
                                                                                    {
                                                                                        _do_emplace<Decay<_ValueType>>(std::forward<_Args>(args)...);
                                                                                        any::ManageArg marg;
                                                                                        this->m_manager(any::MOAccess, this, &marg);
                                                                                        return *static_cast<Decay<_ValueType>*>(marg._obj);
                                                                                    }

                                                                                    /**
                                                                                     * Emplace with an object created from \a list and\a args as the contained object.
                                                                                     */
                                                                                    template <typename _ValueType, typename _Up, typename... _Args>
                                                                                    typename _any_constructible<Decay<_ValueType>&, Decay<_ValueType>,
                                                                                    std::initializer_list<_Up>, _Args&&...>::type
                                                                                    emplace(std::initializer_list<_Up> list, _Args&&... args)
                                                                                    {
                                                                                        _do_emplace<Decay<_ValueType>, _Up>(list, std::forward<_Args>(args)...);
                                                                                        any::ManageArg marg;
                                                                                        this->m_manager(any::MOAccess, this, &marg);
                                                                                        return *static_cast<Decay<_ValueType>*>(marg._obj);
                                                                                    }

                                                                                    //
                                                                                    //  modifiers
                                                                                    //

                                                                                    /**
                                                                                     * If not empty, destroy the contained object.
                                                                                     */
                                                                                    void reset() noexcept;

                                                                                    /**
                                                                                     * Exchange state with another object.
                                                                                     */
                                                                                    void swap(any& rhs) noexcept;

                                                                                    //
                                                                                    //  observers
                                                                                    //

                                                                                    /**
                                                                                     * bool has_value() const noexcept
                                                                                     */
                                                                                    bool has_value() const noexcept;

#if __cpp_rtti
                                                                                    /**
                                                                                     * The \c typeid of the contained object, or \c typeid(void) if empty.
                                                                                     */
                                                                                    const std::type_info& type() const noexcept;
#endif

                                                                                    template<typename _Tp>
                                                                                    static constexpr bool _is_valid_cast()
                                                                                    {
                                                                                        return detail::_or<std::is_reference<_Tp>, std::is_copy_constructible<_Tp>>::value;
                                                                                    }

        private:
            enum ManageOption {
                                                                                        MOAccess,
                                                                                        MOGetTypeInfo,
                                                                                        MOClone,
                                                                                        MODestroy,
                                                                                        MOXfer
                                                                                    };

                                                                                    union ManageArg
                                                                                            {
                                                                                        void* _obj;
                                                                                        const std::type_info* _typeinfo;
                                                                                        any* _any;
                                                                                            };

                                                                                    void(*m_manager)(ManageOption, const any*, ManageArg*);
                                                                                    Storage m_storage;

                                                                                    template<typename _Tp>
                                                                                    friend void* _any_caster(const any* __any);

                                                                                    // Manage in-place contained object.
                                                                                    template<typename _Tp>
                                                                                    struct Manager_internal
                                                                                            {
                                                                                        static void S_manage(ManageOption which, const any* anyp, ManageArg* arg);

                                                                                        template<typename _Up>
                                                                                        static void S_create(Storage& storage, _Up&& value)
                                                                                        {
                                                                                            void* addr = &storage.m_buffer;
                                                                                            ::new (addr) _Tp(std::forward<_Up>(value));
                                                                                        }

                                                                                        template<typename... _Args>
                                                                                        static void S_create(Storage& storage, _Args&&... args)
                                                                                        {
                                                                                            void* addr = &storage.m_buffer;
                                                                                            ::new (addr) _Tp(std::forward<_Args>(args)...);
                                                                                        }
                                                                                            };

                                                                                    // Manage external contained object.
                                                                                    template<typename _Tp>
                                                                                    struct Manager_external
                                                                                            {
                                                                                        static void S_manage(ManageOption __which, const any* __anyp, ManageArg* __arg);

                                                                                        template<typename _Up>
                                                                                        static void S_create(Storage& storage, _Up&& value)
                                                                                        {
                                                                                            storage.m_ptr = new _Tp(std::forward<_Up>(value));
                                                                                        }
                                                                                        template<typename... _Args>
                                                                                        static void S_create(Storage& storage, _Args&&... __args)
                                                                                        {
                                                                                            storage.m_ptr = new _Tp(std::forward<_Args>(__args)...);
                                                                                        }
                                                                                            };
        };

/**
 * Exchange the states of two \a any objects.
 */
inline void swap(any& x, any& y) noexcept
{
    x.swap(y);
}

/**
 * Create an any holding a \c _Tp constructed from \a args.
 */
template <typename _Tp, typename... _Args>
any make_any(_Args&&... args)
{
    constexpr detail::_in_place_type_t<_Tp> __in_place_type;
    return any(__in_place_type, std::forward<_Args>(args)...);
}

/**
 * Create an any holding a \c _Tp constructed from \a list and \a args.
 */
template <typename _Tp, typename _Up, typename... _Args>
any make_any(std::initializer_list<_Up> list, _Args&&... args)
{
    return any(detail::_in_place_type_t<_Tp>(), list, std::forward<_Args>(args)...);
}

/**
 * Access the contained object.
 *
 * \tparam _ValueType A const-reference or CopyConstructible type.
 * \param  a The object to access.
 * \return The contained object.
 * \throw  bad_any_cast If
 *         \code
 *             a.type() != typeid(remove_reference_t<_ValueType>)
 *         \endcode
 */
template<typename _ValueType>
inline _ValueType any_cast(const any& a)
{
    using _Up = typename std::remove_cv<_ValueType>::type;
    static_assert(any::_is_valid_cast<_ValueType>(),
            "Template argument must be a reference or CopyConstructible type");
    static_assert(std::is_constructible<_ValueType, const _Up&>::value,
            "Template argument must be constructible from a const value.");
    auto resultp = any_cast<_Up>(&a);
    if (resultp) {
        return static_cast<_ValueType>(*resultp);
    }
    throw_bad_any_cast();
}

/**
 * Access the contained object.
 *
 * \tparam _ValueType A const-reference or CopyConstructible type.
 * \param  a The object to access.
 * \return The contained object.
 * \throw  bad_any_cast If
 *         \code
 *             a.type() != typeid(remove_reference_t<_ValueType>)
 *         \endcode
 */
template<typename _ValueType>
inline _ValueType any_cast(any& a)
{
    using _Up = typename std::remove_cv<_ValueType>::type;
    static_assert(any::_is_valid_cast<_ValueType>(),
            "Template argument must be a reference or CopyConstructible type");
    static_assert(std::is_constructible<_ValueType, _Up&>::value,
            "Template argument must be constructible from an lvalue.");
    auto resultp = any_cast<_Up>(&a);
    if (resultp) {
        return static_cast<_ValueType>(*resultp);
    }
    throw_bad_any_cast();
}

template<typename _ValueType>
inline _ValueType any_cast(any&& a)
{
    using _Up = typename std::remove_cv<_ValueType>::type;
    static_assert(any::_is_valid_cast<_ValueType>(),
            "Template argument must be a reference or CopyConstructible type");
    static_assert(std::is_constructible<_ValueType, _Up>::value,
            "Template argument must be constructible from an rvalue.");
    auto resultp = any_cast<_Up>(&a);
    if (resultp) {
        return static_cast<_ValueType>(std::move(*resultp));
    }
    throw_bad_any_cast();
}

/// \cond undocumented
template<typename _Tp>
void* _any_caster(const any* a)
{
    // any_cast<T> returns non-null if __any->type() == typeid(T) and
    // typeid(T) ignores cv-qualifiers so remove them:
    using _Up = typename std::remove_cv<_Tp>::type;
    // The contained value has a decayed type, so if decay_t<U> is not U,
    // then it's not possible to have a contained value of type U:
    if (!std::is_same<typename std::decay<_Up>::type, _Up>::value) {
        return nullptr;
    }
    // Only copy constructible types can be used for contained values:
    else if (!std::is_copy_constructible<_Up>::value) {
        return nullptr;
    }
    // First try comparing function addresses, which works without RTTI
    else if (a->m_manager == &any::Manager<_Up>::S_manage
#if __cpp_rtti
|| a->type() == typeid(_Tp)
#endif
)
    {
        any::ManageArg marg;
        a->m_manager(any::MOAccess, a, &marg);
        return marg._obj;
    }
    return nullptr;
}
/// @endcond

/**
 * Access the contained object.
 *
 * \tparam _ValueType A const-reference or CopyConstructible type.
 * \param  a The object to access.
 * \return The contained object.
 * \throw  bad_any_cast If
 *         \code
 *             a != nullptr && a.type() == typeid(_ValueType)
 *         \endcode
 *
 * @{
 */
template<typename _ValueType>
inline const _ValueType* any_cast(const any* a) noexcept
{
    if (std::is_object<_ValueType>::value) {
        if (a) {
            return static_cast<_ValueType*>(_any_caster<_ValueType>(a));
        }
    }
    return nullptr;
}

template<typename _ValueType>
inline _ValueType* any_cast(any* __any) noexcept
{
    if (std::is_object<_ValueType>::value) {
        if (__any) {
            return static_cast<_ValueType*>(_any_caster<_ValueType>(__any));
        }
    }
    return nullptr;
}
/** @} */

template<typename _Tp>
void any::Manager_internal<_Tp>::S_manage(ManageOption which, const any* a, ManageArg* arg)
{
    // The contained object is in _M_storage._M_buffer
    auto ptr = reinterpret_cast<const _Tp*>(&a->m_storage.m_buffer);
    switch (which)
    {
        case MOAccess:
            arg->_obj = const_cast<_Tp*>(ptr);
            break;
            case MOGetTypeInfo:
#if __cpp_rtti
    arg->_typeinfo = &typeid(_Tp);
#endif
    break;
    case MOClone:
        ::new(&arg->_any->m_storage.m_buffer) _Tp(*ptr);
        arg->_any->m_manager = a->m_manager;
        break;
        case MODestroy:
            ptr->~_Tp();
            break;
            case MOXfer:
                ::new(&arg->_any->m_storage.m_buffer) _Tp
                (std::move(*const_cast<_Tp*>(ptr)));
                ptr->~_Tp();
                arg->_any->m_manager = a->m_manager;
                const_cast<any*>(a)->m_manager = nullptr;
                break;
    }
}

template<typename _Tp>
void any::Manager_external<_Tp>::S_manage(ManageOption which, const any* a, ManageArg* arg)
{
    // The contained object is *_M_storage._M_ptr
    auto ptr = static_cast<const _Tp*>(a->m_storage.m_ptr);
    switch (which)
    {
        case MOAccess:
            arg->_obj = const_cast<_Tp*>(ptr);
            break;
            case MOGetTypeInfo:
#if __cpp_rtti
    arg->_typeinfo = &typeid(_Tp);
#endif
    break;
    case MOClone:
        arg->_any->m_storage.m_ptr = new _Tp(*ptr);
        arg->_any->m_manager = a->m_manager;
        break;
        case MODestroy:
            delete ptr;
            break;
            case MOXfer:
                arg->_any->m_storage.m_ptr = a->m_storage.m_ptr;
                arg->_any->m_manager = a->m_manager;
                const_cast<any*>(a)->m_manager = nullptr;
                break;
    }
}
