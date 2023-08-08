#include "SearchServer.hpp"

void SearchServer::SetStopWords() {
    ParseStopWords(ReadLine());
}

void SearchServer::SetDocumentCount(int d_count) {
    document_count = d_count;
}

int SearchServer::GetDocumentCunt() {
    return document_count;
}

void SearchServer::AvoidMinusWords(vector<Document> &found_documents) {
    for (auto word: minus_words) {
        word = word.substr(1);
        for (auto &id: word_to_documents[word]) {
            found_documents.erase(
                    std::remove_if(found_documents.begin(), found_documents.end(), [&id](const Document &doc) {
                        return doc.id == id;
                    }), found_documents.end());
        }
    }
}

void SearchServer::DivideByWords(const std::string &query) {
    for (auto word: SplitIntoWords(query)) {
        if (!word.empty() && word.at(0) == '-')
            minus_words.insert(word);
        else
            plus_words.insert(word);
    }
}

vector<Document> SearchServer::FindAllDocuments(const string &query, vector<int> rates) {

    DivideByWords(query);
    map<int, double> document_to_relevance;
    for (const string &word: plus_words) {
        if (word_to_documents.count(word) == 0) {
            continue;
        }
        for (const int document_id: word_to_documents.at(word)) {
            ++document_to_relevance[document_id];
        }
    }
    vector<Document> found_documents;

    for (auto [document_id, relevance]: document_to_relevance) {
        if (!rates.empty())
            found_documents.push_back({document_id, relevance, rates[document_id]});
        else
            found_documents.push_back({document_id, relevance, 0});

    }
    if (!found_documents.empty())
        AvoidMinusWords(found_documents);
    sort(std::execution::par, found_documents.begin(), found_documents.end(), [](auto &val1, auto val2) {
        return std::tie(val1.relevance, val1.id) < std::tie(val2.relevance, val2.id);
    });
    plus_words.clear();
    minus_words.clear();
    return found_documents;
}

vector<Document> SearchServer::FindAllDocuments(const string &query, const vector<int> &rates, auto lambda) {
    vector<Document> res;
    for (auto &element: FindAllDocuments(query, rates)) {
        if (lambda(element.id, element.rate, element.relevance))
            res.push_back(element);
    }
    return res;
}


void SearchServer::AddDocument(int document_id, string document, DocumentStatus documentStatus) {
    for (const string &word: SplitIntoWordsNoStop(document)) {
        word_to_documents[word].insert(document_id);
        statuses.push_back(documentStatus);
    }
}

double SearchServer::count_relevance(int id) {
    map<string, double> idf;
    map<string, double> tf;
    for (auto &word: plus_words) {
        int count = 0;
        for (int i = 0; i < document_count; i++) {
            if (word_to_documents[word].contains(i))
                count++;
        }
        if (count != 0)
            idf[word] = log((double) document_count / count);
        else
            idf[word] = 0;
    }
    int count = std::count_if(word_to_documents.begin(), word_to_documents.end(), [&](const auto &word) {
        return word.second.contains(id);
    });
    for (auto word: plus_words) {
        if (word_to_documents[word].count(id) > 1)
            count += word_to_documents[word].count(id) - 1;
    }
    for (auto &word: plus_words) {
        int numerator = word_to_documents[word].count(id);
        if (word_to_documents[word].contains(id))
            tf[word] += (double) numerator / count;
        else
            tf[word] = 0;
    }

    double result = 0;
    for (auto [key, value]: idf)
        result += value * tf[key];
    return result;
}

// For each document returns its id and relevance

vector<Document> SearchServer::FindTopDocuments(const string &query) {
    DivideByWords(query);
    map<int, double> document_to_relevance;
    for (size_t i = 0; i < document_count; i++)
        document_to_relevance[i] = count_relevance(i);
    vector<Document> found_documents;
    for (auto [document_id, relevance]: document_to_relevance) {
        found_documents.push_back({document_id, relevance});
    }
    if (!found_documents.empty())
        AvoidMinusWords(found_documents);
    sort(std::execution::par, found_documents.begin(), found_documents.end(), [&](auto &val1, auto &val2) {
        return std::tie(val1.relevance, val1.id) > std::tie(val2.relevance, val2.id) &&
               statuses[val2.id] == DocumentStatus::ACTUAL;
    });
    plus_words.clear();
    minus_words.clear();
    return {found_documents.begin(), min(found_documents.begin() + MAX_RESULT_DOCUMENT_COUNT, found_documents.end())};
}

int SearchServer::GetRates() {
    size_t n;
    cin >> n;
    vector<int> rates(n);
    for (size_t i = 0; i < n; i++)
        cin >> rates[i];
    string s, s1;
    getline(cin, s);
    return ComputeAverageRating(rates);
}

int SearchServer::ComputeAverageRating(const vector<int> &ratings) {
    return std::accumulate(ratings.begin(), ratings.end(), 0) / (int) ratings.size();
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
        if (!word.empty() && word.at(0) == '-')
            minus_words.insert(word);
    }
    return words;
}

map<string, multiset<int>> SearchServer::getWordToDocument() {
    return word_to_documents;
}


set<string> SearchServer::GetMinusWords() {
    return minus_words;
}

set<string> SearchServer::GetStopWords() {
    return stop_words;
}

void SearchServer::SetStopWords(set<string> &s_w) {
    stop_words = s_w;
}