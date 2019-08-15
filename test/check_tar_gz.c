#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <starch.h>

#include <archive_entry.h>

uint8_t SINGLE_FILE_TAR_GZ[] = {
    0x1f, 0x8b, 0x08, 0x00, 0x56, 0x96, 0x55, 0x5d, 0x00, 0x03, 0xcb, 0x48,
    0xcd, 0xc9, 0xc9, 0xd7, 0x2d, 0xcf, 0x2f, 0xca, 0x49, 0xd1, 0x2b, 0xa9,
    0x28, 0x61, 0xa0, 0x05, 0x30, 0x30, 0x30, 0x30, 0x33, 0x31, 0x51, 0x00,
    0xd2, 0x86, 0xe6, 0xa6, 0x46, 0x50, 0xda, 0x10, 0x44, 0x43, 0x81, 0xa1,
    0xa9, 0x82, 0xa1, 0xb1, 0xa9, 0x91, 0xa9, 0xb1, 0xa1, 0xb1, 0x81, 0x89,
    0xa9, 0x82, 0x81, 0xa1, 0x99, 0x81, 0xb9, 0x09, 0x83, 0x82, 0x01, 0x4d,
    0x5c, 0x83, 0x06, 0x4a, 0x8b, 0x4b, 0x12, 0x8b, 0x80, 0x4e, 0xc9, 0xca,
    0xcf, 0x4b, 0x2c, 0xc9, 0x48, 0xcc, 0xcb, 0xc8, 0xcf, 0xc8, 0x49, 0xc5,
    0xaa, 0x2e, 0xb5, 0xa8, 0x18, 0x8f, 0x39, 0x10, 0x9f, 0x28, 0xc0, 0xe9,
    0x21, 0x02, 0x3c, 0x40, 0xf1, 0xaf, 0xa3, 0x10, 0x0e, 0x4a, 0x00, 0x8a,
    0x03, 0xed, 0x98, 0x51, 0x30, 0x0a, 0x46, 0xc1, 0x28, 0x18, 0x05, 0x74,
    0x03, 0x00, 0x36, 0x60, 0x0d, 0x3f, 0x00, 0x08, 0x00, 0x00,
};
size_t SINGLE_FILE_TAR_GZ_SIZE = sizeof(SINGLE_FILE_TAR_GZ);

START_TEST(test_single_chunk) {
    fprintf(stderr, "baz\n");

    struct archive* a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);

    StarchStream* str = starch_init(a);
    ck_assert_msg(!starch_has_entry(str), "Don't expect a record until feeding");

    starch_feed(str, SINGLE_FILE_TAR_GZ, SINGLE_FILE_TAR_GZ_SIZE);
    ck_assert_msg(!starch_has_entry(str), "finishing");
    starch_finish(str);
    ck_assert_msg(starch_has_entry(str), "Expected a record to be ready");

    struct archive_entry* entry = starch_next_entry(str);
    ck_assert_ptr_nonnull(entry); 

    ck_assert_str_eq("hello-world.txt", archive_entry_pathname(entry));

    uint8_t buff[1024] = { 0 };
    ssize_t len = starch_read(str, buff, 1024);
    ck_assert_int_eq(13, len);
    ck_assert_str_eq("Hello, World!", (char*) buff);

    ck_assert_msg(!starch_has_entry(str), "Expected only a single entry");

    starch_finish(str);

    archive_read_free(a);

    // test single chunk
} END_TEST

START_TEST(test_multi_chunk) {
    fprintf(stderr, "baz\n");

    struct archive* a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);

    StarchStream* str = starch_init(a);
    ck_assert_msg(!starch_has_entry(str), "Don't expect a record until feeding");

    size_t half = SINGLE_FILE_TAR_GZ_SIZE / 2;
    starch_feed(str, SINGLE_FILE_TAR_GZ, half);
    ck_assert_msg(!starch_has_entry(str), "finishing");
    starch_feed(str, SINGLE_FILE_TAR_GZ + half, SINGLE_FILE_TAR_GZ_SIZE - half);
    ck_assert_msg(!starch_has_entry(str), "finishing");
    starch_finish(str);
    ck_assert_msg(starch_has_entry(str), "Expected a record to be ready");

    struct archive_entry* entry = starch_next_entry(str);
    ck_assert_ptr_nonnull(entry);

    ck_assert_str_eq("hello-world.txt", archive_entry_pathname(entry));

    uint8_t buff[1024] = { 0 };
    ssize_t len = starch_read(str, buff, 1024);
    ck_assert_int_eq(13, len);
    ck_assert_str_eq("Hello, World!", (char*) buff);

    ck_assert_msg(!starch_has_entry(str), "Expected only a single entry");

    starch_finish(str);

    archive_read_free(a);
} END_TEST

START_TEST(test_underflow) {
    fprintf(stderr, "foo\n");
    // test underflow
} END_TEST

Suite* tar_gz_suite() {
    TCase* tc_core = tcase_create("core");
    tcase_add_test(tc_core, test_single_chunk);
    tcase_add_test(tc_core, test_multi_chunk);
    tcase_add_test(tc_core, test_underflow);

    Suite* s = suite_create("tar.gz Tests");
    suite_add_tcase(s, tc_core);

    return s;
}

int main(int argc, char* argv[]) {
    int failed = 0;

    SRunner* sr = srunner_create(tar_gz_suite());
    srunner_run_all(sr, CK_NORMAL);

    failed = srunner_ntests_failed(sr);

    srunner_free(sr);

    return failed == 0
        ? EXIT_SUCCESS
        : EXIT_FAILURE;
}
