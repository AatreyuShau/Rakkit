#include <stdio.h>

int add(int a, int b) {
return (a + b);
}

int main() {
printf("%s\n", "hello world!!");
const char* str = "input x here:";
printf("%s\n", str);
int x = ({ int __tmp; scanf("%d", &__tmp); __tmp; });
printf("%s\n", "input y here:");
int y = ({ int __tmp; scanf("%d", &__tmp); __tmp; });
printf("%d\n", add(x, y));
}

