
#pragma once


#include <utility>
#include <functional>
#include <cassert>

#include <glscbinding/Value.h>
#include <glscbinding/FunctionCall.h>
#include <glscbinding/CallbackMask.h>
#include <glscbinding/Boolean8.h>


namespace glscbinding
{


template <typename ReturnType, typename... Arguments>
struct BasicCallHelper
{
    inline static ReturnType call(const glscbinding::Function<ReturnType, Arguments...> * function, Arguments&&... arguments)
    {
        return reinterpret_cast<typename glscbinding::Function<ReturnType, Arguments...>::Signature>(function->address())(std::forward<Arguments>(arguments)...);
    }
};


// Special case for booleans because of MSVC differing behavior

template <typename... Arguments>
struct BasicCallHelper<glscbinding::Boolean8, Arguments...>
{
    inline static glscbinding::Boolean8 call(const glscbinding::Function<glscbinding::Boolean8, Arguments...> * function, Arguments&&... arguments)
    {
        return reinterpret_cast<typename glscbinding::Function<glscbinding::Boolean8::underlying_type, Arguments...>::Signature>(function->address())(std::forward<Arguments>(arguments)...);
    }
};


template <typename ReturnType, typename... Arguments>
struct FunctionHelper
{
    inline static ReturnType call(const glscbinding::Function<ReturnType, Arguments...> * function, Arguments&&... arguments)
    {
        glscbinding::FunctionCall functionCall(function);

        if (function->isAnyEnabled(glscbinding::CallbackMask::Parameters))
        {
            functionCall.parameters = glscbinding::createValues(std::forward<Arguments>(arguments)...);
        }

        if (function->isEnabled(glscbinding::CallbackMask::Before))
        {
            AbstractFunction::before(functionCall);

            if (function->beforeCallback())
            {
                function->beforeCallback()(std::forward<Arguments>(arguments)...);
            }
        }

        auto value = BasicCallHelper<ReturnType, Arguments ...>::call(function, std::forward<Arguments>(arguments)...);

        if (function->isAnyEnabled(glscbinding::CallbackMask::ReturnValue))
        {
            functionCall.returnValue = glscbinding::createValue(value);
        }

        if (function->isEnabled(glscbinding::CallbackMask::After))
        {
            AbstractFunction::after(functionCall);

            if (function->afterCallback())
            {
                function->afterCallback()(value, std::forward<Arguments>(arguments)...);
            }
        }

        if (function->isEnabled(glscbinding::CallbackMask::Logging))
        {
            AbstractFunction::log(std::move(functionCall));
        }

        return value;
    }
};


template <typename... Arguments>
struct FunctionHelper<void, Arguments...>
{
    inline static void call(const glscbinding::Function<void, Arguments...> * function, Arguments&&... arguments)
    {
        glscbinding::FunctionCall functionCall(function);

        if (function->isAnyEnabled(glscbinding::CallbackMask::Parameters))
        {
            functionCall.parameters = glscbinding::createValues(std::forward<Arguments>(arguments)...);
        }

        if (function->isEnabled(glscbinding::CallbackMask::Before))
        {
            AbstractFunction::before(functionCall);

            if (function->beforeCallback())
            {
                function->beforeCallback()(std::forward<Arguments>(arguments)...);
            }
        }

        BasicCallHelper<void, Arguments ...>::call(function, std::forward<Arguments>(arguments)...);

        if (function->isEnabled(glscbinding::CallbackMask::After))
        {
            AbstractFunction::after(functionCall);

            if (function->afterCallback())
            {
                function->afterCallback()(std::forward<Arguments>(arguments)...);
            }
        }

        if (function->isEnabled(glscbinding::CallbackMask::Logging))
        {
            AbstractFunction::log(std::move(functionCall));
        }
    }
};


template <typename ReturnType, typename... Arguments>
Function<ReturnType, Arguments...>::Function(const char * _name)
: AbstractFunction{_name}
, m_beforeCallback{nullptr}
, m_afterCallback{nullptr}
{
}

template <typename ReturnType, typename... Arguments>
ReturnType Function<ReturnType, Arguments...>::operator()(Arguments&... arguments) const
{
    return call(arguments...);
}

template <typename ReturnType, typename... Arguments>
ReturnType Function<ReturnType, Arguments...>::call(Arguments&... arguments) const
{
    const auto myAddress = address();

    if (myAddress == nullptr)
    {
        if (isEnabled(CallbackMask::Unresolved))
        {
           AbstractFunction::unresolved(this);
        }

        return ReturnType();
    }

    if (isAnyEnabled(CallbackMask::Before | CallbackMask::After | CallbackMask::Logging))
    {
        return FunctionHelper<ReturnType, Arguments...>::call(this, std::forward<Arguments>(arguments)...);
    }
    else
    {
        return BasicCallHelper<ReturnType, Arguments...>::call(this, std::forward<Arguments>(arguments)...);
    }
}

template <typename ReturnType, typename... Arguments>
ReturnType Function<ReturnType, Arguments...>::directCall(Arguments... arguments) const
{
    if (address() == nullptr)
    {
        return ReturnType();
    }

    return BasicCallHelper<ReturnType, Arguments...>::call(this, std::forward<Arguments>(arguments)...);
}

template <typename ReturnType, typename... Arguments>
void Function<ReturnType, Arguments...>::setBeforeCallback(BeforeCallback callback)
{
    m_beforeCallback = std::move(callback);
}

template <typename ReturnType, typename... Arguments>
void Function<ReturnType, Arguments...>::clearBeforeCallback()
{
    m_beforeCallback = nullptr;
}

template <typename ReturnType, typename... Arguments>
void Function<ReturnType, Arguments...>::setAfterCallback(AfterCallback callback)
{
    m_afterCallback = std::move(callback);
}

template <typename ReturnType, typename... Arguments>
void Function<ReturnType, Arguments...>::clearAfterCallback()
{
    m_afterCallback = nullptr;
}

template <typename ReturnType, typename... Arguments>
typename Function<ReturnType, Arguments...>::BeforeCallback Function<ReturnType, Arguments...>::beforeCallback() const
{
    return m_beforeCallback;
}

template <typename ReturnType, typename... Arguments>
typename Function<ReturnType, Arguments...>::AfterCallback Function<ReturnType, Arguments...>::afterCallback() const
{
    return m_afterCallback;
}

template <typename ReturnType, typename... Arguments>
bool Function<ReturnType, Arguments...>::hasState() const
{
    return hasState(AbstractFunction::currentPos());
}

template <typename ReturnType, typename... Arguments>
bool Function<ReturnType, Arguments...>::hasState(const int pos) const
{
    return pos > -1 && AbstractFunction::maxPos() <= pos;
}

template <typename ReturnType, typename... Arguments>
AbstractState & Function<ReturnType, Arguments...>::state() const
{
    return state(AbstractFunction::currentPos());
}

template <typename ReturnType, typename... Arguments>
AbstractState & Function<ReturnType, Arguments...>::state(const int pos) const
{
    assert(AbstractFunction::maxPos() >= pos);
    assert(pos > -1);

    return m_states.at(pos);
}

template <typename ReturnType, typename... Arguments>
void Function<ReturnType, Arguments...>::resizeStates(int count)
{
    m_states.resize(static_cast<std::size_t>(count));
}


} // namespace glscbinding
