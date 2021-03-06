#pragma once
#include <ethObjects/common.h>

typedef std::vector<test::scheme_RPCBlock> vectorOfSchemeBlock;
class TestBlock
{
public:
    void setNextBlockForked(test::scheme_RPCBlock const& _next)
    {
        m_nextBlockForked.push_back(_next);
    }
    test::scheme_RPCBlock const& getNextBlockForked() const
    {
        assert(m_nextBlockForked.size() > 0);
        return m_nextBlockForked.at(0);
    }
    DataObject& getDataForTestUnsafe() { return m_dataForTest; }
    DataObject const& getDataForTest() const { return m_dataForTest; }
    string const& getRLP() const { return m_dataForTest.atKey("rlp").asString(); }
    void addUncle(test::scheme_RPCBlock const& _uncle) { m_uncles.push_back(_uncle); }
    std::vector<test::scheme_RPCBlock> const& getUncles() const { return m_uncles; }
    void setDoNotExport(bool _doNotExport) { m_doNotExport = _doNotExport; }
    bool isDoNotExport() const { return m_doNotExport; }

private:
    DataObject m_dataForTest;

    // A block mined in paralel representing an uncle (it has the same blocknumber)
    std::vector<test::scheme_RPCBlock> m_nextBlockForked;  // Hypothetical next block for uncles
    std::vector<test::scheme_RPCBlock> m_uncles;
    bool m_doNotExport;
};
