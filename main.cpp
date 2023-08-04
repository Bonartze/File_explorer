#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <execution>

using namespace std;

const size_t MAX_RESULT_DOCUMENT_COUNT = 5;
struct Document {
    int id;
    int relevance;
};

class SearchServer {
private:
    set<string> stop_words;
    map<string, set<int>> word_to_documents;

private:
    vector<string> SplitIntoWords(const string &);

    void ParseStopWords(const string &);

    vector<string> SplitIntoWordsNoStop(const string &text);

public:
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

void SearchServer::SetStopWords() {
    ParseStopWords(ReadLine());
}

vector<Document> SearchServer::FindAllDocuments(const string &query) {
    const vector<string> query_words = SplitIntoWordsNoStop(query);
    map<int, int> document_to_relevance;
    for (const string &word: query_words) {
        if (word_to_documents.count(word) == 0) {
            continue;
        }
        for (const int document_id: word_to_documents.at(word)) {
            ++document_to_relevance[document_id];
        }
    }

    vector<Document> found_documents;
    for (auto [document_id, relevance]: document_to_relevance) {
        found_documents.push_back({document_id, relevance});
    }
    sort(execution::par, found_documents.begin(), found_documents.end(), [](auto &val1, auto val2) {
        if (val1.relevance != val2.relevance)
            return val1.relevance > val2.relevance;
        return val1.id > val2.id;
    });
    return found_documents;
}

void SearchServer::AddDocument(int document_id, string &document) {
    for (const string &word: SplitIntoWordsNoStop(document)) {
        word_to_documents[word].insert(document_id);
    }
}

// For each document returns its id and relevance

vector<Document> SearchServer::FindTopDocuments(const string &query) {
    const vector<string> query_words = SplitIntoWordsNoStop(query);
    map<int, int> document_to_relevance;
    for (const string &word: query_words) {
        if (word_to_documents.count(word) == 0) {
            continue;
        }
        for (const int document_id: word_to_documents.at(word)) {
            ++document_to_relevance[document_id];
        }
    }

    vector<Document> found_documents;
    for (auto [document_id, relevance]: document_to_relevance) {
        found_documents.push_back({document_id, relevance});
    }
    sort(execution::par, found_documents.begin(), found_documents.end(), [](auto &val1, auto &val2) {
        if (val1.relevance != val2.relevance)
            return val1.relevance > val2.relevance;
        return val1.id > val2.id;

    });
    return {found_documents.begin(), found_documents.begin() + MAX_RESULT_DOCUMENT_COUNT};
}

vector<string> SearchServer::SplitIntoWords(const string &text) {
    vector<string> words;
    string word;
    for (const char c: text) {
        if (c == ' ') {
            words.push_back(word);
            word = "";
        } else {
            word += c;
        }
    }
    words.push_back(word);

    return words;
}

void SearchServer::ParseStopWords(const string &text) {
    for (const string &word: SplitIntoWords(text)) {
        stop_words.insert(word);
    }
    // return stop_words;
}

vector<string> SearchServer::SplitIntoWordsNoStop(const string &text) {
    vector<string> words;
    for (const string &word: SplitIntoWords(text)) {
        if (stop_words.count(word) == 0) {
            words.push_back(word);
        }
    }
    return words;
}


int main() {
    SearchServer ss;
    ss.SetStopWords();

    // Read documents
    const int document_count = SearchServer::ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        string s = SearchServer::ReadLine();
        ss.AddDocument(document_id, s);
    }

    const string query = SearchServer::ReadLine();
    for (auto [document_id, relevance]: ss.FindAllDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", relevance = "s << relevance << " }"s << endl;
    }
}