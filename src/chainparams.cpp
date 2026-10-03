// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>

#include <tinyformat.h>
#include <util/system.h>
#include <util/strencodings.h>

#include <assert.h>
#include <cstdio>
#include <cstdlib>

#include <chainparamsseeds.h>
#include <arith_uint256.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion)
{
CMutableTransaction txNew;
txNew.nVersion = 1;
txNew.nTime = nTime;
txNew.vin.resize(1);
txNew.vout.resize(1);
txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
txNew.vout[0].SetEmpty();

CBlock genesis;
genesis.nTime = nTime;
genesis.nBits = nBits;
genesis.nNonce = nNonce;
genesis.nVersion = nVersion;
genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
genesis.hashPrevBlock.SetNull();
genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
return genesis;
}

/**
* Build the genesis block. Note that the output of its generation
* transaction cannot be spent since it did not originally exist in the
* database.
*/
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, bool isTestnet = false)
{
const char* pszTimestamp = isTestnet ? "VERGE TESTNET" : "MicroByte MBC genesis 2026-10-02";
return CreateGenesisBlock(pszTimestamp, nTime, nNonce, nBits, nVersion);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
consensus.vDeployments[d].nStartTime = nStartTime;
consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
* Main network
*/
/**
* What makes a good checkpoint block?
* + Is surrounded by blocks with reasonable timestamps
* (no blocks before with a timestamp after, none after with
* timestamp before)
* + Contains no strange transactions
*/

class CMainParams : public CChainParams {
public:
CMainParams() {
strNetworkID = "main";
consensus.ForkHeight = 1;
consensus.MULTI_ALGO_SWITCH_BLOCK = 2000000000;
consensus.STEALTH_TX_SWITCH_BLOCK = 0;
consensus.FlexibleMiningAlgorithms = 2000000000;
consensus.CLOCK_DRIFT_FORK = 0;
consensus.nSubsidyHalvingInterval = 500000;

consensus.BIP34Height = consensus.ForkHeight;
consensus.BIP65Height = consensus.ForkHeight;
consensus.BIP66Height = consensus.ForkHeight;


consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000"); //ArithToUint256(~arith_uint256(0) >> 20);
consensus.nPowTargetTimespan = 30; // diff readjusting time
consensus.nPowTargetSpacing = 30; //
consensus.fPowAllowMinDifficultyBlocks = false;
consensus.fPowNoRetargeting = false;
consensus.nRuleChangeActivationThreshold = 100; // 100 out of 200 blocks
consensus.nMinerConfirmationWindow = 200;

consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

// Deployment of BIP68, BIP112, and BIP113.
consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1529247969; // May 1st, 2016
consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1559347200; // May 1st, 2017

// Deployment of SegWit (BIP141, BIP143, and BIP147)
// It is all safe :thumbsup:
consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1529247969; // November 15th, 2016.
consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1559347200; // November 15th, 2017.

// The best chain should have at least this much work.
// KeyNote: (Kind of like a checkpoint)
consensus.nMinimumChainWork = uint256S("0x00");

// By default assume that the signatures in ancestors of this block are valid.
// KeyNote: Seems like speedup, similar to a checkpoint
consensus.defaultAssumeValid = uint256S("0x00");

/**
* The message start string is designed to be unlikely to occur in normal data.
* The characters are rarely used upper ASCII, not valid as UTF-8, and produce
* a large 32-bit integer with any alignment.
*/
pchMessageStart[0] = 0xc3;
pchMessageStart[1] = 0x9e;
pchMessageStart[2] = 0xd1;
pchMessageStart[3] = 0xa7;
nDefaultPort = 41820;
nPruneAfterHeight = 100000;

genesis = CreateGenesisBlock(1790978287, 718875, 0x1e0fffff, 1);
consensus.hashGenesisBlock = genesis.GetHash();
assert(genesis.hashMerkleRoot == uint256S("0xefeac44bfd7be254c1dca7cbc66db946aebe4123ef4e8c49b6756a15feb227d6"));
assert(consensus.hashGenesisBlock == uint256S("0x000000f9d7e4d320e006e4ee28ff8c6f59ed7ed69df8f70d47f1638e3ba46abe"));

// Note that of those which support the service bits prefix, most only support a subset of
// possible options.
// This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
// service bits we want, but we should get them updated to support all service bits wanted by any
// release ASAP to avoid it where possible.

base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,50);
base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,55);
base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1,178); //128 + PUBKEY_ADDRESS
base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x6d, 0x62, 0x01};
base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x6d, 0x62, 0x02};

