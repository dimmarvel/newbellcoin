// Copyright (c) 2015-2022 The bellscoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BELLSCOIN_COMMON_URL_H
#define BELLSCOIN_COMMON_URL_H

#include <string>

using UrlDecodeFn = std::string(const std::string& url_encoded);
UrlDecodeFn urlDecode;
extern UrlDecodeFn* const URL_DECODE;

#endif // BELLSCOIN_COMMON_URL_H
