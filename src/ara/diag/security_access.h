#ifndef SECURITY_ACCESS_H
#define SECURITY_ACCESS_H

#include "./reentrancy.h"
#include "./routing/delay_timer.h"
#include "./routing/routable_uds_service.h"

namespace ara
{
    namespace diag
    {
        /// @brief A data model that contains the status of a security level
        /// @note The structure is not ARA compatible.
        struct SecurityLevel
        {
            /// @brief Indicate whether the security level has been unlocked or not
            bool Unlocked;
            /// @brief Seed that corresponds to the security level for key comparison
            uint16_t Seed;

            /// @brief Constructor
            /// @param seed Initial seed
            explicit SecurityLevel(uint16_t seed = 0) noexcept : Unlocked{false}, Seed{seed}
            {
            }
        };

        /// @brief Security key comparison result
        enum class KeyCompareResultType
        {
            kKeyValid = 0x00,  ///< Valid security key
            kKeyInvalid = 0x01 ///< Invalid security key
        };

        /// @brief A class to evaluate the security access to the data among diagnostic services
        /// @note In contrast with the ARA standard, the class is NOT abstract.
        class SecurityAccess : public routing::RoutableUdsService
        {
        private:
            static const uint8_t cSid{0x27};
            static const uint16_t cInitialSeed{1};

            const size_t cSidIndex{0};
            const size_t cSubFunctionIndex{1};
            const size_t cDataRecordOffset{2};
            const size_t cKeyLength{2};

            const uint8_t cSuppressPosRspMask{0x80};
            const uint8_t cGeneralReject{0x10};
            const uint8_t cInvalidKey{0x35};
            const uint8_t cExceededNumberOfAttempts{0x36};

            const uint8_t cIsoReservedSubFunction{0x00};
            const uint8_t cIsoReservedLBound{0x43};
            const uint8_t cIsoReservedHBound{0x5e};
            const uint8_t cSupplierReservedLBound{0x61};
            const uint8_t cSupplierReservedHBound{0x7f};

            const std::string cEncryptorKey{"Encryptor"};
            const std::string cAttemptThresholdKey{"AttemptThreshold"};
            const std::string cExceededAttemptDelayKey{"ExceededAttemptDelay"};

            const ReentrancyType mReentrancy;
            uint16_t mSeed;
            std::map<uint8_t, SecurityLevel> mSecurityLevels;
            routing::DelayTimer mDelayTimer;
            uint8_t mFailedUnlockAttempt;

            bool hasProblem(
                const std::vector<uint8_t> &requestData, uint8_t &nrc) const;

            bool tryFetchSeed(uint8_t level, uint16_t &seed) const;
            uint16_t addLevel(uint8_t level);
            void handleRequestSeed(
                OperationOutput &response,
                uint8_t subFunction,
                const std::vector<uint8_t> &securityAccessDataRecord,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler,
                bool suppressPositiveResponse);
            
            void handlePassedAttempt(
                std::map<uint8_t, SecurityLevel>::iterator securityLevelItr);
            void handleFailedAttempt(MetaInfo &metaInfo);
            void handleSendKey(
                OperationOutput &response,
                uint8_t subFunction,
                const std::vector<uint8_t> &key,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler,
                bool suppressPositiveResponse);

        public:
            /// @brief Constructor
            /// @param specifier Owner instance specifier
            /// @param reentrancyType Service reentrancy type
            explicit SecurityAccess(
                const core::InstanceSpecifier &specifier,
                ReentrancyType reentrancyType) noexcept;

            ~SecurityAccess() noexcept = default;

            std::future<OperationOutput> HandleMessage(
                const std::vector<uint8_t> &requestData,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler) override;

            /// @brief Request a seed from the client side to provide a security key
            /// @param subFunction Security level sub-function
            /// @param securityAccessDataRecord Seed request parameters payload
            /// @param metaInfo Request message metainfo
            /// @param cancellationHandler Callback to be invoked when the current conversation is cancelled
            /// @returns Provided key by the server
            std::future<std::vector<uint8_t>> GetSeed(
                uint8_t subFunction,
                std::vector<uint8_t> securityAccessDataRecord,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler);

            /// @brief Evaluate the key provided by a client
            /// @param subFunction Security level sub-function
            /// @param key Provided key
            /// @param metaInfo Request message metainfo
            /// @param cancellationHandler Callback to be invoked when the current conversation is cancelled
            /// @returns Key evaluation result from the server
            std::future<KeyCompareResultType> CompareKey(
                uint8_t subFunction,
                std::vector<uint8_t> key,
                MetaInfo &metaInfo,
                CancellationHandler &&cancellationHandler);
        };
    }
}
#endif