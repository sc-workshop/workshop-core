#pragma once

#include "core/hashing/hash_stream.h"
#include "core/memory/ref.h"

#include <array>

#ifndef WK_BLAKE3
    #error "blake3 is not included"
#endif

#include <blake3.h>

namespace wk::hash {
    using Blake256Digest = std::array<uint8_t, 32>;
    using Blake512Digest = std::array<uint8_t, 64>;

    class Blake256 : public HashStream<Blake256Digest> {
    public:
        Blake256();

    public:
        virtual Digest digest() const;
        virtual void clear();

    protected:
        virtual void update_hash(const uint8_t* data, size_t length);

    private:
        blake3_hasher m_context;
    };
}
