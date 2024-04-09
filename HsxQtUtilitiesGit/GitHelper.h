#ifndef GITHELPER_H
#define GITHELPER_H

#include "GitTypes.h"
#include <QDateTime>
#include <QString>
#include <functional>
#include <OId.h>
#include <CommitInfo.h>
#include <GitTransferProgress.h>

class git_repository;


#include <HsxQtUtilitiesGit_global.h>

namespace hsx
{
    class HSXQTUTILITIESGIT_EXPORT  GitHelper
    {
    public:
        GitHelper();
        ~GitHelper();
        int clone(const QString &url, const QString &saveOfPath);
        int open(const QString &path);
        void test();
        void getRemotes(QStringList &ret);
        void getReferences(QStringList &ret, const char *remoteName = "origin");
        void getBranchs(QStringList &ret, GitTypes::BranchType btype);
        void getOids(QList<OId> &ret, GitTypes::SortType sort, const QString &refname);
        int getDifferenceOids(QList<OId> &ret, const QString &refname1, const QString &refname2);
        void getCommitInfo(const OId &oid, CommitInfo &ret);
        const QString toReferenceFriendlyName(const QString &fullname);
        int featch(const char *remoteName = "origin");
        int checkout(GitTypes::CheckoutStrategyType strategy, const QStringList &paths);
        int getDifferenceOids(QList<OId> &ret, QList<OId> &oids1, QList<OId> &oids2);
        int pull(GitTypes::CheckoutStrategyType checkout_strategy = GitTypes::GIT_CHECKOUT_FORCE, const char *remoteName = "origin", const char *branchName = "master");
        int merge(GitTypes::CheckoutStrategyType checkout_strategy = GitTypes::GIT_CHECKOUT_FORCE, const char *remoteName = "origin", const char *branchName = "master");
        std::function<void(const QString &path, size_t completed_steps, size_t total_steps)>checkoutProgressFunc = nullptr;
        std::function<void(const GitTransferProgress &stats)>transferProgressFunc = nullptr;
        std::function<void(const QString &str)>sidebandProgressFunc = nullptr;
        int commit(char *committer_name, char *committer_email);
        const QString LocalBranchPrefix = "refs/heads/";
        const QString RemoteTrackingBranchPrefix = "refs/remotes/";
        const QString TagPrefix = "refs/tags/";
        const QString NotePrefix = "refs/notes/";
    private:
        git_repository *g_repo = NULL;
    };
}
#endif // GITHELPER_H
