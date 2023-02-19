#include <stdexcept>
#include "./curl_wrapper.h"

namespace application
{
    namespace helper
    {
        CurlWrapper::CurlWrapper(std::string vccApiKey, std::string bearerToken) : mCurlHandler{curl_easy_init()},
                                                                                   mHeaders{NULL},
                                                                                   mReceivedBuffer{nullptr}
        {
            const std::string cJsonAcceptHeader{"accept: application/json"};
            const std::string cApiKeyHeader{"vcc-api-key: " + vccApiKey};

            if (mCurlHandler)
            {
                // Set HTTP request type to GET
                curl_easy_setopt(mCurlHandler, CURLOPT_HTTPGET, 1L);

                // Apply request headers
                mHeaders = curl_slist_append(mHeaders, cJsonAcceptHeader.c_str());
                mHeaders = curl_slist_append(mHeaders, cApiKeyHeader.c_str());
                curl_easy_setopt(mCurlHandler, CURLOPT_HTTPHEADER, mHeaders);

                // Configure the bearer token
                curl_easy_setopt(mCurlHandler, CURLOPT_XOAUTH2_BEARER, bearerToken.c_str());
                curl_easy_setopt(mCurlHandler, CURLOPT_HTTPAUTH, CURLAUTH_BEARER);

                // Configure the callback (https://everything.curl.dev/libcurl/cplusplus)
                curl_easy_setopt(mCurlHandler, CURLOPT_WRITEFUNCTION, CurlWrapper::callback);
                curl_easy_setopt(mCurlHandler, CURLOPT_WRITEDATA, this);
            }
            else
            {
                throw std::runtime_error("CURL easy initialization failed!");
            }
        }

        size_t CurlWrapper::callback(void *buffer, size_t sz, size_t n, void *f)
        {
            auto _curlWrapper{static_cast<CurlWrapper *>(f)};
            size_t _result{sz * n};

            // Check whether the callback buffer has been set or not
            if (_curlWrapper->mReceivedBuffer)
            {
                auto bufferStr{static_cast<char *>(buffer)};
                _curlWrapper->mReceivedBuffer->assign(
                    bufferStr, bufferStr + _result);
            }

            return _result;
        }

        bool CurlWrapper::TryExecute(std::string url, std::string *response)
        {
            // Set the callback buffer
            mReceivedBuffer = response;
            curl_easy_setopt(mCurlHandler, CURLOPT_URL, url.c_str());
            CURLcode _curlCode{curl_easy_perform(mCurlHandler)};
            bool _result{_curlCode == CURLE_OK};
            // Reset the callback buffer
            mReceivedBuffer = nullptr;

            return _result;
        }

        CurlWrapper::~CurlWrapper()
        {
            curl_slist_free_all(mHeaders);
            curl_easy_cleanup(mCurlHandler);
        }
    }
}