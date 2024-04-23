#include "GitHelper.h"
#include <QDateTime>
#include <QDebug>
#include <git2.h>
#include <git2/errors.h>
#include <git2/merge.h>

using namespace hsx;

#define qdebug_git_fail(expr) do { \
        if((expr) != 0){ \
            const git_error *last = git_error_last(); \
            if(last) \
            { \
                qDebug()<<last->message<<"line:"<<__LINE__;\
            } \
        } \
    } while(0)

#define GIT_AUTHOR_NAME_ENVIRONMENT "GIT_AUTHOR_NAME"
#define GIT_AUTHOR_EMAIL_ENVIRONMENT "GIT_AUTHOR_EMAIL"
#define GIT_AUTHOR_DATE_ENVIRONMENT "GIT_AUTHOR_DATE"

#define GIT_COMMITTER_NAME_ENVIRONMENT "GIT_COMMITTER_NAME"
#define GIT_COMMITTER_EMAIL_ENVIRONMENT "GIT_COMMITTER_EMAIL"
#define GIT_COMMITTER_DATE_ENVIRONMENT "GIT_COMMITTER_DATE"

struct mergehead_peel_payload
{
    git_repository *repo;
    git_commit **next_parent;
};

GitHelper::GitHelper()
{
    git_libgit2_init();
}

void checkoutProgressCallback(const char *path,
                              size_t completed_steps,
                              size_t total_steps,
                              void *payload)
{
    auto py = static_cast<GitHelper *>(payload);
    if(py && py->checkoutProgressFunc)
    {
        py->checkoutProgressFunc(path, completed_steps, total_steps);
    }
}

int transferProgressCallback(const git_transfer_progress *stats, void *data)
{
    auto py = static_cast<GitHelper *>(data);
    if(py && py->transferProgressFunc)
    {
        py->transferProgressFunc(GitTransferProgress(*stats));
    }
    //    int ret = 0;
    //    if(stats)
    //    {
    //        float percent = 100.0 * stats->received_objects / stats->total_objects;

    //        qDebug() << "fetch transfer progress:" << QString::number(percent, 'f', 2);
    //    }

    return 0;
}
void printGitStrarray(git_strarray &list, const QString &head)
{
    for(size_t i = 0; i < list.count; i++)
    {
        auto str = list.strings[i];
        qDebug() << head << str;
    }
}
int sidebandProgressCallback(const char *str, int len, void *payload)
{
    auto py = static_cast<GitHelper *>(payload);
    if(py && py->sidebandProgressFunc)
    {
        py->sidebandProgressFunc(QString::fromUtf8(str, len));
    }
    //    qDebug() << "sidebandProgressCallback" << QString::fromStdString(str);
    return 0;
}

void GitHelper::getBranchs(QStringList &ret, GitTypes::BranchType btype)
{
    if(!g_repo)
    {
        return;
    }
    int error;
    git_reference *ref = NULL;
    git_branch_t btype2;
    git_branch_iterator *iter = NULL;
    git_branch_iterator_new(&iter, g_repo, (git_branch_t)btype);
    while((error = git_branch_next(&ref, &btype2, iter)) == 0)
    {
        const char *name;
        git_branch_name(&name, ref);
        git_reference_free(ref);
        ret << name;
    }
    git_branch_iterator_free(iter);
}

void GitHelper::getOids(QList<OId> &ret, GitTypes::SortType sort, const QString &refname)
{
    if(!g_repo)
    {
        return;
    }
    git_revwalk *walk;
    git_oid id;
    int error;
    git_revwalk_new(&walk, g_repo);
    git_revwalk_sorting(walk, sort);
    git_revwalk_push_ref(walk, refname.toStdString().c_str());
    while((error = git_revwalk_next(&id, walk)) == 0)
    {
        ret << OId(&id);
    }
    git_revwalk_free(walk);
}

int GitHelper::getDifferenceOids(QList<OId> &ret, const QString &refname1, const QString &refname2)
{
    QList<OId> oids1, oids2;
    getOids(oids1, GitTypes::GIT_SORT_TIME, refname1);
    getOids(oids2, GitTypes::GIT_SORT_TIME, refname2);
    foreach(auto oid1, oids1)
    {
        auto hasEqual = false;
        foreach(auto oid2, oids2)
        {
            if(git_oid_equal(oid1.getOid(), oid2.getOid()))
            {
                hasEqual = true;
                break;
            }
        }
        if(!hasEqual)
        {
            ret << oid1;
        }
    }
    return ret.size();
}

