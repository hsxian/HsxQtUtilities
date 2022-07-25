#ifndef OID_H
#define OID_H

class git_oid;

#include <qtutilities_global.h>

namespace hsx
{
    class QTUTILITIESSHARED_EXPORT  OId
    {
    public:
        OId(const git_oid *oid);
        const git_oid *getOid() const;
    private :
        const git_oid *m_oid = nullptr;
    };
}
#endif // OID_H
