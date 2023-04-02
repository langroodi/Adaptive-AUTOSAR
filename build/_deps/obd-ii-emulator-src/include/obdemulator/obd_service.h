#ifndef OBD_SERVICE_H
#define OBD_SERVICE_H

#include <functional>
#include <stdint.h>
#include <vector>

namespace ObdEmulator
{
    /// @brief A class that defines an OBD-II service based on SAE J1979 standard
    class ObdService
    {
    private:
        const uint8_t mService;

    protected:
        /// @brief Constructor
        /// @param service OBD-II service (mode)
        ObdService(uint8_t service) noexcept;

    public:
        virtual ~ObdService() noexcept = default;

        /// @brief Get the service
        /// @returns OBD service number
        uint8_t GetService() const noexcept;

        /// @brief Try to get an emulated response based the queried PID
        /// @param[in] pid Queried PID
        /// @param[out] response Response data byte array
        /// @returns True, if the service handled the query successfully, otherwise false
        /// @note The function may be called from a different thread than the main thread.
        virtual bool TryGetResponse(
            const std::vector<uint8_t> &pid,
            std::vector<uint8_t> &response) const = 0;

        /// @brief Try to get an emulated response asynchronously based the queried PID
        /// @param[in] pid Queried PID
        /// @param[out] callback Callback to be invoked when the response is ready
        /// @returns True, if the query is queued successfully for handling, otherwise false
        /// @note The function may be called from a different thread than the main thread.
        virtual bool TryGetResponseAsync(
            const std::vector<uint8_t> &pid,
            std::function<void(std::vector<uint8_t> &&)> &&callback) = 0;
    };
}

#endif