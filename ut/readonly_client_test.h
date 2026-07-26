#pragma once

#include <datastore/client.h>

#include <gtest/gtest.h>

#include <string>
#include <tuple>
#include <vector>

class ReadonlyClientTest : public testing::TestWithParam<
        std::tuple<datastore::ClientOptions, std::vector<std::string> > /*queries*/> {
protected:
    void SetUp() override;
    void TearDown() override;

    std::unique_ptr<datastore::Client> client_;
};
