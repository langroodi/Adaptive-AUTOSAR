#ifndef ROUTABLE_USD_SERVICE_H
#define ROUTABLE_USD_SERVICE_H

#include <future>
#include <vector>
#include "../../core/instance_specifier.h"
#include "../../core/result.h"
#include "../meta_info.h"
#include "../cancellation_handler.h"

namespace ara
{
    namespace diag
    {
        /// @brief Positive response of a handled UDS request
        struct OperationOutput
        {
            /// @brief Response byte array
            std::vector<uint8_t> responseData;
        };

        /// @brief UDS services routing namespace
        /// @note The namespace is not part of the ARA standard
        namespace routing
        {
            /// @brief An UDS service to handle a diagnostic request by offering itself to a router
            class RoutableUdsService
            {
            private:
                const uint8_t mSid;
                bool mOffered;
                const ara::core::InstanceSpecifier &mSpecifier;

            protected:
                /// @brief SID increment for a positive response
                static const uint8_t cPositiveResponseSidIncrement{0x40};
                /// @brief Unsupported sub-function NRC
                const uint8_t cSubFunctionNotSupported{0x12};
                /// @brief Incorrect message length NRC
                const uint8_t cIncorrectMessageLength{0x13};
                /// @brief Incorrect request sequence NRC
                const uint8_t cRequestSequenceError{0x24};

                /// @brief Constructor
                /// @param specifier Owner instance specifier
                /// @param sid UDS service ID
                RoutableUdsService(
                    const ara::core::InstanceSpecifier &specifier,
                    uint8_t sid) noexcept;

                /// @brief Generate a negative response with a specific NRC
                /// @param[out] response Generated negative response
                /// @param nrc Given Negative Response Code (NRC)
                void GenerateNegativeResponse(
                    OperationOutput &response, uint8_t nrc) const;

                /// @brief Try to extract a value from the given meta-info
                /// @tparam T Extracted value integer type
                /// @param metaInfo Given meta-info for value extraction
                /// @param key Value key within the given meta-info
                /// @param[out] value Extracted value
                /// @returns True if the value is extracted successfully, otherwise false
                template <class T>
                bool TryExtractValue(MetaInfo &metaInfo, std::string key, T &value) const
                {
                    core::Optional<std::string> _optionalValue{metaInfo.GetValue(key)};

                    if (_optionalValue.HasValue())
                    {
                        std::string _valueStr{_optionalValue.Value()};
                        int _valueInt{std::stoi(_valueStr)};
                        value = static_cast<T>(_valueInt);

                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }

            public:
                /// @brief Negative response code service ID
                static const uint8_t cNegativeResponseCodeSid{0x7f};

                /// @brief Get SID
                /// @returns Service identification
                uint8_t GetSid() const noexcept;

                /// @brief Offer handling DM requests
                /// @returns Error result if the service has been already offered
                ara::core::Result<void> Offer();

                /// @brief Get offering status of the service
                /// @returns True if the service has been offered, otherwise false
                bool IsOffered() const noexcept;

                /// @brief Handle an UDS request message
                /// @param requestData Request message byte array
                /// @param metaInfo Request message metainfo
                /// @param cancellationHandler Callback to be invoked when the current conversation is cancelled
                /// @returns Response byte array
                virtual std::future<OperationOutput> HandleMessage(
                    const std::vector<uint8_t> &requestData,
                    MetaInfo &metaInfo,
                    CancellationHandler &&cancellationHandler) = 0;

                /// @brief Stop offering request handling
                void StopOffer() noexcept;

                virtual ~RoutableUdsService() noexcept = default;
            };
        }
    }
}

#endif