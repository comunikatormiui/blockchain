// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2019 The YODA developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "consensus/merkle.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>
#include <limits>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock = 0;
    genesis.nVersion = nVersion;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);

    return genesis;
}

/**
 * Build the genesis block. Note that the output of the genesis coinbase cannot
 * be spent as it did not originally exist in the database.
 * CBlock(hash=00000bac23ca6cbd519c77976eca66655dcad3d472508e0fb2bfd1d566ed8c45, ver=1, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, hashMerkleRoot=1ff345800c3466335dbc6c5b276d7b275e74e429ea7f214a0a895bb37379dee1, nTime=1584619200, nBits=1e0ffff0, nNonce=3655424, vtx=1)
 *  CTransaction(hash=1ff345800c, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(0000000000000000000000000000000000000000000000000000000000000000, 4294967295), coinbase 04ffff001d01044c564d617263682031392c20323032303a2043616c6c2049742061204c61796f66662c2061204675726c6f756768206f722061204375742053686966743a20416d65726963616e7320617265204c6f73696e6720576f726b)
 *     CTxOut(nValue=250.00000000, scriptPubKey=047d65ca6b189b89bf53e00d328392)
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Coronavirus Projected to Trigger Worst Economic Downturn Since 1940s";
    const CScript genesisOutputScript = CScript() << ParseHex("047d65ca6b189b89bf53e00d328392ba157410d84938c927c5c295569b633b7d5e3989237e9a773b2d871887688f8b0f564e75d9477c4fd24a36335ea3a304022a") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of(0, uint256("0x000005703f02ae1a0243a10fde1c3c613055dcca254d9b31990af1a9ed23c335"));

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1587567600, // * UNIX timestamp of last checkpoint block
    0,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of (0, uint256("0x00000a4799639e257149585a69c37e564c6d387ba043b27e44d33297e773fb4b"));

static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1587567601,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x000003e9b04fbd6e763652a05ae36181a8499d95fe1ca229ed55ad687493cb32"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1587567602,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params(bool useModulusV1) const
{
    assert(this);
    static CBigNum bnHexModulus = 0;
    if (!bnHexModulus)
        bnHexModulus.SetHex(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsHex = libzerocoin::ZerocoinParams(bnHexModulus);
    static CBigNum bnDecModulus = 0;
    if (!bnDecModulus)
        bnDecModulus.SetDec(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsDec = libzerocoin::ZerocoinParams(bnDecModulus);

    if (useModulusV1)
        return &ZCParamsHex;

    return &ZCParamsDec;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.powLimit   = ~uint256(0) >> 20; // YODA starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~uint256(0) >> 24;
        consensus.posLimitV2 = ~uint256(0) >> 20;
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMaxMoneyOut = 20000000010 * COIN;
        consensus.nStakeMinAge = 60 * 60;
        consensus.nStakeMinDepth = 600;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;
        consensus.nPremineAmount = 20000000 * COIN;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x85;
        pchMessageStart[1] = 0xc3;
        pchMessageStart[2] = 0xfc;
        pchMessageStart[3] = 0xe8;
        vAlertPubKey = ParseHex("0482795ca6249c51a3352c1c6ae5de72840f1722112007b1ee074af280549cbc17f23bb73efda159eec04d38b075804f13a49691505a195b6f1b6f5c04b5d4cef3");
        nDefaultPort = 51462;
        nMaxReorganizationDepth = 100;

       // Mainnet block.nTime = 1587927600
       // Mainnet block.nNonce = 5416048
       // Mainnet block.hashMerkleRoot: 06af04fe6d2a16e4203ea63fa0373fdba9c80641f62679d1087bb7743e435d19
       // Mainnet block.GetHash = 00000efafc5fb67134178c707a37ccedb5e9d22c8955b0324665470561d22284

        genesis = CreateGenesisBlock(1587927600, 5416048, 0x1e0ffff0, 1, 250 * COIN);

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256("0x00000efafc5fb67134178c707a37ccedb5e9d22c8955b0324665470561d22284"));
        assert(genesis.hashMerkleRoot == uint256("0x06af04fe6d2a16e4203ea63fa0373fdba9c80641f62679d1087bb7743e435d19"));

        nEnforceBlockUpgradeMajority = 8100; // 75%
        nRejectBlockOutdatedMajority = 10260; // 95%
        nToCheckBlockUpgradeMajority = 10800; // Approximate expected amount of blocks in 7 days (1440*7.5)
        nMinerThreads = 0;
        nMasternodeCountDrift = 20;
        nMinColdStakingAmount = 1 * COIN;

        // height based activations
        consensus.height_last_PoW = 1000;
        consensus.height_start_BIP65 = 1; // 82629b7a9978f5c7ea3f70a12db92633a7d2e436711500db28b97efd48b1e527
        consensus.height_start_StakeModifierV2 = 1001;
        consensus.height_start_TimeProtoV2 = 1001; // TimeProtocolV2, Blocks V7 and newMessageSignatures

        /** Height or Time Based Activations **/

        nZerocoinStartHeight = 2594451; // start real date by block
        nZerocoinStartTime = 1745366400; // October 17, 2017 4:30:00 AM

        nBlockRecalculateAccumulators = 0; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 0; //First block that bad serials emerged

        nBlockZerocoinV2 = 2594451; //

        // Public coin spend enforcement
        nPublicZCSpends = 0;

        // New P2P messages signatures
        nBlockEnforceNewMessageSignatures = 1;

        // Blocks v7
        nBlockLastAccumulatorCheckpoint = 1;
        nBlockV7StartHeight = 1;

        // Fake Serial Attack
        nFakeSerialBlockheightEnd = 0;
        nSupplyBeforeFakeSerial = 4131563 * COIN;   // zerocoin supply at block nFakeSerialBlockheightEnd

        // Note that of those with the service bits flag, most only support a subset of possible options
        //vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "yoda.seed.fuzzbawls.pw"));     // Primary DNS Seeder from Fuzzbawls
        //vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "yoda.seed2.fuzzbawls.pw"));    // Secondary DNS Seeder from Fuzzbawls

        //vSeeds.push_back(CDNSSeedData("warrows.dev", "dnsseed.yoda.warrows.dev"));    // Primery DNS Seeder from warrows


        vSeeds.push_back(CDNSSeedData("95.217.162.200", "95.217.162.200"));
        vSeeds.push_back(CDNSSeedData("95.217.162.201", "95.217.162.201"));
        vSeeds.push_back(CDNSSeedData("88.99.91.32", "88.99.91.32"));
        vSeeds.push_back(CDNSSeedData("88.99.91.31", "88.99.91.31"));
        vSeeds.push_back(CDNSSeedData("88.99.134.219", "88.99.134.219"));
        vSeeds.push_back(CDNSSeedData("178.63.235.141", "178.63.235.141"));
        vSeeds.push_back(CDNSSeedData("95.217.164.44", "95.217.164.44"));
        vSeeds.push_back(CDNSSeedData("95.216.11.80", "95.216.11.80"));
        vSeeds.push_back(CDNSSeedData("95.216.11.69", "95.216.11.69"));


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 30);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 13);
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 63);     // starting with 'S'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 212);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        nBudgetCycleBlocks = 43200; //!< Amount of blocks in a months period of time (using 1 minutes per) = (60*24*30)
        strSporkPubKey = "045645ce521c8080ad25d30b4b2bcc89c6054ebe5a385cba7139ab168e1f0ed6b1f575bf1786d9cfefe3899696440ff53d23443aa4510914aeaf1c7d170c7aeecd";
        strObfuscationPoolDummyAddress = "DDFWaSzbg2TyEu7yufbihAHevJEfVz5T6n";
        nStartMasternodePayments = 1403728576; //Wed, 25 Jun 2014 20:36:16 GMT

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMaxZerocoinPublicSpendsPerTransaction = 637; // Assume about 220 bytes each input
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 7; //Block headers must be this version once zerocoin is active
        nZerocoinRequiredStakeDepth = 200; //The required confirmations for a zpiv to be stakable

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
        nProposalEstablishmentTime = 60 * 60 * 24; // Proposals must be at least a day old to make it into a budget
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }

};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~uint256(0) >> 20; // YODA starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~uint256(0) >> 24;
        consensus.posLimitV2 = ~uint256(0) >> 20;
        consensus.nCoinbaseMaturity = 15;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMaxMoneyOut = 43199500 * COIN;
        consensus.nStakeMinAge = 60 * 60;
        consensus.nStakeMinDepth = 100;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;
        consensus.nPremineAmount = 10000000 * COIN;

        // height based activations
        consensus.height_last_PoW = 200;
        consensus.height_start_BIP65 = 851019;
        consensus.height_start_StakeModifierV2 = 1214000;
        consensus.height_start_TimeProtoV2 = 1347000; // TimeProtocolV2, Blocks V7 and newMessageSignatures

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */

        pchMessageStart[0] = 0x45;
        pchMessageStart[1] = 0x76;
        pchMessageStart[2] = 0x65;
        pchMessageStart[3] = 0xba;
        vAlertPubKey = ParseHex("04bc129e21a573811f598fda073b3f272491199843f6611d104c58f754de42b1ea934c483e6024069ecce5faa1e59f812f47921c470e1006f1345ad7a10ede1ef5");
        nDefaultPort = 51464;

        //Testnet block.nTime = 1587927601
        //Testnet block.nNonce = 5450020
        //Testnet block.hashMerkleRoot: 06af04fe6d2a16e4203ea63fa0373fdba9c80641f62679d1087bb7743e435d19
        //Testnet block.GetHash = 000000af6424c4c4fd2587d2809cd03b9bc01080b76f18e82c32f429cd3dd7b7

        genesis = CreateGenesisBlock(1587927601, 5450020, 0x1e0ffff0, 1, 250 * COIN);

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256("0x000000af6424c4c4fd2587d2809cd03b9bc01080b76f18e82c32f429cd3dd7b7"));
        assert(genesis.hashMerkleRoot == uint256("0x06af04fe6d2a16e4203ea63fa0373fdba9c80641f62679d1087bb7743e435d19"));

        nEnforceBlockUpgradeMajority = 4320; // 75%
        nRejectBlockOutdatedMajority = 5472; // 95%
        nToCheckBlockUpgradeMajority = 5760; // 4 days
        nMinerThreads = 0;
        nMasternodeCountDrift = 4;
        nZerocoinStartHeight = 201576;
        nZerocoinStartTime = 1587567661;
        nBlockRecalculateAccumulators = 9908000; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 9891737; //First block that bad serials emerged

        nBlockZerocoinV2 = 444020; //!> The block that zerocoin v2 becomes active

        // Public coin spend enforcement
        nPublicZCSpends = 1106100;

        // New P2P messages signatures
        nBlockEnforceNewMessageSignatures = consensus.height_start_TimeProtoV2;

        // Blocks v7
        nBlockLastAccumulatorCheckpoint = nPublicZCSpends - 10;
        nBlockV7StartHeight = consensus.height_start_TimeProtoV2;

        // Fake Serial Attack
        nFakeSerialBlockheightEnd = -1;
        nSupplyBeforeFakeSerial = 0;

        vFixedSeeds.clear();
        vSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "yoda-testnet.seed.fuzzbawls.pw"));
        //vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "yoda-testnet.seed2.fuzzbawls.pw"));
        //vSeeds.push_back(CDNSSeedData("warrows.dev", "testnet.dnsseed.yoda.warrows.dev"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet yoda addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet yoda script addresses start with '8' or '9'
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 73);     // starting with 'W'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet yoda BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet yoda BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet yoda BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        //convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        nBudgetCycleBlocks = 144; //!< Ten cycles per day on testnet
        strSporkPubKey = "0431a548d33cb81e96ce6c4756456a58fa98779943e7973529150bc3102402c79bec842a1a91fb90d5b8b9f2cc5c9fb551546126d5ec45af11ec4248099356877e";
        strObfuscationPoolDummyAddress = "y57cqfGRkekRyDRNeJiLtYVEbvhXrNbmox";
        nStartMasternodePayments = 1420837558; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet

        nProposalEstablishmentTime = 60 * 5; // Proposals must be at least 5 mns old to make it into a test budget
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~uint256(0) >> 20; // YODA starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~uint256(0) >> 24;
        consensus.posLimitV2 = ~uint256(0) >> 20;
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMaxMoneyOut = 43199500 * COIN;
        consensus.nStakeMinAge = 0;
        consensus.nStakeMinDepth = 0;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;
        consensus.nPremineAmount = 10000000 * COIN;

        // height based activations
        consensus.height_last_PoW = 250;
        consensus.height_start_BIP65 = 851019; // Not defined for regtest. Inherit TestNet value.
        consensus.height_start_StakeModifierV2 = consensus.height_last_PoW + 1; // start with modifier V2 on regtest
        consensus.height_start_TimeProtoV2 = 999999999;


        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */

        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0xac;
        nDefaultPort = 51466;

        //RegNet block.nTime = 1587927602
        //RegNet block.nNonce = 6469769
        //RegNet block.hashMerkleRoot: 06af04fe6d2a16e4203ea63fa0373fdba9c80641f62679d1087bb7743e435d19
        //RegNet block.GetHash = 00000d8f061636f64ad94e3be3f1f7ba421f6af1f18d289835c2a5b96fd3ac4e

        genesis = CreateGenesisBlock(1587927602, 6469769, 0x1e0ffff0, 1, 250 * COIN);

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256("0x00000d8f061636f64ad94e3be3f1f7ba421f6af1f18d289835c2a5b96fd3ac4e"));
        assert(genesis.hashMerkleRoot == uint256("0x06af04fe6d2a16e4203ea63fa0373fdba9c80641f62679d1087bb7743e435d19"));

        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nMasternodeCountDrift = 4;
        nZerocoinStartHeight = 300;
        nBlockZerocoinV2 = 300;
        nZerocoinStartTime = 1587567662;
        nBlockRecalculateAccumulators = 999999999;  // Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 999999999;          // First block that bad serials emerged

        nMintRequiredConfirmations = 10;
        nZerocoinRequiredStakeDepth = nMintRequiredConfirmations;

        // Public coin spend enforcement
        nPublicZCSpends = 400;

        // Blocks v7
        nBlockV7StartHeight = nBlockZerocoinV2;
        nBlockLastAccumulatorCheckpoint = nBlockZerocoinV2+1; // no accumul. checkpoints check on regtest

        // New P2P messages signatures
        nBlockEnforceNewMessageSignatures = 1;

        // Fake Serial Attack
        nFakeSerialBlockheightEnd = -1;

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        /* Spork Key for RegTest:
        WIF private key: 932HEevBSujW2ud7RfB1YF91AFygbBRQj3de3LyaCRqNzKKgWXi
        private key hex: bd4960dcbd9e7f2223f24e7164ecb6f1fe96fc3a416f5d3a830ba5720c84b8ca
        Address: yCvUVd72w7xpimf981m114FSFbmAmne7j9
        */
        strSporkPubKey = "0412c316af1e5331037aba297b98f5321a5d9eb03d79e8d19ef0c1871d3423349f870e75e6ffba15bfe6722c9d745b0bb6d5e2d5501d89eee9c9650ba267c40108";
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

static CChainParams* pCurrentParams = 0;

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
