// ../cli.js --schema test-schema.kiwi --cpp test-schema.h && c++ test.cpp -std=c++11 -I.. && ./a.out

#include "test-schema.h"
#include <vector>

void testByte() {
  puts("testByte");

  auto check = [](uint8_t i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::ByteStructEncoder(bb).add_x(i);
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    uint8_t value = 0;
    bool success = test_cpp::ByteStructDecoder(bb2).read_x(value);
    assert(success);
    assert(value == i);
  };

  check(0x00, {0x00});
  check(0x01, {0x01});
  check(0x7F, {0x7F});
  check(0x80, {0x80});
  check(0xFF, {0xFF});
}

void testUint() {
  puts("testUint");

  auto check = [](uint32_t i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::UintStructEncoder(bb).add_x(i);
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    uint32_t value = 0;
    bool success = test_cpp::UintStructDecoder(bb2).read_x(value);
    assert(success);
    assert(value == i);
  };

  check(0x00, {0x00});
  check(0x01, {0x01});
  check(0x02, {0x02});
  check(0x7F, {0x7F});
  check(0x80, {0x80, 0x01});
  check(0x81, {0x81, 0x01});
  check(0x100, {0x80, 0x02});
  check(0x101, {0x81, 0x02});
  check(0x17F, {0xFF, 0x02});
  check(0x180, {0x80, 0x03});
  check(0x1FF, {0xFF, 0x03});
  check(0x200, {0x80, 0x04});
  check(0x7FFF, {0xFF, 0xFF, 0x01});
  check(0x8000, {0x80, 0x80, 0x02});
  check(0x7FFFFFFF, {0xFF, 0xFF, 0xFF, 0xFF, 0x07});
  check(0x80000000, {0x80, 0x80, 0x80, 0x80, 0x08});
}

void testInt() {
  puts("testInt");

  auto check = [](int32_t i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::IntStructEncoder(bb).add_x(i);
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    int32_t value = 0;
    bool success = test_cpp::IntStructDecoder(bb2).read_x(value);
    assert(success);
    assert(value == i);
  };

  check(0x00, {0x00});
  check(-0x01, {0x01});
  check(0x01, {0x02});
  check(-0x02, {0x03});
  check(0x02, {0x04});
  check(-0x3F, {0x7D});
  check(0x3F, {0x7E});
  check(-0x40, {0x7F});
  check(0x40, {0x80, 0x01});
  check(-0x3FFF, {0xFD, 0xFF, 0x01});
  check(0x3FFF, {0xFE, 0xFF, 0x01});
  check(-0x4000, {0xFF, 0xFF, 0x01});
  check(0x4000, {0x80, 0x80, 0x02});
  check(-0x3FFFFFFF, {0xFD, 0xFF, 0xFF, 0xFF, 0x07});
  check(0x3FFFFFFF, {0xFE, 0xFF, 0xFF, 0xFF, 0x07});
  check(-0x40000000, {0xFF, 0xFF, 0xFF, 0xFF, 0x07});
  check(0x40000000, {0x80, 0x80, 0x80, 0x80, 0x08});
  check(-0x7FFFFFFF, {0xFD, 0xFF, 0xFF, 0xFF, 0x0F});
  check(0x7FFFFFFF, {0xFE, 0xFF, 0xFF, 0xFF, 0x0F});
  check(-0x80000000, {0xFF, 0xFF, 0xFF, 0xFF, 0x0F});
}

void testFloat() {
  puts("testFloat");

  auto check = [](float i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::FloatStructEncoder(bb).add_x(i);
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    float value = 0;
    bool success = test_cpp::FloatStructDecoder(bb2).read_x(value);
    assert(success);
    assert(value == i);
  };

  check(0, {0, 0, 0, 0});
  check(1, {0, 0, 128, 63});
  check(-1, {0, 0, 128, 191});
  check(3.1415927410125732, {219, 15, 73, 64});
  check(-3.1415927410125732, {219, 15, 73, 192});
}

void testString() {
  puts("testString");

  auto check = [](std::string i, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::StringStructEncoder(bb).add_x(i);
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    std::string value;
    bool success = test_cpp::StringStructDecoder(bb2).read_x(value);
    assert(success);
    assert(value == i);
  };

  check("", {0});
  check("abc", {3, 97, 98, 99});
  check({"\0abc\0", 5}, {5, 0, 97, 98, 99, 0});
  check("🙉🙈🙊", {12, 240, 159, 153, 137, 240, 159, 153, 136, 240, 159, 153, 138});
}

void testCompound() {
  puts("testCompound");

  auto check = [](int32_t x, int32_t y, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::CompoundStructEncoder(bb).add_x(x).add_y(y);
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test_cpp::CompoundStructDecoder decoder(bb2);
    int32_t value_x = 0;
    int32_t value_y = 0;
    bool success_x = decoder.read_x(value_x);
    bool success_y = decoder.read_y(value_y);
    assert(success_x);
    assert(success_y);
    assert(value_x == x);
    assert(value_y == y);
  };

  check(0, 0, {0, 0});
  check(1, -1, {2, 1});
  check(12345, 54321, {242, 192, 1, 226, 208, 6});
}

void testNested() {
  puts("testNested");

  auto check = [](int32_t a, int32_t bx, int32_t by, int32_t c, std::vector<uint8_t> o) {
    kiwi::ByteBuffer bb;
    test_cpp::NestedStructEncoder encoder(bb);
    encoder.add_a(a);
    encoder.add_b().add_x(bx).add_y(by);
    encoder.add_c(c);
    assert(std::vector<uint8_t>(bb.data(), bb.data() + bb.size()) == o);

    kiwi::ByteBuffer bb2(o.data(), o.size());
    test_cpp::NestedStructDecoder decoder(bb2);
    int32_t value_a = 0;
    int32_t value_bx = 0;
    int32_t value_by = 0;
    int32_t value_c = 0;
    bool success_a = decoder.read_a(value_a);
    test_cpp::CompoundStructDecoder decoder2;
    bool success_b = decoder.read_b(decoder2);
    bool success_bx = decoder2.read_x(value_bx);
    bool success_by = decoder2.read_y(value_by);
    bool success_c = decoder.read_c(value_c);

    assert(success_a);
    assert(success_b);
    assert(success_bx);
    assert(success_by);
    assert(success_c);
    assert(value_a == a);
    assert(value_bx == bx);
    assert(value_by == by);
    assert(value_c == c);
  };

  check(0, 0, 0, 0, {0, 0, 0, 0});
  check(1, -2, 3, -4, {2, 3, 6, 7});
  check(534, 12345, 54321, 321, {172, 8, 242, 192, 1, 226, 208, 6, 130, 5});
}

int main() {
  testByte();
  testUint();
  testInt();
  testFloat();
  testString();
  testCompound();
  testNested();
  puts("all tests passed");
  return 0;
}
