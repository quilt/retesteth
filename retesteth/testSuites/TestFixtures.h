/** @file TestFixtures.h
 * Test initialization classes
 */

#pragma once
#include "EthChecks.h"
#include "Options.h"
#include "TestOutputHelper.h"
using namespace std;

namespace test
{
static std::vector<std::string> const c_timeConsumingTestSuites{std::string{"stTimeConsuming"},
    std::string{"stQuadraticComplexityTest"}, std::string{"bcExploitTest"}, std::string{"bcExpectSection"},
    std::string{"bcWalletTest"}, std::string{"stQuadraticComplexityTest"}};

enum class TestExecution
{
    RequireOptionAll,
    NotRefillable
};

class NotRefillable
{
public:
    NotRefillable() {}
    std::set<TestExecution> getFlags() { return {TestExecution::NotRefillable}; }
};

class RequireOptionAll
{
public:
    RequireOptionAll() {}
    std::set<TestExecution> getFlags() { return {TestExecution::RequireOptionAll}; }
};

class DefaultFlags
{
public:
    DefaultFlags() {}
    std::set<TestExecution> getFlags() { return std::set<TestExecution>(); }
};

// what if U has the information about flags
template <class T, class U>
class TestFixture
{
public:
    TestFixture(std::set<TestExecution> const& _execFlags = {})
    {
        T suite;
        U defaultBoostFlags;
        std::set<TestExecution> allFlags = _execFlags;
        for (auto const& el : defaultBoostFlags.getFlags())
            allFlags.emplace(el);

        if (allFlags.count(TestExecution::NotRefillable) &&
            (Options::get().fillchain || Options::get().filltests))
            ETH_ERROR_MESSAGE("Tests are sealed and not refillable!");

        string const casename = boost::unit_test::framework::current_test_case().p_name;
        boost::filesystem::path const suiteFillerPath =
            suite.getFullPathFiller(casename).parent_path();

        // skip wallet test as it takes too much time (250 blocks) run it with --all flag
        if ((inArray(c_timeConsumingTestSuites, casename) || allFlags.count(TestExecution::RequireOptionAll))
             && !test::Options::get().all)
        {
            std::cout << "Skipping " << casename << " because --all option is not specified.\n";
            test::TestOutputHelper::get().markTestFolderAsFinished(suiteFillerPath, casename);
            return;
        }

        suite.runAllTestsInFolder(casename);
        test::TestOutputHelper::get().markTestFolderAsFinished(suiteFillerPath, casename);
    }
};


}  // namespace test
