#include "readonly_client_test.h"
#include "utils.h"

#include <datastore/columns/column.h>
#include <datastore/block.h>

#include <iostream>

namespace {
    using namespace datastore;
}

void ReadonlyClientTest::SetUp() {
    const auto & options = std::get<0>(GetParam());
    if (options.host.empty() && options.endpoints.empty())
        GTEST_SKIP() << "No server configured: set the corresponding *_HOST environment variable.";

    client_ = std::make_unique<Client>(options);
}

void ReadonlyClientTest::TearDown() {
    client_.reset();
}

// Sometimes gtest fails to detect that this test is instantiated elsewhere, suppress the error explicitly.
GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(ReadonlyClientTest);
TEST_P(ReadonlyClientTest, Select) {

    const auto & queries = std::get<1>(GetParam());
    for (const auto & query : queries) {
        client_->Select(query,
            [& query](const Block& block) {
                if (block.GetRowCount() == 0 || block.GetColumnCount() == 0)
                    return;
                std::cout << query << " => "
                          << "\n\trows: " << block.GetRowCount()
                          << ", columns: " << block.GetColumnCount()
                          << ", data:\n" << PrettyPrintBlock{block} << std::endl;
            }
        );
    }
}
