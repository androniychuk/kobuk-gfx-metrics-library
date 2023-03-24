/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2023 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_override_posh_query.h

@brief Enables posh query override.
*/

#pragma once

namespace ML::BASE
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for OverridePoshQueryTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct OverridePoshQueryTrait : DdiObject<T, TT::Overrides::PoshQuery, OverrideHandle_1_0, ObjectType::OverridePoshQuery>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( OverridePoshQueryTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( OverridePoshQueryTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base = DdiObject<T, TT::Overrides::PoshQuery, OverrideHandle_1_0, ObjectType::OverridePoshQuery>;
        using Base::IsValid;
        using Base::FromHandle;
        using Base::Allocate;
        using Base::Delete;
        using Base::Derived;
        using Base::m_Context;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Override posh query constructor.
        /// @param context library context.
        //////////////////////////////////////////////////////////////////////////
        OverridePoshQueryTrait( TT::Context& context )
            : Base( context )
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "OverridePoshQueryTrait<Traits> (Windows)";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Creates posh query override.
        /// @param  context library context.
        /// @return handle  override handle.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode Create(
            TT::Context&        context,
            OverrideHandle_1_0& handle )
        {
            auto override = Allocate( context );

            if( override )
            {
                handle = override->GetHandle();
            }

            return override
                ? StatusCode::Success
                : StatusCode::Failed;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns state of user overrides.
        /// @return getData data requested by client.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode GetData( GetReportOverride_1_0& getData )
        {
            ML_FUNCTION_CHECK_STATIC( getData.Data != nullptr );
            ML_FUNCTION_CHECK_STATIC( getData.DataSize == sizeof( TT::Layouts::Override::PoshQuery ) );
            ML_FUNCTION_CHECK_STATIC( IsValid( getData.Handle ) );

            auto& override = FromHandle( getData.Handle );
            ML_FUNCTION_LOG( StatusCode::Success, &override.m_Context );

            auto& report     = *static_cast<TT::Layouts::Override::PoshQuery*>( getData.Data );
            report.m_Enabled = false;

            return log.m_Result;
        }
    };
} // namespace ML::BASE

namespace ML::GEN9
{
    template <typename T>
    struct OverridePoshQueryTrait : BASE::OverridePoshQueryTrait<T>
    {
        ML_DECLARE_TRAIT( OverridePoshQueryTrait, BASE );
    };
} // namespace ML::GEN9

namespace ML::GEN11
{
    template <typename T>
    struct OverridePoshQueryTrait : GEN9::OverridePoshQueryTrait<T>
    {
        ML_DECLARE_TRAIT( OverridePoshQueryTrait, GEN9 );
    };
} // namespace ML::GEN11

namespace ML::XE_LP
{
    template <typename T>
    struct OverridePoshQueryTrait : GEN11::OverridePoshQueryTrait<T>
    {
        ML_DECLARE_TRAIT( OverridePoshQueryTrait, GEN11 );
    };
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    template <typename T>
    struct OverridePoshQueryTrait : XE_LP::OverridePoshQueryTrait<T>
    {
        ML_DECLARE_TRAIT( OverridePoshQueryTrait, XE_LP );
    };
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    template <typename T>
    struct OverridePoshQueryTrait : XE_HP::OverridePoshQueryTrait<T>
    {
        ML_DECLARE_TRAIT( OverridePoshQueryTrait, XE_HP );
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct OverridePoshQueryTrait : XE_HPG::OverridePoshQueryTrait<T>
    {
        ML_DECLARE_TRAIT( OverridePoshQueryTrait, XE_HPG );
    };
} // namespace ML::XE_HPC