bech32_hrp = "mb";

vFixedSeeds.clear();

fDefaultConsistencyChecks = false;
fRequireStandard = true;
fMineBlocksOnDemand = false;

checkpointData = {
{
{ 0, consensus.hashGenesisBlock },
}
};

checkpointIndexData = {
{
{ 0, consensus.hashGenesisBlock },
}
};

chainTxData = ChainTxData{
1790978287,
1,
0.01
};

// reorg
consensus.nMaxReorgDepth = 6;
consensus.nMaxReorgDepthEnforcementBlock = 4800000;

/* disable fallback fee on mainnet */
m_fallback_fee_enabled = false;
}
};

/**
* Testnet (v3)
*/
class CTestNetParams : public CChainParams {
public:
CTestNetParams() {
// KeyNote: we'll leave testnet as is for now
strNetworkID = "test";
consensus.nSubsidyHalvingInterval = 210000;
consensus.MULTI_ALGO_SWITCH_BLOCK = 340000;
consensus.STEALTH_TX_SWITCH_BLOCK = 1824150;
consensus.FlexibleMiningAlgorithms = 2042000;
consensus.CLOCK_DRIFT_FORK = 2218500;

consensus.BIP34Height = 0;
consensus.BIP65Height = 0; // 00000000007f6655f22f98e72ed80d8b06dc761d5da09df0fa1dc4be4f861eb6
consensus.BIP66Height = 0; // 000000002104c8c45e99a8853285a3b592602a3ccde2b832481da85e9e4ba182
consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000"); // FIXME: change to actual genesis limit
consensus.nPowTargetTimespan = 24 * 60 * 60; // 1 day
consensus.nPowTargetSpacing = 45;
consensus.fPowAllowMinDifficultyBlocks = true;
consensus.fPowNoRetargeting = false;
consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

// Deployment of BIP68, BIP112, and BIP113.
consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1456790400; // March 1st, 2016
consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

// Deployment of SegWit (BIP141, BIP143, and BIP147)
consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1462060800; // May 1st 2016
consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1493596800; // May 1st 2017

// The best chain should have at least this much work.
consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000000000000000000f");

// By default assume that the signatures in ancestors of this block are valid.
consensus.defaultAssumeValid = uint256S("0x65b4e101cacf3e1e4f3a9237e3a74ffd1186e595d8b78fa8ea22c21ef5bf9347"); //also genesis

pchMessageStart[0] = 0xcd;
pchMessageStart[1] = 0xf2;
pchMessageStart[2] = 0xc0;
pchMessageStart[3] = 0xef;
nDefaultPort = 21104;
nPruneAfterHeight = 1000;

genesis = CreateGenesisBlock(1462058066, 2, 0x1e0fffff, 1, true);
consensus.hashGenesisBlock = genesis.GetHash();

assert(consensus.hashGenesisBlock == uint256S("0x65b4e101cacf3e1e4f3a9237e3a74ffd1186e595d8b78fa8ea22c21ef5bf9347"));
assert(genesis.hashMerkleRoot == uint256S("0x768cc22f70bbcc4de26f83aca1b4ea2a7e25f0d100497ba47c7ff2d9b696414c"));

vFixedSeeds.clear();
vSeeds.clear();
// nodes with support for servicebits filtering should be at the top
vSeeds.emplace_back("uacxdw34wnfybshfjs6hxdfzwkqxs765peu4iyyakqnz2mqyvspubeqd.onion"); // dedicated testnet server
vSeeds.emplace_back("ankgfybzzxzvu3ogo4fkopz6nfk4qv3j7gbxllawx6or5oxxjtujeyqd.onion"); // testnet blockchain explorer
vSeeds.emplace_back("2l7hxpeyhmy4c2tnlmgf7rgcn6epsaaspv7473f3r5uncqzs6pnltqqd.onion");
vSeeds.emplace_back("qwwqi7h6bkkcw6clp34ttg5mxmcwerkot2hepfhi6g4yclvlhsv2kxid.onion");
vSeeds.emplace_back("2l7hxpeyhmy4c2tnlmgf7rgcn6epsaaspv7473f3r5uncqzs6pnltqqd.onion");

base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,115);
base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,198);
base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1,243); // 128 + PUBKEY_ADDRESS_TEST
base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

