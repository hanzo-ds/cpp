#pragma once

#include <datastore/columns/column.h>
#include <memory>

namespace datastore {
    class Client;
}

datastore::ColumnRef RoundtripColumnValues(datastore::Client& client, datastore::ColumnRef expected);

template <typename T>
auto RoundtripColumnValuesTyped(datastore::Client& client, std::shared_ptr<T> expected_col)
{
    return RoundtripColumnValues(client, expected_col)->template As<T>();
}
