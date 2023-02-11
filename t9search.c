#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define BUFFERSIZE 100
int max_levenshtein_distance = 0;
bool is_strict = true;

char keyboard_layout[10][8] = {
    {'+', ' '}, //0
    {},//1
    {'A', 'a', 'B', 'b', 'C', 'c'},//2
    {'D', 'd', 'E', 'e', 'F', 'f'},//3
    {'G', 'g', 'H', 'h', 'I', 'i'},//4
    {'J', 'j', 'K', 'k', 'L', 'l'},//5
    {'M', 'm', 'N', 'n', 'O', 'o'},//6
    {'P', 'p', 'Q', 'q', 'R', 'r', 'S', 's'},//7
    {'T', 't', 'U', 'u', 'V', 'v'},//8
    {'W', 'w', 'X', 'x', 'Y', 'y', 'Z', 'z'}//9
};

struct contact_t // structure for contact. Contains name and phone_number strings
{
    char name[BUFFERSIZE];
    char phone_number[BUFFERSIZE];
};

int min(int a, int b, int c) // min() returns minimal number
{	
	if(a <= b && a <= c) return a;
	else if(b <= a && b <= c) return b;
	else if(c <= a && c <= b) return c;
    return 0;
}

int get_levenshtein_distance(char *passed_number, char *full_number) { // get_levenshtein_distance() returns the number of changes needed to turn one string into another
    int x, y, passed_number_len, full_number_len;
    passed_number_len = strlen(passed_number);
    full_number_len = strlen(full_number);
    int matrix[full_number_len+1][passed_number_len+1];
    matrix[0][0] = 0;
    for (x = 1; x <= full_number_len; x++)
        matrix[x][0] = matrix[x-1][0] + 1;
    for (y = 1; y <= passed_number_len; y++)
        matrix[0][y] = matrix[0][y-1] + 1;
    for (x = 1; x <= full_number_len; x++)
        for (y = 1; y <= passed_number_len; y++)
            matrix[x][y] = min(matrix[x-1][y] + 1, matrix[x][y-1] + 1, matrix[x-1][y-1] + (passed_number[y-1] == full_number[x-1] ? 0 : 1));
    return matrix[full_number_len][passed_number_len];
}

bool check_for_numbers(char *passed_number){ // check_for_numbers() returns true if string contains only numbers 
    for (int index = 0; passed_number[index] != '\0'; ++index){
        if (!(passed_number[index] <= '9' && passed_number[index] >= '0')) return false;
    }
    return true;
}

void check_parameters(int argc, char *argv[], char *passed_number){ // check_paramenets() writes variables from parameters
    for (int argument_index = 1; argument_index<argc; argument_index++){
        if(!strcmp(argv[argument_index], "-s") && argument_index == 1) is_strict = false;
        else if(!strcmp(argv[argument_index], "-l") && check_for_numbers(argv[argument_index+1])){
            max_levenshtein_distance = strtol(argv[argument_index+1], NULL, 10);
            argument_index++;
        }
        else if(check_for_numbers(argv[argument_index])) strcpy(passed_number, argv[argument_index]);
    }

}


bool is_letter_matches_number(char letter, int number){ // is_letter_matches_number() returns true if provided letter in under provided number on keyboard layout. For example - is_letter_matches_number('A', 2); -> true, is_letter_matches_number('b', 4); -> false
    for (size_t letter_index = 0; letter_index < strlen(keyboard_layout[number]); letter_index++){
        if (letter == keyboard_layout[number][letter_index]) return true;
    }
    return false;
}

void name_to_number(char * contact_name, char * name_in_number){ // name_to_number() writes string with numbers into name_in_number array, according to keyboard layout. For example name_to_number("adI") -> "234" because 'a'-2, 'd'-3, 'I'-4
    for (int name_letter_index = 0; contact_name[name_letter_index] != '\0'; ++name_letter_index){
        for (int keyboard_number = 0; keyboard_number < 10; keyboard_number++){
            for (int keyboard_letter_index = 0; keyboard_letter_index<8;keyboard_letter_index++){
                if (contact_name[name_letter_index] == keyboard_layout[keyboard_number][keyboard_letter_index]){
                    name_in_number[name_letter_index] = keyboard_number+'0';
                }
            }
        }
    }
}

