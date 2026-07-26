#pragma once

#include "column.h"

namespace datastore {

struct CreateColumnByTypeSettings
{
    bool low_cardinality_as_wrapped_column = false;
};

ColumnRef CreateColumnByType(const std::string& type_name, CreateColumnByTypeSettings settings = {});

}
