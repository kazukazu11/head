#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>

#define FILE_SIZE 1024

int max = 10;

int main(int argc, char *argv[]) {
	int fd;
	char *map;
	long page_size, map_size;
	int i;
	int j;
	int count1 = 0;
	int count2 = 0;

	fd = open(argv[1], O_RDONLY);

	/*正しくファイルを入力されなかった場合*/
	if (fd < 0) {
		printf("Error : can't open file\n");
		return -1;
	}

	/* ページサイズからマッピング時のサイズを計算 */
	page_size = getpagesize();
	map_size = (FILE_SIZE / page_size + 1) * page_size;

	/* メモリ上にマッピング。バッファリング */
	map = (char *)mmap(NULL, map_size, PROT_READ, MAP_SHARED, fd, 0);

	if (mmap == MAP_FAILED) {
		printf("Error : mmap failed\n");
		return -1;
	}

	/* いったん改行を全てカウント */
	for (i = 0; i < page_size; i++) {
		if (*(map + i) == '\n') {
			count1++;
		}
	}

	/*末尾からmax行前の行から出力していく*/
	for (i = 0; i < page_size; i++) {
		if (*(map + i) == '\n') {
			count2++;
		}
		if (count2 > count1 - max) {
			printf("%c", *(map + i+1));
		}
		if (count2 == count1) {
			break;
		}
	}

	close(fd);
	munmap(map, map_size);

	return 0;
}