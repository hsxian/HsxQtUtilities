#include "GitTransferProgress.h"
#include <git2.h>
using namespace hsx;
GitTransferProgress::GitTransferProgress()
{

}

GitTransferProgress::GitTransferProgress(const git_indexer_progress2 &status)
{
    indexed_deltas = status.indexed_deltas;
    indexed_objects = status.indexed_objects;
    local_objects = status.local_objects;
    received_objects = status.received_objects;
    received_bytes = status.received_bytes;
    local_objects = status.local_objects;
    total_objects = status.total_objects;
}
