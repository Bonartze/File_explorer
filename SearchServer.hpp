#include <iostream>
#include <map>
#include <set>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <execution>

#pragma once
const size_t MAX_RESULT_DOCUMENT_COUNT = 5;

using std::string, std::vector, std::set, std::map, std::cin, std::cout, std::endl, std::multiset;

struct Document {
    int id;
    double relevance;
};

class SearchServer {
private:
    int document_count;
    set<string> stop_words;
    map<string, multiset<int>> word_to_documents;
    set<string> minus_words;
    set<string> plus_words;


private:
    double count_relevance(int);

    static vector<string> SplitIntoWords(const string &);

    void ParseStopWords(const string &);

    vector<string> SplitIntoWordsNoStop(const string &text);

    void AvoidMinusWords(vector<Document> &);

public:
    void SetDocumentCount(int);

    void DivideByWords(const string &);

    int GetDocumentCunt();

    void SetStopWords();

    vector<Document> FindAllDocuments(const string &query);

    void AddDocument(int, string &);

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


