#include "OId.h"

#include <git2.h>
using namespace hsx;
OId::OId(const git_oid *oid)
{
    m_oid = oid;
}

const git_oid *OId::getOid() const
{
    return m_oid;
}
