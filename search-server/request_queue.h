#pragma once

#include "search_server.h"
#include <deque>

class RequestQueue {
public:
    RequestQueue(const SearchServer& search_server);
    
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate);
    
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& raw_query);
    
    int GetNoResultRequests() const;
    void IncreaseTime();
    
private:
    struct QueryResult {
        bool isEmpty_ = true;
        std::vector<Document> result;
    };
    std::deque<QueryResult> requests_;
    int time = 0;
    const static int min_in_day_ = 1440;
    int emptyQueries_ = 0;
    const SearchServer& search_server_;
};

template <typename DocumentPredicate>
std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
        IncreaseTime();
        if (requests_.size() == 1400) {
            requests_.pop_front();
            if ((*requests_.begin()).isEmpty_)
                emptyQueries_--;
        }

        const auto search_results = search_server_.FindTopDocuments(raw_query, document_predicate);

        QueryResult temp = { search_results.empty(), search_results };

        requests_.push_back(temp);
        if (temp.isEmpty_)
            emptyQueries_++;

        return search_results;
    }