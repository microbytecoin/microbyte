# MicroByte (MBC)

MicroByte is a proof-of-work cryptocurrency forked from [Verge Core](https://github.com/vergecurrency/verge) v26.8. It keeps Verge's codebase but runs a single mining algorithm (Blake2s), its own genesis block, its own network parameters and its own block reward schedule.

> **Status: experimental.** MicroByte is running on a private test chain only. Consensus parameters, including the genesis block, may change before any public launch. Do not treat any coins on the test chain as having value.

MicroByte is not affiliated with or endorsed by the Verge project.

## Specifications

| Specification | Value |
|---|---|
| Consensus | Proof of work |
| Algorithm | Blake2s only (other algorithms are rejected) |
| Block time | 30 seconds |
| Block signature | Required (coinbase pays a public key, block is signed with it) |
| Coinbase maturity | 120 blocks |
| P2P port | 41820 |
| RPC port | 41821 |
| Address prefix | `M` (pubkey byte 50, script byte 55) |
| Bech32 prefix | `mb` |
| Network magic bytes | `c3 9e d1 a7` |
| Pre-mine / ICO | None (the genesis block pays nothing) |

### Block rewards

Fees are paid to the miner in addition to the subsidy.

| Block range | Subsidy | Coins issued in range |
|---|---|---|
| 1 to 1,666,666 | 8 MBC | 13,333,328 |
| 1,666,667 to 3,333,332 | 4 MBC | 6,666,664 |
| 3,333,333 to 4,999,998 | 2 MBC | 3,333,332 |
| 4,999,999 to 6,666,664 | 1 MBC | 1,666,666 |
| 6,666,665 onward | 0.5 MBC | open-ended tail |

The four halving eras issue 24,999,990 MBC (about 25 million) in roughly 6.3 years at 30-second blocks. After that, a permanent 0.5 MBC tail subsidy (about 525,600 MBC per year) continues, so supply is not hard-capped.

### Difficulty

Blocks below height 450 are mined at the minimum difficulty. From height 450, difficulty retargets on every block toward the 30-second target. With a small number of miners, expect block times to swing noticeably around the target.

### Genesis block (test chain)

| Field | Value |
|---|---|
| Time | 1790978287 |
| Nonce | 718875 |
| Hash | `000000f9d7e4d320e006e4ee28ff8c6f59ed7ed69df8f70d47f1638e3ba46abe` |
| Merkle root | `efeac44bfd7be254c1dca7cbc66db946aebe4123ef4e8c49b6756a15feb227d6` |

The block ID is the scrypt hash of the header. The proof of work is the Blake2s hash, which is what `pow_hash` shows in `getblock`.

## Differences from Verge

- Blake2s is the only accepted algorithm. `setalgo` rejects anything else.
- The multi-algorithm checks are switched off through the fork heights in `chainparams.cpp`.
- New reward schedule, genesis block, ports, magic bytes and address prefixes.
- Verge's seed nodes, checkpoints and chain-work values are removed.
- Binaries are `microbyted`, `microbyte-cli` and `microbyte-tx`.
- The data directory is `~/.microbyte` and the config file is `microbyte.conf`.
- Default minimum fee is 0.001 MBC per kB (a policy setting, not a consensus rule).

Inherited from Verge and not tested for MicroByte: stealth addresses, secure messaging (SMSG), Tor support.

## Building from source

Only Linux has been tested (Ubuntu). The steps below follow what has been built and run so far.

### 1. Dependencies

```shell
sudo apt update
sudo apt install build-essential libtool autotools-dev automake pkg-config \
  libssl-dev libevent-dev bsdmainutils libboost-all-dev libminiupnpc-dev \
  libzmq3-dev libseccomp-dev libsqlite3-dev libqrencode-dev git
```

The wallet needs BerkeleyDB 4.8, which Ubuntu does not package. Build it from source, or use the system library for fresh wallets only with `--with-incompatible-bdb`. Wallet files created with a different BDB version are not portable.

### 2. Build

```shell
git clone https://github.com/microbytecoin/microbyte.git
cd microbyte
./autogen.sh
./configure --without-gui --disable-bench --disable-tests \
  BDB_LIBS="-L$HOME/db4/lib -ldb_cxx-4.8" \
  BDB_CFLAGS="-I$HOME/db4/include"
make -j2
```

Adjust the BDB paths to wherever you built it. Do not use `--disable-wallet` if you want to mine, because block signing needs the wallet.

On a machine with 2 GB of RAM or less, add swap before compiling, and keep `-j` low. A message like `Killed` during `make` means the build ran out of memory.

The binaries are in `src/` (the real executables are in `src/.libs/` and `src/microbyted` is a libtool wrapper).

## Running a node

Create `~/.microbyte/microbyte.conf`:

```ini
server=1
daemon=1
rpcuser=choose_a_user
rpcpassword=choose_a_long_random_password
```

Start the node and check it:

```shell
./src/microbyted -daemon
./src/microbyte-cli getblockchaininfo
```

The RPC password is read when the node starts. After changing it, restart the node. Shut down with `microbyte-cli stop`, or send `SIGTERM`. Do not use `kill -9`.

To connect nodes to each other, add `addnode=<ip>:41820` to the config and open port 41820 in the firewall.

## Mining

The node has a built-in CPU miner:

```shell
./src/microbyte-cli -rpcclienttimeout=0 generate 10 2000000000
```

Notes:

- The second argument is the **total** hash budget for the whole call, not a per-block limit. The largest accepted value is 2147483647. If the call returns fewer blocks than requested, run it again.
- Use `generate`, not `generatetoaddress`. Blocks must pay a raw public key and be signed with it. `generate` does this with a wallet key, while `generatetoaddress` pays a normal address and produces blocks that fail validation.
- Once difficulty rises above the minimum, a single block can take minutes on a CPU. The node keeps mining even if the CLI times out, so check `getblockcount` before starting another `generate`.
- Block version for mined blocks is 8196.

## Wallet safety

- Back up your wallet: `microbyte-cli backupwallet /path/to/backup.dat`. Keep copies off the server.
- Never commit `wallet.dat`, `microbyte.conf` or backups to the repository.

## License and attribution

MicroByte is released under the MIT license, the same as Verge Core. Copyright notices of the Verge Core developers, the Bitcoin Core developers and other upstream authors are retained, as the license requires. See [COPYING](COPYING).

## Reporting issues

To be added. Please do not report security vulnerabilities in public issues.
