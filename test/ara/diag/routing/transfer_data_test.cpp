#include <gtest/gtest.h>
#include "../../../../src/ara/diag/routing/transfer_data.h"

namespace ara
{
    namespace diag
    {
        namespace routing
        {
            class TransferDataTest : public testing::Test
            {
            private:
                static core::InstanceSpecifier mSpecifier;

            protected:
                TransferData Service;
                MetaInfo GeneralMetaInfo;

            public:
                TransferDataTest() : Service{mSpecifier},
                                     GeneralMetaInfo(Context::kDoIP)
                {
                }
            };

            core::InstanceSpecifier TransferDataTest::mSpecifier{"Instance0"};

            TEST_F(TransferDataTest, ValidTransferConfiguration)
            {
                const TransferDirection cTransferDirection{TransferDirection::kDownload};
                const size_t cMemoryAddress{512};
                const size_t cMemorySize{64};

                EXPECT_TRUE(
                    Service.TrySetTransferConfiguration(
                        cTransferDirection, cMemoryAddress, cMemorySize));
            }

            TEST_F(TransferDataTest, InvalidTransferConfiguration)
            {
                const TransferDirection cValidTransferDirection{TransferDirection::kDownload};
                const size_t cValidMemoryAddress{512};
                const size_t cValidMemorySize{64};

                const TransferDirection cInvalidTransferDirection{TransferDirection::kNone};
                const size_t cInvalidMemoryAddress{2048};
                const size_t cInvalidMemorySize{1024};
                const size_t cZeroMemorySize{0};

                EXPECT_FALSE(
                    Service.TrySetTransferConfiguration(
                        cInvalidTransferDirection, cValidMemoryAddress, cValidMemorySize));

                EXPECT_FALSE(
                    Service.TrySetTransferConfiguration(
                        cValidTransferDirection, cInvalidMemoryAddress, cValidMemorySize));
                
                EXPECT_FALSE(
                    Service.TrySetTransferConfiguration(
                        cValidTransferDirection, cValidMemoryAddress, cZeroMemorySize));

                EXPECT_FALSE(
                    Service.TrySetTransferConfiguration(
                        cValidTransferDirection, cValidMemoryAddress, cInvalidMemorySize));
            }

            TEST_F(TransferDataTest, TransferConfigurationReset)
            {
                const TransferDirection cTransferDirection{TransferDirection::kDownload};
                const size_t cMemoryAddress{512};
                const size_t cMemorySize{64};

                EXPECT_FALSE(Service.TryResetTransferConfiguration());

                Service.TrySetTransferConfiguration(
                        cTransferDirection, cMemoryAddress, cMemorySize);

                EXPECT_TRUE(Service.TryResetTransferConfiguration());
            }
        }
    }
}