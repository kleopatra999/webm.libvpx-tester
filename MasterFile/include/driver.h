#include <string>
using namespace std;

#ifndef DRIVER_H_GUARD
#define DRIVER_H_GUARD

int create_working_folder(int argc, char *argv[], char *WorkingDirChar);
string date_string();
void on_error_output();
void write_32bit_quick_test(string WorkingDir);
void write_64bit_quick_test(string WorkingDir);
void print_quick_test_files(string WorkingDir);
void formated_print(string SummaryStr, int selector);
void test_help(int argc, char *argv[], string WorkingDir);
int  tool_help(string InputString);
void format_summary_by_test(char *InputFileNameCharAr, int DeleteOldFile);
void format_summary_by_test_and_result(char *InputFileNameCharAr, int DeleteOldFile);
void show_hidden_cmds();
int  main(int argc, char *argv[]);

#endif