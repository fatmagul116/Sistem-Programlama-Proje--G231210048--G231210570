#ifndef TARSAU_H
#define TARSAU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>

#define MAX_FILES 32
#define MAX_TOTAL_SIZE (200 * 1024 * 1024) // 200 MB

// Fonksiyon Prototipleri
int is_ascii(const char *filename);
void archive_files(int file_count, char *filenames[], char *output_name);
void extract_archive(char *archive_name, char *target_dir);

#endif