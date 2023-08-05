#include "SearchServer.hpp"


int main() {
    SearchServer ss;
    ss.SetStopWords();

    // Read documents
    ss.SetDocumentCount(SearchServer::ReadLineWithNumber());
    for (int document_id = 0; document_id < ss.GetDocumentCunt(); ++document_id) {
        string s = SearchServer::ReadLine();
        ss.AddDocument(document_id, s);
    }
    const string query = SearchServer::ReadLine();
    for (auto [document_id, relevance]: ss.FindAllDocuments(query)) {
        cout << "{ document_id = " << document_id << ", relevance = " << relevance << " }" << endl;
    }
}