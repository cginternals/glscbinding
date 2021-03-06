
#pragma once


#include <functional>

#include <glscbinding/glscbinding_api.h>


namespace glscbinding
{


/**
*  @brief
*    The generic pointer to a function
*/
using ProcAddress = void(*)();

/**
*  @brief
*    The signature for the getProcAddress function
*/
using GetProcAddress = std::function<ProcAddress(const char*)>;


} // namespace glscbinding