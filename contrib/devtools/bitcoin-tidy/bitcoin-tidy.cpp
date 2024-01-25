// Copyright (c) 2023 bellscoin Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "logprintf.h"

#include <clang-tidy/ClangTidyModule.h>
#include <clang-tidy/ClangTidyModuleRegistry.h>

class BellscoinModule final : public clang::tidy::ClangTidyModule
{
public:
    void addCheckFactories(clang::tidy::ClangTidyCheckFactories& CheckFactories) override
    {
        CheckFactories.registerCheck<bellscoin::LogPrintfCheck>("bellscoin-unterminated-logprintf");
    }
};

static clang::tidy::ClangTidyModuleRegistry::Add<BellscoinModule>
    X("bellscoin-module", "Adds bellscoin checks.");

volatile int BellscoinModuleAnchorSource = 0;
