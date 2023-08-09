#include "SearchServer.hpp"
#include "UnitTests.hpp"

int main() {
    UnitTesting::UnitTests us;
    us.RunAllTests();
    /*  SearchServer ss;
      ss.SetStopWords();

      // Read documents
      vector<int> rates;
      ss.SetDocumentCount(SearchServer::ReadLineWithNumber());
      for (int document_id = 0; document_id < ss.GetDocumentCunt(); ++document_id) {
          string s = SearchServer::ReadLine();
          rates.push_back(SearchServer::GetRates());
          ss.AddDocument(document_id, s, DocumentStatus::ACTUAL);
      }
      const string query = SearchServer::ReadLine();
      for (auto [document_id, relevance, rate]: ss.FindAllDocuments(query, rates)) {
          cout << "{ document_id = " << document_id << ", relevance = " << relevance << ", rating = " << rate << " }"
               << endl;
      }*/
}