bech32_hrp = "vt";

vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

fDefaultConsistencyChecks = false;
fRequireStandard = false;
fMineBlocksOnDemand = false;


checkpointData = {
{
{0, uint256S("65b4e101cacf3e1e4f3a9237e3a74ffd1186e595d8b78fa8ea22c21ef5bf9347")},
}
};

chainTxData = ChainTxData{
// Data as of block 000000000000033cfa3c975eb83ecf2bb4aaedf68e6d279f6ed2b427c64caff9 (height 1260526)
1462058066,
1,
0.1
};

// reorg
consensus.nMaxReorgDepth = 4;
consensus.nMaxReorgDepthEnforcementBlock = 100;

/* enable fallback fee on testnet */
m_fallback_fee_enabled = true;
}
};

/**
* Regression test
*/
class CRegTestParams : public CChainParams {
public:
CRegTestParams() {
strNetworkID = "regtest";
consensus.nSubsidyHalvingInterval = 150;
consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)

consensus.ForkHeight = 2800000;
consensus.MULTI_ALGO_SWITCH_BLOCK = 340000;
consensus.STEALTH_TX_SWITCH_BLOCK = 1824150;
consensus.FlexibleMiningAlgorithms = 2042000;
consensus.CLOCK_DRIFT_FORK = 2218500;
consensus.nSubsidyHalvingInterval = 500000;
consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

consensus.nPowTargetTimespan = 7 * 24 * 60 * 60; // one week
consensus.nPowTargetSpacing = 30;
consensus.fPowAllowMinDifficultyBlocks = true;
consensus.fPowNoRetargeting = true;
consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)

consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

// The best chain should have at least this much work.
consensus.nMinimumChainWork = uint256S("0x00");

// By default assume that the signatures in ancestors of this block are valid.
consensus.defaultAssumeValid = uint256S("0x00");

pchMessageStart[0] = 0xfa;
pchMessageStart[1] = 0xbf;
pchMessageStart[2] = 0xb5;
pchMessageStart[3] = 0xda;
nDefaultPort = 31102;
nPruneAfterHeight = 1000;

genesis = CreateGenesisBlock(1462058066, 2, 0x1e0fffff, 1, true);
consensus.hashGenesisBlock = genesis.GetHash();

assert(consensus.hashGenesisBlock == uint256S("0x65b4e101cacf3e1e4f3a9237e3a74ffd1186e595d8b78fa8ea22c21ef5bf9347"));
assert(genesis.hashMerkleRoot == uint256S("0x768cc22f70bbcc4de26f83aca1b4ea2a7e25f0d100497ba47c7ff2d9b696414c"));

vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
vSeeds.clear(); //!< Regtest mode doesn't have any DNS seeds.

fDefaultConsistencyChecks = true;
fRequireStandard = false;
fMineBlocksOnDemand = true;

checkpointData = {
{}
};

chainTxData = ChainTxData{
0,
0,
0
};

base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1,239);
base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

bech32_hrp = "vgrt";

// reorg
consensus.nMaxReorgDepth = 4;
consensus.nMaxReorgDepthEnforcementBlock = 100;

/* enable fallback fee on regtest */
m_fallback_fee_enabled = true;
}
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
assert(globalChainParams);
return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
if (chain == CBaseChainParams::MAIN)
return std::unique_ptr<CChainParams>(new CMainParams());
else if (chain == CBaseChainParams::TESTNET)
return std::unique_ptr<CChainParams>(new CTestNetParams());
else if (chain == CBaseChainParams::REGTEST)
return std::unique_ptr<CChainParams>(new CRegTestParams());
throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
SelectBaseParams(network);
globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
