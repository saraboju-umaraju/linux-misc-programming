
#include <stdio.h>

void test_method() {

  printf("%pn", &test_method);
}

int main(int argc, char **argv) {
  test_method();
  return 0;
}
