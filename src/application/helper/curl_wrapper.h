#ifndef CURL_WRAPPER_H
#define CURL_WRAPPER_H

#include <curl/curl.h>
#include <string>

namespace application
{
    namespace helper
    {
        /// @brief Minimalist C++ wrapper of easy (blocking interface) CURL library for RESTful communication
        class CurlWrapper
        {
        private:
            static size_t callback(void *buffer, size_t sz, size_t n, void *f);

            CURL *mCurlHandler;
            struct curl_slist *mHeaders;
            std::string *mReceivedBuffer;

        public:
            /// @brief Constructor
            /// @param vccApiKey VCC API key
            /// @param bearerToken OAuth 2.0 bearer token
            /// @throws std::runtime_error Throws when the CURL handler creation failed.
            CurlWrapper(std::string vccApiKey, std::string bearerToken);

            CurlWrapper() = delete;
            ~CurlWrapper();

            /// @brief Try to execute a RESTful GET request
            /// @param[in] url REST API request URL
            /// @param[out] response REST API server response in JSON format excluding the header
            /// @return True if the request is executed successfully (even with not 200 HTTP code); otherwise false
            /// @note In case of getting 'false' as the return value, the 'response' content should be discared.
            /// @remark The function call is blocking.
            bool TryExecute(std::string url, std::string *response);
        };
    }
}

#endif