#pragma once
#include "../util/imports.h"
#include "TokenStream.h"

namespace Arc
{
    class TokenStream;

    class SplitTokenStream
    {
    public:
        SplitTokenStream(const std::vector<Arc::TokenStream>& splitTokenStream) : splitTokenStream(splitTokenStream) {}
    private:
        std::vector<Arc::TokenStream> splitTokenStream;
    public:
        std::vector<Arc::TokenStream> getTokenStreams() const;
        Arc::TokenStream at(const int index) const;
        Arc::TokenStream join() const;
        Arc::TokenStream next();
        int size() const;
        bool empty() const;

        template<typename T>
        std::vector<T> transform(const std::function<T(Arc::TokenStream&)>& fn) {
            std::vector<T> mapped;

            for (auto& tokenStream : this->splitTokenStream) {
                mapped.push_back(fn(tokenStream));
            }

            return mapped;
        }
    };
}