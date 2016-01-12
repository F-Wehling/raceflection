#include "GameObjectSpec.h"

#include <numeric>

BEGINNAMESPACE

const GameObjectSpec* GameObjectSpec::FromBuffer(const Byte* _buffer) {
    return (const GameObjectSpec*)_buffer;
}

void GameObjectSpec::ToBuffer(const GameObjectSpec * res, Byte * _buffer)
{
    *((GameObjectSpec*)_buffer) = *res;
}

uint32 GameObjectSpec::MemSize(const GameObjectSpec * spec)
{
    return sizeof(GameObjectSpec);
}

ENDNAMESPACE