int GitHelper::getDifferenceOids(QList<OId> &ret, QList<OId> &oids1, QList<OId> &oids2)
{
    foreach(auto oid1, oids1)
    {
        auto hasEqual = false;
        foreach(auto oid2, oids2)
        {
            if(git_oid_equal(oid1.getOid(), oid2.getOid()))
            {
                hasEqual = true;
                break;
            }
        }
        if(!hasEqual)
        {
            ret << oid1;
        }
    }
    return ret.size();
}
void check_error()
{
    const git_error *error = giterr_last();
    if(error)
    {
        qDebug() << QString("Error %1 - %2").arg(error->klass).arg(error->message ? error->message : "???");
    }


}

int GitHelper::merge(GitTypes::CheckoutStrategyType checkout_strategy, const char *remoteName, const char *branchName)
{
    if(!g_repo)
    {
        return GIT_ENOTFOUND;
    }

    int err = GIT_OK;
    int autocommit = 1;

    git_merge_analysis_t analysis;
    git_merge_preference_t preference;

    auto str = QString("%1/%2").arg(remoteName).arg(branchName).toStdString();
    const char *commit_str = str.c_str();
    git_reference *commit_ref = NULL;
    git_object *commit_obj = NULL;
    git_annotated_commit *commit_merge_head = NULL;

    git_reference *head_ref = NULL;

    git_tree *commit_tree_obj = NULL;
    git_reference *new_ref = NULL;

    git_merge_options merge_options = GIT_MERGE_OPTIONS_INIT;
    git_checkout_options checkout_options = GIT_CHECKOUT_OPTIONS_INIT;

    git_index *index = NULL;

    if((err = git_branch_lookup(&commit_ref, g_repo, commit_str, GIT_BRANCH_LOCAL)))
    {
        if(err == GIT_ENOTFOUND)
        {
            if((err = git_branch_lookup(&commit_ref, g_repo, commit_str, GIT_BRANCH_REMOTE)))
            {
                goto out;
            }
        }
        else
        {
            goto out;
        }
    }
    if((err = git_annotated_commit_from_ref(&commit_merge_head, g_repo, commit_ref)))
    {
        goto out;
    }

    if((err = git_merge_analysis(&analysis, &preference, g_repo, (const git_annotated_commit **)&commit_merge_head, 1)))
    {
        goto out;
    }

    if(analysis & GIT_MERGE_ANALYSIS_UP_TO_DATE)
    {
        qDebug() << QString("Your branch is up to date with '%1'.").arg(commit_str).toStdString().c_str();
        goto  out;
    }

    checkout_options.checkout_strategy = checkout_strategy;


    if(analysis & GIT_MERGE_ANALYSIS_FASTFORWARD && preference != GIT_MERGE_PREFERENCE_NO_FASTFORWARD)
    {

        if((err = git_reference_peel(&commit_obj, commit_ref, GIT_OBJ_COMMIT)))
        {
            goto out;
        }

        if((err = git_repository_head(&head_ref, g_repo)))
        {
            goto out;
        }

        if((err = git_commit_tree(&commit_tree_obj, (git_commit *)commit_obj)))
        {
            goto out;
        }

        if((err = git_checkout_tree(g_repo, (git_object *)commit_tree_obj, &checkout_options)))
        {
            goto out;
        }

        if((err = git_reference_set_target(&new_ref, head_ref, git_commit_id((git_commit *)commit_obj), NULL)))
        {
            goto out;
        }

        goto out;
    }

    if((err = git_merge(g_repo, (const git_annotated_commit **)&commit_merge_head, 1, &merge_options, &checkout_options)))
    {
        goto out;
    }

    if((err = git_repository_index(&index, g_repo)))
    {
        goto out;
    }

    if(!git_index_has_conflicts(index))
    {
        if(autocommit)
        {
            //            char *argv[3];
            //            char message[256];
            //            sprintf(message, "Merged branch '%s'", commit_str);
            //            argv[0] = "commit";
            //            argv[1] = "-m";
            //            argv[2] = message;
            //cmd_commit_repo(repo, 3, argv);
        }
    }
    else
    {
        qDebug() << "conflict during merge! Please resolve and commit";
    }
out:
    check_error();

    if(index)
    {
        git_index_free(index);
    }
    if(commit_merge_head)
    {
        git_annotated_commit_free(commit_merge_head);
    }
    if(commit_obj)
    {
        git_object_free(commit_obj);
    }
    if(commit_ref)
    {
        git_reference_free(commit_ref);
    }
    if(head_ref)
    {
        git_reference_free(head_ref);
    }
    if(commit_tree_obj)
    {
        git_tree_free(commit_tree_obj);
    }
    if(new_ref)
    {
        git_reference_free(new_ref);
    }

    return err;
}
int mergehead_peel_cb(const git_oid *oid, void *payload)
{
    int err;
    struct mergehead_peel_payload *peel_payload = (struct mergehead_peel_payload *)payload;

    if((err = git_commit_lookup(peel_payload->next_parent, peel_payload->repo, oid)))
    {
        return -1;
    }
    peel_payload->next_parent++;
    return 0;
}
int mergehead_count_cb(const git_oid *oid, void *payload)
{
    int *nparents = (int *)payload;
    *nparents = *nparents + 1;
    return 0;
}
int sgit_repository_mergeheads_count(int *num_parents, git_repository *repo)
{
    return git_repository_mergehead_foreach(repo, mergehead_count_cb, num_parents);
}
int sgit_get_author_signature(git_repository *repo, git_signature **author_signature)
{
    int err;
    int author_offset = 0;
    char *author_name = NULL;
    char *author_email = NULL;
    char *author_date = NULL;
    unsigned long author_timestamp = 0;

    err = git_signature_default(author_signature, repo);
    if(err == 0)
    {
        return 0;
    }
    if(err != GIT_ENOTFOUND)
    {
        return err;
    }

    author_name = getenv(GIT_AUTHOR_NAME_ENVIRONMENT);
    author_email = getenv(GIT_AUTHOR_EMAIL_ENVIRONMENT);
    author_date = getenv(GIT_AUTHOR_DATE_ENVIRONMENT);

    if(!author_name || !author_email)
    {
        fprintf(stderr, "Author information not properly configured!\n");
    }

    if(!author_name)
    {
        author_name = "hsx";
    }

    if(!author_email)
    {
        author_email = "hsxhsx@qq.com";
    }

    /*if(author_date)
    {
        if((parse_date_basic(author_date, &author_timestamp, &author_offset)))
        {
            fprintf(stderr, "Bad author date format\n!");
            return GIT_ERROR;
        }
    }*/

    if(!author_timestamp)
    {
        return git_signature_now(author_signature, author_name, author_email);
    }
    else
    {
        return git_signature_new(author_signature, author_name, author_email,
                                 author_timestamp, author_offset);
    }
}

