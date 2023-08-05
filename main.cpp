#include "SearchServer.hpp"


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
    for (auto [document_id, relevance]: ss.FindTopDocuments(query)) {
        cout << "{ document_id = " << document_id << ", relevance = " << relevance << " }" << endl;
    }
}