#include <stdlib.h>
#include <avr/pgmspace.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "../lib/matejx_avr_lib/mfrc522.h"
#include <string.h>
#include <stdio.h>


typedef struct card {
    char *uid;
    char *user;
    short size;
    struct card *next;
} card_t;

card_t * authorized_cards = NULL;

void rfid_read(const char *const *argv)
{
    (void) argv;
    Uid uid;
    uart0_puts_p(PSTR("\r\n"));

    if (PICC_IsNewCardPresent()) {
        uart0_puts_p(PSTR("Card selected!\r\n"));
        PICC_ReadCardSerial(&uid);
        uart0_puts_p(PSTR("Card type: "));
        uart0_puts(PICC_GetTypeName(PICC_GetType(uid.sak)));
        uart0_puts_p(PSTR("\r\n"));
        uart0_puts_p(PSTR("Card UID: "));
        char to_hex[4];

        for (byte i = 0; i < uid.size; i++) {
            sprintf(to_hex, "%02X", uid.uidByte[i]);
            uart0_puts(to_hex);
        }

        char uid_size[10];
        uart0_puts_p(PSTR(" (size " ));
        uart0_puts(itoa(uid.size, uid_size, 10));
        uart0_puts_p(PSTR(" bytes)\r\n"));
    } else {
        uart0_puts_p(PSTR("Unable to select card.\r\n"));
    }
}

void rfid_add(const char *const *argv)
{
    uart0_puts_p(PSTR("\r\n"));
    card_t *new_card;
    new_card = malloc(sizeof(card_t));

    if (!(strlen(argv[1]) > 20)) {
        if (new_card == NULL) {
            uart0_puts_p(PSTR("Failed to allocate memory\r\n"));
            free(new_card);
            return;
        }
    } else {
        uart0_puts_p(
            PSTR("UID maximum size is 20 hexadecimal numbers, e.g 10 bytes!\r\n"));
        return;
    }

    // Memory allocation and initializing new card values
    new_card->uid = malloc(strlen(argv[1]) + 1);

    if (new_card->uid == NULL) {
        uart0_puts_p(PSTR("Failed to allocate memory\r\n"));
        free(new_card);
        return;
    }

    strcpy(new_card->uid, argv[1]);
    new_card->size = strlen(argv[1]) / 2;
    new_card->user = malloc(strlen(argv[2]) + 1);

    if (new_card->user == NULL) {
        uart0_puts_p(PSTR("Failed to allocate memory\r\n"));
        free(new_card);
        return;
    }

    strcpy(new_card->user, argv[2]);

    // Check if card is already added
    if (authorized_cards != NULL) {
        new_card->next = authorized_cards;
        card_t *current = authorized_cards;

        while (current != NULL) {
            if (!strcmp(current->uid, new_card->uid)) {
                uart0_puts_p(PSTR("This card is already in use.\r\n"));
                return;
            }

            current = current->next;
        }

        //Added card is first in list
    } else {
        new_card->next = NULL;
    }

    char uid_size[10];
    authorized_cards = new_card;
    uart0_puts_p(PSTR("Added new card with UID: "));
    uart0_puts(new_card->uid);
    uart0_puts_p(PSTR("\r\nUID size: "));
    uart0_puts(itoa(new_card->size, uid_size, 10));
    uart0_puts_p(PSTR(" bytes\r\nUser name: "));
    uart0_puts(new_card->user);
    uart0_puts_p(PSTR("\r\n"));
}

void rfid_remove(const char *const *argv)
{
    card_t *current = authorized_cards;
    card_t *prev = NULL;

    if (authorized_cards == NULL) {
        uart0_puts("There are no cards added yet.\r\n");
        return;
    } else {
        while (current != NULL) {
            if (!strcmp(argv[1], current->uid)) {
                uart0_puts_p(PSTR("Card with uid "));
                uart0_puts(current->uid);
                uart0_puts_p(PSTR(" is removed.\r\n"));
                free(current);
                return;
            } else {
                if (prev == NULL) {
                    authorized_cards = current->next;
                } else {
                    prev->next = current->next;
                }
            }

            prev = current;
            current = current->next;
        }

        uart0_puts_p(PSTR("Card not found.\n\r"));
        return;
    }
}

void rfid_print(void)
{
    uart0_puts_p(PSTR("\r\n"));

    if (authorized_cards == NULL) {
        uart0_puts_p(PSTR("There are no cards added yet.\r\n"));
    } else {
        card_t *current = authorized_cards;
        int counter = 1;
        char counts[10];
        char uid_size[10];

        while (current != NULL) {
            itoa(counter, counts, 10);
            uart0_puts(counts);
            uart0_puts_p(PSTR(". UID: "));
            uart0_puts(current->uid);
            uart0_puts_p(PSTR(" (size "));
            uart0_puts(itoa(current->size, uid_size, 10));
            uart0_puts_p(PSTR(" bytes) belongs to user: "));
            uart0_puts(current->user);
            uart0_puts_p(PSTR("\r\n"));
            current = current->next;
            counter++;
        }
    }
}

const char *find_user(char *user_uid)
{
    card_t *current = authorized_cards;

    while (current != NULL) {
        if (!strcmp(current->uid, user_uid)) {
            return current->user;
        }

        current = current->next;
    }

    return NULL;
}