/*========================== begin_copyright_notice ============================

Copyright © 2020-2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_function_log_common.h

@brief Common type for function logging.
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief A structure that logs function entered/exiting.
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename Result = uint32_t>
    struct FunctionLogCommonTrait
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( FunctionLogCommonTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( FunctionLogCommonTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "FunctionLogCommonTrait<Traits>";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        std::string m_Name;
        Result      m_Result;

        //////////////////////////////////////////////////////////////////////////
        /// @brief FunctionLogCommonTrait constructor.
        /// @param name     function name.
        /// @param result   function initial result value.
        //////////////////////////////////////////////////////////////////////////
        FunctionLogCommonTrait(
            const char*  name,
            const Result result )
            : m_Result( result )
        {
            const bool setFunctionName = T::Tools::CheckLogLayer();

            if( setFunctionName )
            {
                SetFunctionName( name );
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief FunctionLogCommonTrait destructor.
        //////////////////////////////////////////////////////////////////////////
        ~FunctionLogCommonTrait()
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Strips string of information that is not useful from
        ///         logging perspective (e.g. information has been already logged etc.).
        ///         Right now, namespace and template are both removed.
        /// @param  name    full function name (full path, namespace etc.).
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void SetFunctionName( const char* name )
        {
            // Store a function name.
            m_Name = name;

            // If requested, simplify it.
            if( !T::ConstantsOs::Log::m_SimplifyFunctionName )
            {
                return;
            }

            auto& functionNamesMap = T::Debug::GetFunctionNamesMap();

            auto foundFunctionName = functionNamesMap.find( m_Name );
            if( foundFunctionName != functionNamesMap.end() )
            {
                m_Name = foundFunctionName->second;
                return;
            }

            std::string erasedString( m_Name );

            // Remove "ML::" namespace from function name.
            const auto& namespaceBegin = erasedString.find_first_of( "M" );
            if( namespaceBegin != std::string::npos )
            {
                const auto& namespaceEnd = erasedString.find_first_of( ":" ) + 2 - namespaceBegin;
                erasedString.erase( namespaceBegin, namespaceEnd );
            }

            // Remove template specialization from function name.
            const auto& bracketsBegin = erasedString.find_first_of( "<" );
            if( bracketsBegin != std::string::npos )
            {
                const auto& bracketsEnd = erasedString.find_first_of( ">" ) + 1 - bracketsBegin;
                erasedString.erase( bracketsBegin, bracketsEnd );
            }

            // Remove namespaces from function name.
            RemoveNamespaces( Constants::Log::m_SimplifyFunctionNameLevel, erasedString );

            // Cache result.
            functionNamesMap.insert( { m_Name, erasedString } );

            // Store a simplified function name.
            m_Name = erasedString;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Simplify function name by removing namespace or struct information.
        /// @param  level           level of simplifying string. By increasing the level
        ///                         the function name is more complex.
        /// @return functionName    a string with simplified function name.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static void RemoveNamespaces( const uint32_t level, std::string& functionName )
        {
            if( level == 0 )
            {
                return;
            }

            std::string& simplifiedString = functionName;

            const std::string namespaceOperator     = "::";
            const size_t      namespaceOperatorSize = namespaceOperator.size();
            const size_t      namespaceBegin        = 0;
            size_t            namespaceEnd          = simplifiedString.size();

            for( uint32_t i = 0; i < level; ++i )
            {
                namespaceEnd = simplifiedString.find_last_of( namespaceOperator, namespaceEnd );

                if( namespaceEnd == std::string::npos )
                {
                    return;
                }

                namespaceEnd -= namespaceOperatorSize;

                if( namespaceEnd == std::string::npos )
                {
                    break;
                }
            }

            simplifiedString.erase( namespaceBegin, namespaceEnd + namespaceOperatorSize + 1 );
        }
    };
} // namespace ML
