#include "SearchServer.hpp"

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
    sort(std::execution::par, found_documents.begin(), found_documents.end(), [](auto &val1, auto val2) {
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
    sort(std::execution::par, found_documents.begin(), found_documents.end(), [](auto &val1, auto &val2) {
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