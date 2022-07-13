# morton_enc

Header only library offering a generic implementation for morton encoding (Z-ordering). This is similar to the LUT approach of the amazing [morton-nd](https://github.com/morton-nd/morton-nd) library with the following differences:
- gathers the results to a std::bitset to support arbitrary length for the encoded value
- no support for compile-time evaluation (bitset shift operations are not `constexpr`)
- no tweaking of chunk size etc., the only template parameter is the output bit length of the encoded value

The goal of this implementation is to 
- offer an alternative when bit truncation is undesirable
-`allow straightforward integration when std::bitset` is used elsewhere in the code
- satisfy my NIH syndrome

## Usage

Simply add the header file to your project. You can encode values as such:

```
#include <morton_enc.hpp>

morton::encoder<64> e; // an appropriate output bit size must be selected to fit the encoded values or there will be truncated bits

std::bitset<64> x = e.encode(1, 2, 3, 4, 5);
std::bitset<64> y = e.encode(1, 2, 3, 4, 5, 6, 7, 8); // the dimensionality is not fixed for a particular encoder object
```
