#pragma once

#include <Windows.h>
#include <cstddef>
#include <string>
#include <type_traits>

namespace ipc::data {

constexpr std::size_t commandSize = 32768;

struct Data {
    Data() = default;
    Data(DWORD processId, const std::wstring& command);

    DWORD processId = 0;
    wchar_t command[commandSize] = {};
};

static_assert(std::is_trivially_copyable_v<Data>);

inline constexpr DWORD dataSize = static_cast<DWORD>(sizeof(Data));

} // namespace ipc::data
