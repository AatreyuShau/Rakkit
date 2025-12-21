#include <stdio.h>

#include "runtime/csv.h"
int add(int a, int b) {
return (a + b);
}

int main() {
printf("%s\n", "input n (for nth name) here:");
int x = ({ int __tmp; scanf("%d", &__tmp); __tmp; });
printf("%s\n", "input m (for mth city) here:");
int y = ({ int __tmp; scanf("%d", &__tmp); __tmp; });
printf("%s", "sum: ");
printf("%d\n", add(x, y));
CSV* data = csvRead("people.csv");
printf("%s", "name: ");
const char* nom = csvGet(data, x, "name");
printf("%s\n", nom);
printf("%s", "city: ");
const char* city = csvGet(data, y, "city");
printf("%s\n", city);
printf("%s\n", "hello world!!");
}

