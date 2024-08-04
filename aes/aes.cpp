#include "aes.hpp"
#include "aes_impl.hpp"

template
class Aes<AesVariant::Aes128>;

template
class Aes<AesVariant::Aes192>;

template
class Aes<AesVariant::Aes256>;
