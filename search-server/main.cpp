#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <cmath>
#include <map>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document 
{
    int id;
    double relevance;
};

class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document) 
    {
        document_count_++;
        double size = 1./countWords(document);
        for(const string& s : SplitIntoWordsNoStop(document))
        {
                word_to_document_freqs_[s].insert({document_id, size});
        }
    }

    vector<Document> FindTopDocuments(const string& raw_query) const 
    {
        const set<string> query_words = ParseQuery(raw_query);
        const set<string> query_words_negative = ParseQueryNeg(raw_query);
        auto matched_documents = FindAllDocuments(query_words, query_words_negative);

        sort(matched_documents.begin(), matched_documents.end(), [](const Document& lhs, const Document& rhs) 
        {
            return lhs.relevance > rhs.relevance;
        });
        
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

private:

    int document_count_ = 0;
    
    map<string, map<int, double>> word_to_document_freqs_;

    set<string> stop_words_;
    
    set<string> negative_words_;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    int countWords(const string& text) const {
        return SplitIntoWordsNoStop(text).size();
    }
    
    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    set<string> ParseQuery(const string& text) const {
        set<string> query_words;
        for (const string& word : SplitIntoWordsNoStop(text)) 
        {
            if(word[0]!='-')
            query_words.insert(word);
        }
        return query_words;
    }
    set<string> ParseQueryNeg(const string& text) const {
        set<string> query_words;
        for (string& word : SplitIntoWordsNoStop(text)) 
        {
                if(word[0]=='-')
            {
                word.erase(0,1);
                query_words.insert(word);
            }
        }
        return query_words;
    }

    double CalculateRelevance(const double& tf, const string& word) const
    {
        double rel = tf * log(static_cast<double>(document_count_) / word_to_document_freqs_.at(word).size()); 
        return rel;
    }
    
    vector<Document> FindAllDocuments(const set<string>& query_words, const set<string>& query_words_negative) const 
    {
        map<int, double> document_to_relevance;
        vector<Document> matched_documents;
        for(const string& s : query_words)
        {
		//Going Through Each Document Mentioned In The Word s
            if(word_to_document_freqs_.count(s) > 0)
                for(auto& itr : word_to_document_freqs_.at(s))
                {
                 document_to_relevance[itr.first]+=calculateRelevance(itr.second, s);
                }
        }
        
        for(const string& s : query_words_negative)
        {
            if(word_to_document_freqs_.count(s) > 0)
                for(auto& itr : word_to_document_freqs_.at(s))
                {
                        document_to_relevance.erase(itr.first);
                }
        }
        
        for(const auto& document : document_to_relevance)
        {
            matched_documents.push_back({document.first, document.second});
        }
        
        return matched_documents;
    }
};

SearchServer CreateSearchServer() 
{
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
	
    for (int document_id = 0; document_id < document_count; ++document_id) 
    {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}

int main() 
{
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    
    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(query)) 
    {
        cout << "{ document_id = "s << document_id << ", "
             << "relevance = "s << relevance << " }"s << endl;
    }
}