int sgit_get_committer_signature(git_repository *repo, git_signature **committer_signature, char *committer_name, char *committer_email)
{
    int err;
    char *committer_date = NULL;
    unsigned long committer_timestamp = 0;
    int committer_offset = 0;

    err = git_signature_default(committer_signature, repo);
    if(err == 0)
    {
        return 0;
    }
    if(err != GIT_ENOTFOUND)
    {
        return err;
    }

    committer_name = getenv(GIT_COMMITTER_NAME_ENVIRONMENT);
    committer_email = getenv(GIT_COMMITTER_EMAIL_ENVIRONMENT);
    committer_date = getenv(GIT_COMMITTER_DATE_ENVIRONMENT);

    if(!committer_name || !committer_email)
    {
        fprintf(stderr, "Committer information not properly configured!\n");
    }

    /*if(committer_date)
    {
        if((parse_date_basic(committer_date, &committer_timestamp, &committer_offset)))
        {
            fprintf(stderr, "Bad committer date format\n!");
            return GIT_ERROR;
        }
    }*/
    if(!committer_timestamp)
    {
        return git_signature_now(committer_signature, committer_name, committer_email);
    }
    else
    {
        return git_signature_new(committer_signature, committer_name,
                                 committer_email, committer_timestamp, committer_offset);
    }
}
int GitHelper::commit(char *committer_name, char *committer_email)
{
    int err = 0;
    git_reference *head = NULL;
    git_reference *branch = NULL;
    git_commit *parent = NULL;
    git_oid tree_oid;
    git_oid commit_oid;
    git_commit *commit = NULL;

    git_index *idx = NULL;

    git_signature *author_signature = NULL;
    git_signature *committer_signature = NULL;
    git_tree *tree = NULL;
    git_commit **parents = NULL;
    //TODO: write the message for commit
    const char *message = "demoCommit";

    int i;
    int rc = EXIT_FAILURE;
    int num_parents = 0;

    /* Count the number of parents */
    if((git_repository_head(&head, g_repo)) == GIT_OK)
    {
        num_parents++;
    }

    err = sgit_repository_mergeheads_count(&num_parents, g_repo);
    if(err && err != GIT_ENOTFOUND)
    {
        goto out;
    }

    /* Now determine the actual parents */
    if(num_parents)
    {
        if(!(parents = (git_commit **)malloc(sizeof(*parents) * num_parents)))
        {
            fprintf(stderr, "Not enough memory!\n");
            goto out;
        }

        if((err = git_reference_peel((git_object **)&parent, head, GIT_OBJ_COMMIT)))
        {
            goto out;
        }
        parents[0] = parent;

        if(num_parents > 1)
        {
            struct mergehead_peel_payload peel_payload;


            peel_payload.repo = g_repo;
            peel_payload.next_parent = parents + 1;

            if((err = git_repository_mergehead_foreach(g_repo, mergehead_peel_cb, &peel_payload)))
            {
                goto out;
            }
        }
    }

    if((err = sgit_get_author_signature(g_repo, &author_signature)) != GIT_OK)
    {
        goto out;
    }

    if((err = sgit_get_committer_signature(g_repo, &committer_signature, committer_name, committer_email)) != GIT_OK)
    {
        goto out;
    }

    /* Write index as tree */
    if((err = git_repository_index(&idx, g_repo)) != GIT_OK)
    {
        goto out;
    }
    if(git_index_entrycount(idx) == 0)
    {
        fprintf(stderr, "Nothing to commit!\n");
        goto out;
    }
    if((err = git_index_write_tree_to(&tree_oid, idx, g_repo)) != GIT_OK)
    {
        goto out;
    }
    if((err = git_tree_lookup(&tree, g_repo, &tree_oid)) != GIT_OK)
    {
        goto out;
    }

    /* Write tree as commit */
    if((err = git_commit_create(&commit_oid, g_repo, "HEAD", author_signature, committer_signature,
                                NULL, message, tree, num_parents, (const git_commit * *)parents)) != GIT_OK)
    {
        goto out;
    }

    rc = EXIT_SUCCESS;
out:
    check_error();
    if(head)
    {
        git_reference_free(head);
    }
    if(tree)
    {
        git_tree_free(tree);
    }
    if(idx)
    {
        git_index_free(idx);
    }
    if(parents)
    {
        for(i = 0; i < num_parents; i++)
        {
            git_commit_free(parents[i]);
        }
        free(parents);
    }
    if(author_signature)
    {
        git_signature_free(author_signature);
    }
    if(committer_signature)
    {
        git_signature_free(committer_signature);
    }
    if(commit)
    {
        git_commit_free(commit);
    }
    if(branch)
    {
        git_reference_free(branch);
    }
    return rc;
}
int GitHelper::pull(GitTypes::CheckoutStrategyType checkout_strategy, const char *remoteName, const char *branchName)
{
    if(!g_repo)
    {
        return GIT_ENOTFOUND;
    }

    auto er = featch(remoteName);
    if(er)
    {
        return er;
    }
    er = merge(checkout_strategy, remoteName, branchName);
    if(er)
    {
        return er;
    }
    return er;
}

