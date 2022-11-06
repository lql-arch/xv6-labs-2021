#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"
#include "../kernel/fs.h" 

void find(char *argv);
char *fmtname(char *path);

char finally[512];

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(2, "参数太少。\n");
    exit(1);
  }

  if (argc == 2) {
    strcpy(finally, argv[1]);
    find(".");
  } else if (argc == 3) {
    strcpy(finally, argv[2]);
    find(argv[1]);
  } else {
    fprintf(2, "参数太多。\n");
  }

  exit(0);
}

void find(char *path) {
  int len = strlen(path);
  int fd;
  char *p;
  struct dirent dirent;
  struct stat st;
  char buf[512];

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find : cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type) {
  case T_DIR:
      if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
        fprintf(2, "find: path too long\n");
        break;
      }
      strcpy(buf, path);
      p = buf + len;
      *p++ = '/';
      while ( read(fd, &dirent, sizeof(dirent)) == sizeof(dirent)) {
        if (dirent.inum == 0) {
          continue;
        }
        memmove(p, dirent.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0) {
          fprintf(2, "find: path too long\n");
          continue;
        }
        if(st.type == T_DIR && strcmp(p, ".") != 0 && strcmp(p, "..") != 0){
          find(buf);
        }
        if (strcmp(dirent.name, finally) == 0) {
          printf("%s\n", fmtname(buf));
        }
      }
      break;
    default:
      fprintf(2, "using find <dir> <filename/dir>\n");
      break;
  }
  close(fd);
}

char *fmtname(char *path) {
  static char buf[DIRSIZ + 1];
  char *p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if (strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
  return buf;
}