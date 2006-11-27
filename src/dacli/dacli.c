/*
 * dacli.c - libdisarm command line interface
 *
 * Copyright (C) 2007  Jon Lund Steffensen <jonlst@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include <libdisarm/disarm.h>


#define USAGE \
	"Usage: %s [-EB|-EL] [-h] [-m OFFSET] [-s SKIP] [FILE]\n"
#define HELP \
	USAGE \
	" Disassemble ARM machine code from FILE or standard input.\n" \
	"  -EB\t\tRead input as big endian data\n" \
	"  -EL\t\tRead input as little endian data\n" \
	"  -h\t\tDisplay this help message\n" \
	"  -m OFFSET\tUse OFFSET as memory address of input\n" \
	"  -s SKIP\tNumber of bytes to skip before disassembly\n" \
	"Report bugs to <" PACKAGE_BUGREPORT ">.\n"

/* Return -1 on error, 0 on EOF, 1 on succesful read. */
int
read_hex_input(void *dest, size_t size, FILE *f)
{
	int c = fgetc(f);
	if (c == EOF) return 0;
	
	int i;
	for (i = 0; i < size; i++) {
		while (isspace(c)) {
			c = fgetc(f);
		}

		if (c == EOF) return 0;

		int byte = 0;
		int j;
		for (j = 0; j < 2; j++) {
			int input;
			if (c >= '0' && c <= '9') {
				input = c - '0';
			} else if (c >= 'a' && c <= 'f') {
				input = c - 'a' + 10;
			} else if (c >= 'A' && c <= 'F') {
				input = c - 'A' + 10;
			} else if (j > 0 && isspace(c)) {
				break;
			} else {
				return -1;
			}
					
			byte = (byte << 4) | input;

			c = fgetc(f);
			if (c == EOF) break;
		}

		((unsigned char *)dest)[i] = byte;
	}

	return 1;
}

int
main(int argc, char *argv[])
{
	int r;

	int hex_input = 0;
	da_addr_t mem_offset = 0;
	off_t file_offset = 0;
	ssize_t disasm_size = -1;
	int big_endian = 0;

	int opt;
	while ((opt = getopt(argc, argv, "c:E:hm:s:x")) != -1) {
		switch (opt) {
		case 'c':
			disasm_size = atoi(optarg);
			break;
		case 'E':
			if (optarg != NULL &&
			    (optarg[0] == 'B' || optarg[0] == 'L')) {
				big_endian = (optarg[0] == 'B');
			} else {
				fprintf(stderr, USAGE, argv[0]);
				exit(EXIT_FAILURE);
			}
			break;
		case 'h':
			printf(HELP, argv[0]);
			exit(EXIT_SUCCESS);
			break;
		case 'm':
			mem_offset = atoi(optarg);
			break;
		case 's':
			file_offset = atoi(optarg);
			break;
		case 'x':
			hex_input = 1;
			break;
		default:
			fprintf(stderr, USAGE, argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	FILE *f = stdin;
	
	if (optind < argc && strcmp(argv[optind], "-")) {
		f = fopen(argv[optind], "rb");
		if (f == NULL) {
			perror("fopen");
			exit(EXIT_FAILURE);
		}

		if (file_offset > 0) {
			r = fseek(f, file_offset, SEEK_SET);
			if (r < 0) {
				perror("fseek");
				exit(EXIT_FAILURE);
			}
		}
	}

	da_addr_t addr = mem_offset;

	while (disasm_size < 0 || addr < mem_offset + disasm_size) {
		da_word_t data;

		if (!hex_input) {
			size_t read = fread(&data, sizeof(da_word_t), 1, f);
			if (read < 1) {
				if (feof(f)) break;
				else {
					perror("fread");
					exit(EXIT_FAILURE);
				}
			}
		} else {
			r = read_hex_input(&data, sizeof(da_word_t), f);
			if (r < 0) {
				fprintf(stderr, "Unable to parse input.\n");
				exit(EXIT_FAILURE);
			} else if (r == 0) {
				break;
			}
		}
		
		da_instr_t instr;
		da_instr_args_t args;
		da_instr_parse(&instr, data, big_endian);
		da_instr_parse_args(&args, &instr);

		printf("%08x\t", addr);
		printf("%08x\t", instr.data);
		da_instr_fprint(stdout, &instr, &args, addr);
		printf("\n");

		addr += sizeof(da_word_t);
	}

	r = fclose(f);
	if (r < 0) {
		perror("fclose");
		exit(EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;
}
