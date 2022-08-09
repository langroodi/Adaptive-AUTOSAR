#ifndef ECU_RESET_REQUEST_H
#define ECU_RESET_REQUEST_H

#include "./routing/routable_uds_service.h"

namespace ara
{
    namespace diag
    {
        /// @brief ECU reset request type
        enum class ResetRequestType : uint32_t
        {
            kSoftReset = 1,     ///!< ECU reset due to an application request
            kHardReset = 2,     ///!< ECU reset due to disconnecting and re-connecting the power supply
            kKeyOffOnReset = 3, ///!< ECU reset due to the ignition key off and on
            kCustomReset = 4    ///!< Manufacture specific reset type
        };

        /// @brief ECU reset request handling service
        class EcuResetRequest : public routing::RoutableUdsService
        {
        private:
            static const uint8_t cSid{0x11};

            std::promise<ResetRequestType> mResetTypePromise;
            std::future<ResetRequestType> mResetTypeFuture;
            bool mRapidShutdownEnabled;

            bool TryHandleSubFunction(
                uint8_t subFunction,
                const MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler,
                bool &powerDownTimeRequired);

        public:
            /// @brief Constructor
            /// @param specifier Instance specifier that owns the service
            explicit EcuResetRequest(const core::InstanceSpecifier &specifier);

            std::future<OperationOutput> HandleMessage(
                const std::vector<uint8_t> &requestData,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler) override;

            /// @brief Request the ECU to reset
            /// @param resetType ECU reset type
            /// @param id Type of the custom ECU reset ID (if applicable)
            /// @param metaInfo Request handling meta-info
            /// @param cancellationHandler Request cancellation token
            /// @returns Valid future without any exception
            /// @note The method is not compatible with the ARA standard.
            std::future<void> RequestReset(
                ResetRequestType resetType,
                core::Optional<uint8_t> id,
                const MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler);

            /// @brief Execute the ECU reset after the request handling
            /// @param metaInfo ECU reset execution meta-info
            /// @note The method is not compatible with the ARA standard.
            /// @throws std::logic_error Throws when the reset has not been requested
            /// @remarks The method will be called via DM after RequestReset is being triggered.
            void ExecuteReset(const MetaInfo &metaInfo);

            /// @brief Set enable state of the rapid shutdown (stand-by) mode
            /// @param enable Indicate whether or not the rapid shutdown should be enabled
            /// @param metaInfo Request handling meta-info
            /// @param cancellationHandler Request cancellation token
            /// @returns Valid future without any exception
            /// @note The method is not compatible with the ARA standard.
            std::future<void> EnableRapidShutdown(
                bool enable,
                const MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler);
        };
    }
}

#endif