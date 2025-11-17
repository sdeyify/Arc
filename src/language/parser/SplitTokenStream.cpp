#include "Parser.h"
#include "../util/imports.h"

std::vector<Arc::TokenStream> Arc::SplitTokenStream::getTokenStreams() const {
    return this->splitTokenStream;
}

Arc::TokenStream Arc::SplitTokenStream::at(const int index) const {
    return (index >= 0)
        ? this->splitTokenStream[index]
        : this->splitTokenStream[this->size() + index];
}

Arc::TokenStream Arc::SplitTokenStream::next() {
    const Arc::TokenStream tokenStream = this->splitTokenStream[0];
    this->splitTokenStream.erase(this->splitTokenStream.begin());
    return tokenStream;
}

Arc::TokenStream Arc::SplitTokenStream::join() const {
    Arc::TokenStream joined;

    for (const Arc::TokenStream& tokenStream : this->splitTokenStream) {
        joined = joined.concat(tokenStream);
    }

    return joined;
}

int Arc::SplitTokenStream::size() const {
    return this->splitTokenStream.size();
}

bool Arc::SplitTokenStream::empty() const {
    return this->splitTokenStream.empty();
}