bool is_number_matches(char *passed_number, char *full_number, bool is_strict){ // is_number_matches() returns true if provided numbers are in contacts phone_number. is_strict is about -s argument. For example is_name_matches("38", "231338474" ,false); -> true because number contains "38"
    if (strlen(full_number) < strlen(passed_number)) return false;
    if(get_levenshtein_distance(passed_number, full_number) <= max_levenshtein_distance){
        return true;
    }
    bool status;
    int found_characters_number = 0;
    for (int full_number_index = 0; full_number[full_number_index] != '\0'; ++full_number_index){
        status = true;
        if (is_strict){
            if((strlen(full_number) - full_number_index) < strlen(passed_number)) return false;
            if(full_number[full_number_index] == passed_number[0]){
                for (size_t passed_number_index = 0; passed_number_index < strlen(passed_number); passed_number_index++){
                    if(!(full_number[full_number_index + passed_number_index] == passed_number[passed_number_index])) status = false;
                }
                if (status) return true;
            }
        }else{
            if(full_number[full_number_index] == passed_number[found_characters_number]){
                found_characters_number++;
                if(found_characters_number == (int)strlen(passed_number)){
                    return true;
                }
            }
        }
    }

    return false;
}

bool is_contact_matches(char *passed_number, struct contact_t contact, bool is_strict){ // is_contact_matches() returns true if provided contact matches to number from argument
    if(passed_number[0] == '\0') return true;
    char name_in_number[BUFFERSIZE]; 
    name_to_number(contact.name, name_in_number); // unfortunately, because malloc() is forbidden in this project, I have to pre-create array and modify it instead of returning it from function
    return is_number_matches(passed_number, name_in_number, is_strict) || is_number_matches(passed_number, contact.phone_number, is_strict);
}

int read_file(struct contact_t * contacts_list){ // read_file() reads contacts from file and writes them into contacts_list array
    char buffer[BUFFERSIZE];
    int index = 0;
    bool is_contacts_name = true;
    struct contact_t contact;
    while(fgets(buffer, BUFFERSIZE , stdin)){
        
        if (is_contacts_name)
        {
            strcpy(contact.name, buffer);
            is_contacts_name = false;
        } else {
            strcpy(contact.phone_number, buffer);
            is_contacts_name = true;
            contact.name[strcspn(contact.name, "\n")] = 0;
            contact.phone_number[strcspn(contact.phone_number, "\n")] = 0;
            contacts_list[index] = contact;
            index++;
        }
    }
    return index;
}

void get_results(char *passed_number, struct contact_t *contacts_list, int contacts_number, bool is_strict){ // get_results() gets matching contacts and prints them
    int matching_contacts_count = 0;
    struct contact_t result_contacts_list[contacts_number];
    for (int contact_index = 0; contact_index < contacts_number; contact_index++){
        if (is_contact_matches(passed_number, contacts_list[contact_index], is_strict)){
            result_contacts_list[matching_contacts_count] = contacts_list[contact_index];
            matching_contacts_count++;
        }
    }
    if (!matching_contacts_count){
        printf("Not found\n");
    }
    for (int contact_index = 0; contact_index < matching_contacts_count; contact_index++){
        printf("%s, %s\n", result_contacts_list[contact_index].name, result_contacts_list[contact_index].phone_number);
    }
}

int main(int argc, char *argv[]){
    char passed_number[BUFFERSIZE] = "";
    check_parameters(argc, argv, passed_number);
    struct contact_t contacts_list[BUFFERSIZE];
    int contacts_number = read_file(contacts_list); // unfortunately, because malloc() is forbidden in this project, I have to pre-create array and modify it instead of returning it from function
    get_results(passed_number, contacts_list, contacts_number, is_strict);
    return 0;
}