
#ifndef DUMMY_SUPERVISION_H
#define DUMMY_SUPERVISION_H

#include "../../../../src/ara/phm/supervisors/elementary_supervision.h"

namespace ara
{
    namespace phm
    {
        namespace supervisors
        {
            class DummySupervision : public ElementarySupervision
            {
            public:
                DummySupervision() noexcept : ElementarySupervision(TypeOfSupervision::LogicalSupervision)
                {
                }

                inline void ReportStatus(SupervisionStatus status)
                {
                    Report(status);
                }
            };
        }
    }
}

#endif