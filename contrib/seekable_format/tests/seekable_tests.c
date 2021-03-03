#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "zstd_seekable.h"

/* Basic unit tests for zstd seekable format */
int main(int argc, const char** argv)
{
    unsigned testNb = 1;
    (void)argc; (void)argv;
    printf("Beginning zstd seekable format tests...\n");
    printf("Test %u - check that seekable decompress does not hang: ", testNb++);
    {   /* Github issue #2335 */
        const size_t compressed_size = 17;
        const uint8_t compressed_data[17] = {
            '^',
            '*',
            'M',
            '\x18',
            '\t',
            '\x00',
            '\x00',
            '\x00',
            '\x00',
            '\x00',
            '\x00',
            '\x00',
            ';',
            (uint8_t)('\xb1'),
            (uint8_t)('\xea'),
            (uint8_t)('\x92'),
            (uint8_t)('\x8f'),
        };
        const size_t uncompressed_size = 32;
        uint8_t uncompressed_data[32];

        ZSTD_seekable* const stream = ZSTD_seekable_create();
        assert(stream != NULL);
        {   size_t const status = ZSTD_seekable_initBuff(stream, compressed_data, compressed_size);
            if (ZSTD_isError(status)) {
                ZSTD_seekable_free(stream);
                goto _test_error;
        }   }

        /* Should return an error, but not hang */
        {   const size_t offset = 2;
            size_t const status = ZSTD_seekable_decompress(stream, uncompressed_data, uncompressed_size, offset);
            if (!ZSTD_isError(status)) {
                ZSTD_seekable_free(stream);
                goto _test_error;
        }   }

        ZSTD_seekable_free(stream);
    }
    printf("Success!\n");

    printf("Test %u - check #2 that seekable decompress does not hang: ", testNb++);
    {   /* Github issue #FIXME */
        const size_t compressed_size = 27;
        const uint8_t compressed_data[27] = {
	    '\x28',
	    '\xb5',
	    '\x2f',
	    '\xfd',
	    '\x00',
	    '\x32',
	    '\x91',
	    '\x00',
	    '\x00',
	    '\x00',
	    '\x5e',
	    '\x2a',
	    '\x4d',
	    '\x18',
	    '\x09',
	    '\x00',
	    '\x00',
	    '\x00',
	    '\x00',
	    '\x00',
	    '\x00',
	    '\x00',
	    '\x00',
	    '\xb1',
	    '\xea',
	    '\x92',
	    '\x8f',
	};
        const size_t uncompressed_size = 400;
        uint8_t uncompressed_data[400];

        ZSTD_seekable* stream = ZSTD_seekable_create();
        size_t status = ZSTD_seekable_initBuff(stream, compressed_data, compressed_size);
        if (ZSTD_isError(status)) {
            ZSTD_seekable_free(stream);
            goto _test_error;
        }

        const size_t offset = 2;
        /* Should return an error, but not hang */
        status = ZSTD_seekable_decompress(stream, uncompressed_data, uncompressed_size, offset);
        if (!ZSTD_isError(status)) {
            ZSTD_seekable_free(stream);
            goto _test_error;
        }

        ZSTD_seekable_free(stream);
    }
    printf("Success!\n");

    /* TODO: Add more tests */
    printf("Finished tests\n");
    return 0;

_test_error:
    printf("test failed! Exiting..\n");
    return 1;
}
