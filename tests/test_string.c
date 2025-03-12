#include "s21_test_string.h"

int main() {
  int fails_number = 0;
  // int suite_counter = 1;
  SRunner *sr = srunner_create(suite_build());
  for (int i = 0; i < 21; i++) {
    // 18 maximum
    srunner_add_suite(sr, suite_maker(i));
    // SRunner *sr = srunner_create(suite_maker(i));
  }
  // srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  srunner_free(sr);
  fails_number += srunner_ntests_failed(sr);
  return (fails_number == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

Suite *suite_maker(int i) {
  const Suite *list[] = {suite_memchr(),   suite_memcmp(),  suite_memcpy(),
                         suite_memset(),   suite_strncat(), suite_strlen(),
                         suite_strchr(),   suite_strncmp(), suite_strncpy(),
                         suite_strrchr(),  suite_strcspn(), suite_strstr(),
                         suite_strpbrk(),  suite_strtok(),  suite_to_upper(),
                         suite_to_lower(), suite_insert(),  suite_trim(),
                         suite_sprintf(),  suite_sscanf(),  suite_strerror()};
  return (Suite *)list[i];
}

// Не использовать srunner_add_suite т.к. течет по стршному (либо пойми как с
// ним работать) Вилку не юзать по той же причине
