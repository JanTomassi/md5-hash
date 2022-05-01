#define LEFTROTATE(x, y, z) ((x << y) | (x >> ((sizeof(z) * 8) - y)))

#include <cinttypes>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <string.h>
#include <string>
#include <vector>

uint32_t K[] = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a,
       0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
       0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340,
       0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
       0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8,
       0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
       0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa,
       0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
       0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
       0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
       0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

uint32_t s[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
            5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20,
            4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
            6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

int main(int argv, char **argk) {

  const uint64_t msg_l = strlen(argk[1]);
  char *msg_p = argk[1];
  std::vector<uint8_t> msg;
  for (size_t i = 0; i < msg_l; i++) {
    msg.push_back(msg_p[i]);
  }

  msg.push_back((uint8_t)0x80);
  for (size_t i = msg.size(); (i % 64) != (56 % 64); i++) {
    msg.push_back((uint8_t)0x00);
  }

  std::cout << msg_l << std::endl;

  for (int i = 0; i < 8; i++) {
    msg.push_back(0x00);
  }

  for (const char m : msg) {
    std::cout << std::setw(2) << std::setfill('0') << std::hex
              << ((uint32_t)m & 0xFF);
    std::cout << " ";
  }
  std::cout << std::endl << std::dec;
  std::cout << msg.size() << std::endl;

  uint32_t a0 = 0x67452301;
  uint32_t b0 = 0xefcdab89;
  uint32_t c0 = 0x98badcfe;
  uint32_t d0 = 0x10325476;

  for (size_t i = 0; i < msg.size(); i += 64) {
    uint32_t A = a0;
    uint32_t B = b0;
    uint32_t C = c0;
    uint32_t D = d0;

    for (int j = 0; j < 64; j++) {
      uint32_t F, g;
      if (0 <= j && j <= 15) {
        F = (B & C) | ((~B) & D);
        g = j;
      } else if (16 <= j && j <= 31) {
        F = (D & B) | ((~D) & C);
        g = (5 * j + 1) % 16;
      } else if (32 <= j && j <= 47) {
        F = B ^ C ^ D;
        g = (3 * j + 5) % 16;
      } else if (48 <= j && j <= 63) {
        F = C ^ (B | (~D));
        g = (7 * j) % 16;
      }
      F = F + A + K[j] +
          ((((uint)msg[i + (g * 4)]) << 0) |
           (((uint)msg[i + (g * 4) + 1]) << 8) |
           (((uint)msg[i + (g * 4) + 2]) << 16) |
           (((uint)msg[i + (g * 4) + 3]) << 24));
      std::cout << "Pre: " << std::hex << F << "\t" << A << "\t" << B << "\t"
                << C << "\t" << D << "\t" << K[j] << std::endl;
      A = D;
      D = C;
      C = B;
      B = B + (uint32_t)LEFTROTATE(F, s[j], uint32_t);
      std::cout << "Post: " << std::hex << F << "\t" << A << "\t" << B << "\t"
                << C << "\t" << D << std::endl;
    }
    a0 += A;
    b0 += B;
    c0 += C;
    d0 += D;
  }

  std::cout << std::hex << a0 << " " << b0 <<" " << c0 <<" " << d0 << std::endl;

  return (0);
}
