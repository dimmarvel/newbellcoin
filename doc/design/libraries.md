# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libbellscoin_cli*         | RPC client functionality used by *bellscoin-cli* executable |
| *libbellscoin_common*      | Home for common functionality shared by different executables and libraries. Similar to *libbellscoin_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libbellscoin_consensus*   | Stable, backwards-compatible consensus functionality used by *libbellscoin_node* and *libbellscoin_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libbellscoinconsensus*    | Shared library build of static *libbellscoin_consensus* library |
| *libbellscoin_kernel*      | Consensus engine and support library used for validation by *libbellscoin_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libbellscoinqt*           | GUI functionality used by *bellscoin-qt* and *bellscoin-gui* executables |
| *libbellscoin_ipc*         | IPC functionality used by *bellscoin-node*, *bellscoin-wallet*, *bellscoin-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libbellscoin_node*        | P2P and RPC server functionality used by *bellscoind* and *bellscoin-qt* executables. |
| *libbellscoin_util*        | Home for common functionality shared by different executables and libraries. Similar to *libbellscoin_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libbellscoin_wallet*      | Wallet functionality used by *bellscoind* and *bellscoin-wallet* executables. |
| *libbellscoin_wallet_tool* | Lower-level wallet functionality used by *bellscoin-wallet* executable. |
| *libbellscoin_zmq*         | [ZeroMQ](../zmq.md) functionality used by *bellscoind* and *bellscoin-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libbellscoin_consensus* and *libbellscoin_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libbellscoin_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libbellscoin_node* code lives in `src/node/` in the `node::` namespace
  - *libbellscoin_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libbellscoin_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libbellscoin_util* code lives in `src/util/` in the `util::` namespace
  - *libbellscoin_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "basis" }}}%%

graph TD;

bellscoin-cli[bellscoin-cli]-->libbellscoin_cli;

bellscoind[bellscoind]-->libbellscoin_node;
bellscoind[bellscoind]-->libbellscoin_wallet;

bellscoin-qt[bellscoin-qt]-->libbellscoin_node;
bellscoin-qt[bellscoin-qt]-->libbellscoinqt;
bellscoin-qt[bellscoin-qt]-->libbellscoin_wallet;

bellscoin-wallet[bellscoin-wallet]-->libbellscoin_wallet;
bellscoin-wallet[bellscoin-wallet]-->libbellscoin_wallet_tool;

libbellscoin_cli-->libbellscoin_util;
libbellscoin_cli-->libbellscoin_common;

libbellscoin_common-->libbellscoin_consensus;
libbellscoin_common-->libbellscoin_util;

libbellscoin_kernel-->libbellscoin_consensus;
libbellscoin_kernel-->libbellscoin_util;

libbellscoin_node-->libbellscoin_consensus;
libbellscoin_node-->libbellscoin_kernel;
libbellscoin_node-->libbellscoin_common;
libbellscoin_node-->libbellscoin_util;

libbellscoinqt-->libbellscoin_common;
libbellscoinqt-->libbellscoin_util;

libbellscoin_wallet-->libbellscoin_common;
libbellscoin_wallet-->libbellscoin_util;

libbellscoin_wallet_tool-->libbellscoin_wallet;
libbellscoin_wallet_tool-->libbellscoin_util;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class bellscoin-qt,bellscoind,bellscoin-cli,bellscoin-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libbellscoin_wallet* and *libbellscoin_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libbellscoin_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libbellscoin_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libbellscoin_common* should serve a similar function as *libbellscoin_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libbellscoin_util* and *libbellscoin_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for bellscoin-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libbellscoin_kernel* is not supposed to depend on *libbellscoin_common*, only *libbellscoin_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libbellscoin_kernel* should only depend on *libbellscoin_util* and *libbellscoin_consensus*.

- The only thing that should depend on *libbellscoin_kernel* internally should be *libbellscoin_node*. GUI and wallet libraries *libbellscoinqt* and *libbellscoin_wallet* in particular should not depend on *libbellscoin_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libbellscoin_consensus*, *libbellscoin_common*, and *libbellscoin_util*, instead of *libbellscoin_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libbellscoinqt*, *libbellscoin_node*, *libbellscoin_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libbellscoin_node* to *libbellscoin_kernel* as part of [The libbellscoinkernel Project #24303](https://github.com/bellscoin/bellscoin/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/bellscoin/bellscoin/issues/15732)
