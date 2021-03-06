
#pragma once


#include <string>
#include <set>
#include <utility>

#include <glscbinding/glscbinding_api.h>
#include <glscbinding/glscbinding_features.h>


namespace glscbinding
{


/**
*  @brief
*    The Version class represents an OpenGL feature, consisting of majow version and minor version, excluding the profile information.
*
*    This instance can represent both any officially released OpenGL feature and other combinations of major and minar version and provides methods for validity checking and comparison
*
*  Example code:
*  \code{.cpp}
*  const glscbinding::Version currentVersion = glscbinding::aux::ContextInfo::version();
*
*  if (currentVersion >= glscbinding::Version(3, 2))
*  {
*      // do something
*  }
*  \endcode
*/
class AbstractVersion
{
public:
    /**
    *  @brief
    *    Default constructor, resulting in an invalid Version object
    */
    GLSCBINDING_CONSTEXPR inline AbstractVersion();

    /**
    *  @brief
    *    Constructor for a Version object with the given major and minor version
    *
    *  @param[in] majorVersion
    *    The major version
    *  @param[in] minorVersion
    *    The minor version
    */
    GLSCBINDING_CONSTEXPR inline AbstractVersion(unsigned char majorVersion, unsigned char minorVersion);

    /**
    *  @brief
    *    Copy constructor
    *
    *  @param[in] version
    *    The Version the data is used from
    */
    GLSCBINDING_CONSTEXPR inline AbstractVersion(const AbstractVersion & version) = delete;

    /**
    *  @brief
    *    Move constructor
    *
    *  @param[in] version
    *    The Version the data is moved from
    */
    inline AbstractVersion(AbstractVersion && version) = delete;

    /**
    *  @brief
    *    Accessor for the major version
    *
    *  @return
    *    the major version
    */
    GLSCBINDING_CONSTEXPR inline unsigned char majorVersion() const;

    /**
    *  @brief
    *    Accessor for the minor version
    *
    *  @return
    *    the minor version
    */
    GLSCBINDING_CONSTEXPR inline unsigned char minorVersion() const;

    /**
    *  @brief
    *    Cast operator for a std::pair cast of type unsigned char
    */
    inline operator std::pair<unsigned char, unsigned char>() const;

    /**
    *  @brief
    *    Cast operator for a std::pair cast of type unsigned short
    */
    inline operator std::pair<unsigned short, unsigned short>() const;

    /**
    *  @brief
    *    Cast operator for a std::pair cast of type unsigned int
    */
    inline operator std::pair<unsigned int, unsigned int>() const;

    /**
    *  @brief
    *    Create a string representing the Version using the scheme "<majorVersion>.<minorVersion>"
    *
    *  @return
    *    The version as string, "-.-" iff the string is invalid
    */
    GLSCBINDING_API inline std::string toString() const;

    /**
    *  @brief
    *    Check if the Version was constructed using the default constructor
    *
    *  @return
    *    'true' if the major version is 0, else 'false'
    *
    *  @remarks
    *    This method can be used to check if this Version was constructed using the default constructor or is otherwise malformed
    */
    GLSCBINDING_CONSTEXPR inline bool isNull() const;


protected:
    unsigned char m_major; ///< The major version
    unsigned char m_minor; ///< The minor version
};


} // namespace glscbinding


#include <glscbinding/AbstractVersion.inl>
