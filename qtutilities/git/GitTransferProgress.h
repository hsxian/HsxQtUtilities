#ifndef GITTRANSFERPROGRESS_H
#define GITTRANSFERPROGRESS_H

#include <cstddef>
typedef  struct git_indexer_progress git_indexer_progress2;
#include <qtutilities_global.h>

namespace hsx
{
    class QTUTILITIESSHARED_EXPORT  GitTransferProgress
    {
    public:
        GitTransferProgress();
        GitTransferProgress(const git_indexer_progress2 &status);
        /** number of objects in the packfile being indexed */
        unsigned int total_objects;

        /** received objects that have been hashed */
        unsigned int indexed_objects;

        /** received_objects: objects which have been downloaded */
        unsigned int received_objects;

        /**
         * locally-available objects that have been injected in order
         * to fix a thin pack
         */
        unsigned int local_objects;

        /** number of deltas in the packfile being indexed */
        unsigned int total_deltas;

        /** received deltas that have been indexed */
        unsigned int indexed_deltas;

        /** size of the packfile received up to now */
        size_t received_bytes;
    };
}
#endif // GITTRANSFERPROGRESS_H
