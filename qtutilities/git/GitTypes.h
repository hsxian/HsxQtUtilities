#ifndef GITTYPES_H
#define GITTYPES_H
#include <qtutilities_global.h>

namespace hsx
{
    class QTUTILITIESSHARED_EXPORT  GitTypes
    {
    public:
        enum SortType
        {
            /**
             * Sort the output with the same default method from `git`: reverse
             * chronological order. This is the default sorting for new walkers.
             */
            GIT_SORT_NONE = 0,

            /**
             * Sort the repository contents in topological order (no parents before
             * all of its children are shown); this sorting mode can be combined
             * with time sorting to produce `git`'s `--date-order``.
             */
            GIT_SORT_TOPOLOGICAL = 1 << 0,

            /**
             * Sort the repository contents by commit time;
             * this sorting mode can be combined with
             * topological sorting.
             */
            GIT_SORT_TIME = 1 << 1,

            /**
             * Iterate through the repository contents in reverse
             * order; this sorting mode can be combined with
             * any of the above.
             */
            GIT_SORT_REVERSE = 1 << 2
        };

        enum CheckoutStrategyType
        {
            GIT_CHECKOUT_NONE = 0, /**< default is a dry run, no actual updates */

            /**
             * Allow safe updates that cannot overwrite uncommitted data.
             * If the uncommitted changes don't conflict with the checked out files,
             * the checkout will still proceed, leaving the changes intact.
             *
             * Mutually exclusive with GIT_CHECKOUT_FORCE.
             * GIT_CHECKOUT_FORCE takes precedence over GIT_CHECKOUT_SAFE.
             */
            GIT_CHECKOUT_SAFE = (1u << 0),

            /**
             * Allow all updates to force working directory to look like index.
             *
             * Mutually exclusive with GIT_CHECKOUT_SAFE.
             * GIT_CHECKOUT_FORCE takes precedence over GIT_CHECKOUT_SAFE.
             */
            GIT_CHECKOUT_FORCE = (1u << 1),


            /** Allow checkout to recreate missing files */
            GIT_CHECKOUT_RECREATE_MISSING = (1u << 2),

            /** Allow checkout to make safe updates even if conflicts are found */
            GIT_CHECKOUT_ALLOW_CONFLICTS = (1u << 4),

            /** Remove untracked files not in index (that are not ignored) */
            GIT_CHECKOUT_REMOVE_UNTRACKED = (1u << 5),

            /** Remove ignored files not in index */
            GIT_CHECKOUT_REMOVE_IGNORED = (1u << 6),

            /** Only update existing files, don't create new ones */
            GIT_CHECKOUT_UPDATE_ONLY = (1u << 7),

            /**
             * Normally checkout updates index entries as it goes; this stops that.
             * Implies `GIT_CHECKOUT_DONT_WRITE_INDEX`.
             */
            GIT_CHECKOUT_DONT_UPDATE_INDEX = (1u << 8),

            /** Don't refresh index/config/etc before doing checkout */
            GIT_CHECKOUT_NO_REFRESH = (1u << 9),

            /** Allow checkout to skip unmerged files */
            GIT_CHECKOUT_SKIP_UNMERGED = (1u << 10),
            /** For unmerged files, checkout stage 2 from index */
            GIT_CHECKOUT_USE_OURS = (1u << 11),
            /** For unmerged files, checkout stage 3 from index */
            GIT_CHECKOUT_USE_THEIRS = (1u << 12),

            /** Treat pathspec as simple list of exact match file paths */
            GIT_CHECKOUT_DISABLE_PATHSPEC_MATCH = (1u << 13),

            /** Ignore directories in use, they will be left empty */
            GIT_CHECKOUT_SKIP_LOCKED_DIRECTORIES = (1u << 18),

            /** Don't overwrite ignored files that exist in the checkout target */
            GIT_CHECKOUT_DONT_OVERWRITE_IGNORED = (1u << 19),

            /** Write normal merge files for conflicts */
            GIT_CHECKOUT_CONFLICT_STYLE_MERGE = (1u << 20),

            /** Include common ancestor data in diff3 format files for conflicts */
            GIT_CHECKOUT_CONFLICT_STYLE_DIFF3 = (1u << 21),

            /** Don't overwrite existing files or folders */
            GIT_CHECKOUT_DONT_REMOVE_EXISTING = (1u << 22),

            /** Normally checkout writes the index upon completion; this prevents that. */
            GIT_CHECKOUT_DONT_WRITE_INDEX = (1u << 23),

            /**
             * Show what would be done by a checkout.  Stop after sending
             * notifications; don't update the working directory or index.
             */
            GIT_CHECKOUT_DRY_RUN = (1u << 24),

            /** Include common ancestor data in zdiff3 format for conflicts */
            GIT_CHECKOUT_CONFLICT_STYLE_ZDIFF3 = (1u << 25),

            /**
             * THE FOLLOWING OPTIONS ARE NOT YET IMPLEMENTED
             */

            /** Recursively checkout submodules with same options (NOT IMPLEMENTED) */
            GIT_CHECKOUT_UPDATE_SUBMODULES = (1u << 16),
            /** Recursively checkout submodules if HEAD moved in super repo (NOT IMPLEMENTED) */
            GIT_CHECKOUT_UPDATE_SUBMODULES_IF_CHANGED = (1u << 17)
        };
        enum BranchType
        {
            GIT_BRANCH_LOCAL = 1,
            GIT_BRANCH_REMOTE = 2,
            GIT_BRANCH_ALL = GIT_BRANCH_LOCAL | GIT_BRANCH_REMOTE
        };
    };
}

#endif // GITTYPES_H
