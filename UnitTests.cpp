#include "UnitTests.hpp"
#include <cassert>

namespace UnitTesting {
    UnitTests::UnitTests() : ss(SearchServer()) {
    }

    bool UnitTests::TestAddDocument() {
        ss.SetDocumentCount(3);
        ss.AddDocument(0, "Just test", DocumentStatus::ACTUAL);
        ss.AddDocument(1, "", DocumentStatus::BANNED);
        ss.AddDocument(2, "Testing", DocumentStatus::IRRELEVANT);
        map<string, multiset<int>> test = {{"",        {1}},
                                           {"Just",    {0}},
                                           {"Testing", {2}},
                                           {"test",    {0}}};
        assert(ss.getWordToDocument() == test);
        cerr << "Test TestAddDocument - OK\n";
    }

    bool UnitTests::TestFindAll() {
        vector<Document> test1 = {{0, 1, 0}};
        vector<Document> test2 = {{2, 1, 0}};
        vector<Document> test3 = {{1, 1, 0}};
        assert(ss.FindAllDocuments("Just Test", {}) == test1);
        assert(ss.FindAllDocuments("Testing", {}) == test2);
        assert(ss.FindAllDocuments("", {}) == test3);
        cerr << "Test TestFindAllDocuments - OK\n";
    }

    bool UnitTests::TestFindTop() {
        vector<Document> test = {{2, 1.0986122886681098, 0},
                                 {1, 0,                  0},
                                 {0, 0,                  0}};
        auto t = ss.FindTopDocuments("Testing");
        assert(t[0].id == test[0].id &&
               t[0].rate == test[0].rate);
        assert(t[0].relevance > test[0].relevance - 0.1 &&
               test[0].relevance + 0.1 > t[0].relevance);
        assert(t[1] == test[1]);
        assert(t[2] == test[2]);
        cerr << "Test TestFindTop - OK\n";
    }

    bool UnitTests::TestMinusWords() {
        ss.SplitIntoWordsNoStop("query type new -minus");
        ss.SplitIntoWordsNoStop("dark -way -out hi");
        set<string> test_minus_words = {"-minus", "-way", "-out"};
        assert(ss.GetMinusWords() == test_minus_words);
        cerr << "Test MinusWords - OK\n";
    }

    void UnitTests::RunAllTests() {
        TestAddDocument();
        TestFindAll();
        TestFindTop();
        TestMinusWords();
        cerr << "All tests're passed\n";
    }
}