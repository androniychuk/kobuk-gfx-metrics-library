/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2022 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_configuration_hw_counters_oa.h

@brief Creates, activates and deactivates oa configuration.
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for ConfigurationHwCountersOaTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct ConfigurationHwCountersOaTrait : DdiObject<T, TT::Configurations::HwCountersOa, ConfigurationHandle_1_0, ObjectType::ConfigurationHwCountersOa>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( ConfigurationHwCountersOaTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( ConfigurationHwCountersOaTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base = DdiObject<T, TT::Configurations::HwCountersOa, ConfigurationHandle_1_0, ObjectType::ConfigurationHwCountersOa>;
        using Base::Allocate;
        using Base::Delete;
        using Base::m_Context;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        TT::KernelInterface&                                       m_Kernel;
        TT::Layouts::Configuration::PerformanceMonitoringRegisters m_OaRegisters;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Configuration hw counters oa constructor.
        /// @param context  library context.
        //////////////////////////////////////////////////////////////////////////
        ConfigurationHwCountersOaTrait( TT::Context& context )
            : Base( context )
            , m_Kernel{ context.m_Kernel }
            , m_OaRegisters{}
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "ConfigurationHwCountersOaTrait<Traits> (Linux)";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Creates oa configuration.
        /// @param  context library context.
        /// @return handle  configuration handle.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode Create(
            TT::Context&             context,
            ConfigurationHandle_1_0& handle )
        {
            auto       configuration = Allocate( context );
            const bool valid         = configuration && ML_SUCCESS( configuration->Initialize() );

            if( valid )
            {
                handle = configuration->GetHandle();
            }
            else
            {
                Delete( configuration );
            }

            return valid
                ? StatusCode::Success
                : StatusCode::Failed;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Activates oa configuration.
        /// @param  activateData  oa counter activate data.
        /// @return               operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Activate( const ConfigurationActivateData_1_0& activateData ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );
            ML_FUNCTION_CHECK( activateData.Type == GpuConfigurationActivationType::Tbs );

            auto&      oaBuffer   = m_Kernel.m_Tbs.GetOaBufferMapped( T::Layouts::OaBuffer::Type::Oa );
            const bool restartTbs = T::Policy::ConfigurationOa::Activate::m_RestartTbs;

            m_Kernel.m_OaConfigurationReferenceCounter++;

            return log.m_Result = ( restartTbs && !oaBuffer.m_Mapped )
                ? m_Kernel.m_Tbs.m_Stream.Restart()
                : m_Kernel.LoadOaConfigurationToGpu( m_OaRegisters );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Deactivates oa configuration.
        ///         If oa metric set is in use, reference counter will be decreased.
        ///         if reference counter will become zero, another metric set will be
        ///         allowed to be used.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Deactivate() const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            m_Kernel.m_OaConfigurationReferenceCounter--;

            return log.m_Result = T::Policy::ConfigurationOa::Activate::m_RestartTbs
                ? StatusCode::Success
                : m_Kernel.UnloadOaConfigurationFromGpu( m_OaRegisters );
        }

    private:
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Initializes activated oa configuration.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Initialize()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );
            return log.m_Result = m_Kernel.GetOaConfiguration( m_OaRegisters );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Calls flush command buffer callback.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode FlushCommandBuffer() const
        {
            const bool  validFlushCallback = m_Context.m_ClientCallbacks.CommandBufferFlush != nullptr;
            const bool  validFlushPolicy   = T::Policy::QueryHwCounters::Begin::FlushCommandBuffer;
            const auto& callback           = m_Context.m_ClientCallbacks.CommandBufferFlush;

            return validFlushCallback && validFlushPolicy
                ? callback( m_Context.m_ClientHandle )
                : StatusCode::Success;
        }
    };
} // namespace ML
