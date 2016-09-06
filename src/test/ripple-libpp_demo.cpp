//------------------------------------------------------------------------------
/*
    This file is part of rippled: https://github.com/ripple/rippled
    Copyright (c) 2012-2016 Ripple Labs Inc.

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#include <ripple/protocol/digest.h>
#include <ripple/protocol/HashPrefix.h>
#include <ripple/protocol/JsonFields.h>
#include <ripple/protocol/st.h>
#include <ripple/protocol/TxFlags.h>
#include <ripple/basics/StringUtilities.h>
#include <ripple/json/to_string.h>
#include <boost/version.hpp>

std::string serialize(ripple::STTx const& tx)
{
    using namespace ripple;

    return strHex(tx.getSerializer().peekData());
}

std::shared_ptr<ripple::STTx const> deserialize(std::string blob)
{
    using namespace ripple;

    std::pair<Blob, bool> ret(strUnHex(blob));

    if (!ret.second || !ret.first.size())
        Throw<std::runtime_error>("transaction not valid hex");

    SerialIter sitTrans(makeSlice(ret.first));

    // Can Throw
    return std::make_shared<STTx const>(std::ref(sitTrans));
}

bool demonstrateSigning(ripple::KeyType keyType, std::string seedStr,
    std::string expectedAccount)
{
    using namespace ripple;

    auto const seed = parseGenericSeed(seedStr);
    assert(seed);
    auto const keypair = generateKeyPair(keyType, *seed);
    auto const id = calcAccountID(keypair.first);
    assert(toBase58(id) == expectedAccount);

    STTx noopTx(ttACCOUNT_SET,
        [&](auto& obj)
    {
        obj[sfAccount] = id;
        obj[sfFee] = STAmount{ 100 };
        obj[sfFlags] = tfFullyCanonicalSig;
        obj[sfSequence] = 18;
        obj[sfSigningPubKey] = keypair.first.slice();
    });

    std::cout << "\nBefore signing: \n" << noopTx << "\n"
        << "Serialized: " << noopTx.getJson(0, true)[jss::tx] << "\n";

    noopTx.sign(keypair.first, keypair.second);

    auto const serialized = serialize(noopTx);
    std::cout << "\nAfter signing: \n" << noopTx << "\n"
        << "Serialized: " << serialized << "\n";

    auto const deserialized = deserialize(serialized);
    assert(deserialized);
    assert(deserialized->getTransactionID() == noopTx.getTransactionID());
    std::cout << "Deserialized: " << *deserialized << "\n";

    auto const check1 = noopTx.checkSign(false);

    std::cout << "Check 1: " << (check1.first ? "Good" : "Bad!") << "\n";
    assert(check1.first);

    // Use the function primitives, which are hidden by the `STTx`
    // interface, to check the signature again and verify that
    // `STTx` is working properly.
    auto const& signatureSlice = noopTx[sfTxnSignature];
    Blob const data = [&]
    {
        // This is a copy of the static `getSigningData` function body
        // which is needed by `verify`.
        Serializer s;
        s.add32(HashPrefix::txSign);
        noopTx.addWithoutSigningFields(s);
        return s.getData();
    }();

    // STTx::checkSign calls `verify` indirectly via `checkSingleSign`
    auto const check2 = verify(
        keypair.first,
        makeSlice(data),
        signatureSlice,
        true);

    std::cout << "Check 2: " << (check2 ? "Good" : "Bad!") << "\n";

    return check1.first && check2;
}

// Must be outside the namespace for obvious reasons
//
int main (int argc, char** argv)
{
#if defined(__GNUC__) && !defined(__clang__)
    auto constexpr gccver = (__GNUC__ * 100 * 100) +
                            (__GNUC_MINOR__ * 100) +
                            __GNUC_PATCHLEVEL__;

    static_assert (gccver >= 50100,
        "GCC version 5.1.0 or later is required to compile rippled.");
#endif

    static_assert (BOOST_VERSION >= 105700,
        "Boost version 1.57 or later is required to compile rippled");

    auto const pass1 = demonstrateSigning(ripple::KeyType::secp256k1,
        "alice", "rG1QQv2nh2gr7RCZ1P8YYcBUKCCN633jCn");

    auto const pass2 = demonstrateSigning(ripple::KeyType::ed25519,
        "alice", "r9mC1zjD9u5SJXw56pdPhxoDSHaiNcisET");

    auto pass3 = false;
    try
    {
        auto const empty = deserialize("");
    }
    catch (std::runtime_error e)
    {
        pass3 = true;
        assert(e.what() == std::string("transaction not valid hex"));
    }

    assert(pass1 && pass2 && pass3);
    return pass1 && pass2 && pass3 ? 0 : 1;
}