void GitHelper::getCommitInfo(const OId &oid, CommitInfo &ret)
{
    git_commit *commit;
    git_commit_lookup(&commit, g_repo, oid.getOid());
    ret.oid = git_oid_tostr_s(oid.getOid());
    ret.author = git_commit_author(commit)->name;
    ret.message = git_commit_message(commit);
    auto time = git_commit_time(commit);
    ret.time = QDateTime::fromTime_t(time);
    git_commit_free(commit);
}

const QString GitHelper::toReferenceFriendlyName(const QString &fullname)
{
    if(fullname.startsWith(LocalBranchPrefix))
    {
        return fullname.mid(LocalBranchPrefix.length());
    }
    if(fullname.startsWith(RemoteTrackingBranchPrefix))
    {
        return fullname.mid(RemoteTrackingBranchPrefix.length());
    }
    return "";
}
void GitHelper::getReferences(QStringList &ret, const char *remoteName)
{
    if(!remoteName)
    {
        return;
    }
    git_strarray list;
    git_remote *remote;
    git_remote_lookup(&remote, g_repo, remoteName);
    git_remote_get_fetch_refspecs(&list, remote);
    for(size_t i = 0; i < list.count; i++)
    {
        auto str = list.strings[i];
        ret << str;
    }
    git_remote_free(remote);
    git_strarray_dispose(&list);
}
void GitHelper::getRemotes(QStringList &ret)
{
    if(!g_repo)
    {
        return;
    }
    git_strarray list;
    git_remote_list(&list, g_repo);
    for(size_t i = 0; i < list.count; i++)
    {
        auto str = list.strings[i];
        ret << str;
    }
    git_strarray_dispose(&list);
}
void GitHelper::test()
{
    if(!g_repo)
    {
        return;
    }
    QStringList remotes;
    getRemotes(remotes);
    foreach(auto name, remotes)
    {
        qDebug() << "remote:" << name;
        QStringList references;
        getReferences(references, name.toStdString().c_str());
        qDebug() << "\treferences:" << references;
    }
    QStringList localBranchs;
    getBranchs(localBranchs, GitTypes::GIT_BRANCH_LOCAL);
    qDebug() << "GIT_BRANCH_LOCAL:" << localBranchs;

    QStringList remoteBranchs;
    getBranchs(remoteBranchs, GitTypes::GIT_BRANCH_REMOTE);
    qDebug() << "GIT_BRANCH_REMOTE:" << remoteBranchs;


    foreach(auto lbn, localBranchs)
    {
        foreach(auto rbn, remoteBranchs)
        {
            if(rbn.endsWith(lbn))
            {
                qDebug() << "now lookup branch:" << lbn;

                QList<OId> oids1;
                QList<OId> oids2;
                QList<OId> oidsdiff;
                auto refname1 = RemoteTrackingBranchPrefix + rbn;
                auto refname2 = LocalBranchPrefix + lbn;

                getOids(oids1, GitTypes::GIT_SORT_TIME, refname1);
                getOids(oids2, GitTypes::GIT_SORT_TIME, refname2);
                getDifferenceOids(oidsdiff, oids1, oids2);
                foreach(auto oid, oidsdiff)
                {
                    CommitInfo ci;
                    getCommitInfo(oid.getOid(), ci);
                    qDebug() << "\tcommit:" << ci.oid.left(6) << ci.time.toString(Qt::DateFormat::ISODate) << ci.author << ci.message;
                }
                break;
            }
        }
    }

    pull();
}

