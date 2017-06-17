//------------------------------------------------------------------------------
/*
    This file is part of ripple-libpp:
        https://github.com/ripple/ripple-libpp
    Copyright (c) 2017 Ripple Labs Inc.
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

#include <BeastConfig.h>
#include <ripple/beast/unit_test.h>
#include <ripple/core/Config.h>
#include <ripple/protocol/BuildInfo.h>
#include <beast/unit_test/dstream.hpp>
#include <boost/program_options.hpp>
#include <test/quiet_reporter.h>

namespace po = boost::program_options;

namespace ripple {
//------------------------------------------------------------------------------
//

static int runUnitTests(
    std::string const& pattern,
    std::string const& argument,
    bool quiet,
    bool log)
{
    using namespace beast::unit_test;
    using namespace ripple::test;
    beast::unit_test::dstream dout{std::cout};

    std::unique_ptr<runner> r;
    if(quiet)
        r = std::make_unique<quiet_reporter>(dout, log);
    else
        r = std::make_unique<reporter>(dout);
    r->arg(argument);
    bool const anyFailed = r->run_each_if(
        global_suites(), match_auto(pattern));
    if(anyFailed)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

void printHelp (const po::options_description& desc)
{
    std::cerr
        << systemName () << "d [options] <command> <params>\n"
        << desc << std::endl;
}

int run (int argc, char** argv)
{
    using namespace std;

    po::variables_map vm;

    // Set up option parsing.
    po::options_description desc ("General Options");
    desc.add_options ()
    ("help,h", "Display this message.")
    ("unittest,u", po::value <std::string> ()->implicit_value (""), "Perform unit tests.")
    ("unittest-arg", po::value <std::string> ()->implicit_value (""), "Supplies argument to unit tests.")
    ("unittest-log", po::value <std::string> ()->implicit_value (""), "Force unit test log output, even in quiet mode.")
    ("parameters", po::value< vector<string> > (), "Specify comma separated parameters.")
    ("quiet,q", "Reduce diagnotics.")
    ("version", "Display the build version.")
    ;

    // Interpret positional arguments as --parameters.
    po::positional_options_description p;
    p.add ("parameters", -1);

    // Parse options, if no error.
    try
    {
        po::store (po::command_line_parser (argc, argv)
            .options (desc)               // Parse options.
            .positional (p)               // Remainder as --parameters.
            .run (),
            vm);
        po::notify (vm);                  // Invoke option notify functions.
    }
    catch (std::exception const&)
    {
        std::cerr << "rippled: Incorrect command line syntax." << std::endl;
        std::cerr << "Use '--help' for a list of options." << std::endl;
        return 1;
    }

    if (vm.count ("help"))
    {
        printHelp (desc);
        return 0;
    }

    if (vm.count ("version"))
    {
        std::cout << "ripple-libpp version " <<
            BuildInfo::getVersionString () << std::endl;
        return 0;
    }

    // Run the unit tests if requested.
    // The unit tests will exit the application with an appropriate return code.
    //
    if (vm.count ("unittest"))
    {
        std::string argument;

        if (vm.count("unittest-arg"))
            argument = vm["unittest-arg"].as<std::string>();
        return runUnitTests(
            vm["unittest"].as<std::string>(), argument,
            static_cast<bool> (vm.count ("quiet")),
            static_cast<bool> (vm.count ("unittest-log")));
    }

    if (!vm.count ("parameters"))
    {
        runUnitTests(
            "", "",
            static_cast<bool> (vm.count ("quiet")),
            static_cast<bool> (vm.count ("unittest-log")));
        return 0;
    }
}

} // ripple

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
   
    auto const result (ripple::run (argc, argv));
    return result;
}

