#include "blake.h"
#include <blake3.h>

namespace wk::hash {
    Blake256::Blake256() {
        clear();
    }

    Blake256Digest Blake256::digest() const {
        Blake256Digest digest {};
        blake3_hasher_finalize(&m_context, digest.data(), BLAKE3_OUT_LEN);
        return digest;
    }

    void Blake256::update_hash(const uint8_t* data, size_t length) {
        blake3_hasher_update(&m_context, data, length);
    }

    void Blake256::clear() {
        m_context = blake3_hasher{};
        blake3_hasher_init(&m_context);
    }
}