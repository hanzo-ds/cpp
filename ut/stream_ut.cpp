#include <datastore/base/wire_format.h>
#include <datastore/base/output.h>
#include <datastore/base/input.h>

#include <gtest/gtest.h>

using namespace datastore;

TEST(CodedStreamCase, Varint64) {
    Buffer buf;

    {
        BufferOutput output(&buf);
        WireFormat::WriteVarint64(output, 18446744071965638648ULL);
        output.Flush();
    }

    {
        ArrayInput input(buf.data(), buf.size());
        uint64_t value = 0;
        ASSERT_TRUE(WireFormat::ReadVarint64(input, &value));
        ASSERT_EQ(value, 18446744071965638648ULL);
    }
}
