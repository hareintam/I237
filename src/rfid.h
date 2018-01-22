#ifndef RFID_H_
#define RFID_H_

void rfid_read(const char *const *argv);
void rfid_add(const char *const *argv);
void rfid_remove(const char *const *argv);
void rfid_print(void);
char *find_user(char *user_uid);

#endif