int GitHelper::checkout(GitTypes::CheckoutStrategyType strategy, const QStringList &paths)
{
    if(!g_repo)
    {
        return GIT_ENOTFOUND;
    }
    git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;
    opts.checkout_strategy = strategy;
    git_strarray git_paths = {0};
    if(paths.size())
    {
        char **paths_char=new char*[paths.size()];
        for(int i = 0; i < paths.size(); ++i)
        {
            paths_char[i] = (char *)paths.at(i).toStdString().c_str();
        }
        git_paths.count = paths.size();
        git_paths.strings = paths_char;
        opts.paths = git_paths;
    }
    int er = GIT_OK;
    qdebug_git_fail((er = git_checkout_head(g_repo, &opts)));
    return er;
}
int GitHelper::featch(const char *remoteName)
{
    if(!g_repo)
    {
        return GIT_ENOTFOUND;
    }

    git_remote *remote = NULL;

    git_fetch_options fetch_opts = GIT_FETCH_OPTIONS_INIT;

    int r = git_remote_lookup(&remote, g_repo, remoteName);

    fetch_opts.callbacks.sideband_progress = &sidebandProgressCallback;
    fetch_opts.callbacks.transfer_progress = &transferProgressCallback;


    r = git_remote_fetch(remote, NULL, &fetch_opts, "fetch");

    check_error();

    //    auto stats = git_remote_stats(remote);
    git_remote_free(remote);
    return r;
}
int GitHelper::open(const QString &path)
{
    int er = GIT_OK;
    qdebug_git_fail((er = git_repository_open(&g_repo, path.toStdString().c_str())));
    return er;
}
int GitHelper::clone(const QString &url, const QString &saveOfPath)
{
    git_checkout_options dummy_opts = GIT_CHECKOUT_OPTIONS_INIT;
    dummy_opts.perfdata_payload = (void *) this;
    dummy_opts.progress_cb = &checkoutProgressCallback;
    git_fetch_options dummy_fetch = GIT_FETCH_OPTIONS_INIT;
    git_remote_callbacks remoteCallbacks = GIT_REMOTE_CALLBACKS_INIT;
    remoteCallbacks.transfer_progress = &transferProgressCallback;
    remoteCallbacks.sideband_progress = &sidebandProgressCallback;
    remoteCallbacks.payload = (void *) this;
    dummy_fetch.callbacks = remoteCallbacks;

    git_clone_options options = GIT_CLONE_OPTIONS_INIT;
    memset(&options, 0, sizeof(git_clone_options));
    options.version = GIT_CLONE_OPTIONS_VERSION;
    options.checkout_opts = dummy_opts;
    options.checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE;
    options.fetch_opts = dummy_fetch;

    int er = GIT_OK;
    qdebug_git_fail((er = git_clone(&g_repo,
                                    url.toStdString().c_str(),
                                    saveOfPath.toStdString().c_str(),
                                    &options)));

    return er;
}

GitHelper::~GitHelper()
{
    git_libgit2_shutdown();
}
