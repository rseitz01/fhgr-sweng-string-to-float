# string_to_float

## Project Member(s)
- Raphael Seitz

## String to Float conversion features ([`string_to_float.c`](src/string_to_float.c))
- convert positive / negative integers (e.g. `12`)
- convert positive / negative floating point numbers (e.g. `-12.34`)
- convert positive / negative floating point numbers with exponents (e.g. `+12.34e-56`)
- allows incomplete inputs (e.g. `.12`, `12.`, `.12e3`, `-.12e+3`; but not `-.12e`, `-e23`)
- precision of 15 decimal places
- ignores leading + trailing whitespace
- accepts only valid number literals (e.g. `1d` is invalid)
- general rule: follow C-style floating point notation and you're good to go

## Building

```sh
make
```

- Works with `GNU Make 4.4.1`
- Expected to create two binaries:
    - `bin/test` (unit testing)
    - `bin/main` (ignore this one)

### if all else fails
```sh
./compile_test.sh
```

- will create `bin/test`

## Running Tests

```sh
bin/test
```
- expecting 80 tests to successfully run

### Successfully passed tests with
- `gcc (Gentoo 13.2.1_p20230826 p7) 13.2.1 20230826`
- `tcc version 0.9.27 (x86_64 Linux)`
- `clang version 16.0.6`

