#include "request_queue.h"

    RequestQueue::RequestQueue(const SearchServer& search_server)
        :search_server_(search_server) {
    }

    std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) 
    {
        IncreaseTime();
        if (requests_.size() == 1440) {
            requests_.pop_front();
            if ((*requests_.begin()).isEmpty_)
                emptyQueries_--;
        }

        const auto search_results = search_server_.FindTopDocuments(raw_query, status);

        QueryResult temp = { search_results.empty(), search_results };

        requests_.push_back(temp);
        if (temp.isEmpty_)
            emptyQueries_++;

        return search_results;
    }

    std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) 
    {
        IncreaseTime();
        if (requests_.size() == 1440) {
            requests_.pop_front();
            if ((*requests_.begin()).isEmpty_)
                emptyQueries_--;
        }

        const auto search_results = search_server_.FindTopDocuments(raw_query);

        QueryResult temp = { search_results.empty(), search_results };

        requests_.push_back(temp);
        if (temp.isEmpty_)
            emptyQueries_++;

        return search_results;
    }

    int RequestQueue::GetNoResultRequests() const {

        return emptyQueries_;
    }

    void RequestQueue::IncreaseTime()
    {
        if (time < 1440)
            time++;
        else
            time = 0;
    }