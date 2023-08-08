#include <iostream>
#include <map>
#include <set>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <execution>
#include <functional>

#pragma once
const size_t MAX_RESULT_DOCUMENT_COUNT = 5;

using std::string, std::vector, std::set, std::map, std::cin, std::cout, std::endl, std::multiset, std::cerr;

enum class DocumentStatus {
    ACTUAL, IRRELEVANT, BANNED, REMOVED
};

struct Document {
    int id;
    double relevance;
    int rate;

    bool operator==(Document document) const {
        return id == document.id && relevance == document.relevance && rate == document.rate;
    }
};


class SearchServer {
private:
    int document_count;
    vector<DocumentStatus> statuses;
    set<string> stop_words;
    map<string, multiset<int>> word_to_documents;
    set<string> minus_words;
    set<string> plus_words;


private:
    double count_relevance(int);

    static vector<string> SplitIntoWords(const string &);

    void ParseStopWords(const string &);


    void AvoidMinusWords(vector<Document> &);

public:

    vector<string> SplitIntoWordsNoStop(const string &text);


    set<string> GetMinusWords();

    static int GetRates();

    map<string, multiset<int>> getWordToDocument();

    static int ComputeAverageRating(const vector<int> &);

    void SetDocumentCount(int);

    void DivideByWords(const string &);

    int GetDocumentCunt();

    void SetStopWords();

    void SetStopWords(set<string> &);

    set<string> GetStopWords();

    vector<Document> FindAllDocuments(const string &, vector<int>);

    vector<Document> FindAllDocuments(const string &, const vector<int> &, auto);

    void AddDocument(int, string, DocumentStatus documentStatus);

// For each document returns its id and relevance

    vector<Document> FindTopDocuments(const string &query);

    static string ReadLine() {
        string s;
        getline(cin, s);
        return s;
    }

    static int ReadLineWithNumber() {
        int result;
        cin >> result;
        ReadLine();
        return result;
    }
};


