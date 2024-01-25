#ifndef BELLSCOIN_NODE_PEERMAN_ARGS_H
#define BELLSCOIN_NODE_PEERMAN_ARGS_H

#include <net_processing.h>

class ArgsManager;

namespace node {
void ApplyArgsManOptions(const ArgsManager& argsman, PeerManager::Options& options);
} // namespace node

#endif // BELLSCOIN_NODE_PEERMAN_ARGS_H
