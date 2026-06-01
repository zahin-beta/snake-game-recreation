#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

static int isRegularFile(const char *path)
{
    struct stat st;
    return stat(path, &st) == 0 && (st.st_mode & S_IFREG);
}

static void writeData(FILE *out, const char *path, const char *varName)
{
    FILE *f = fopen(path, "rb");
    if (!f) { fprintf(stderr, "Cannot open %s\n", path); exit(1); }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    fprintf(out, "const unsigned char %s_DATA[] = {\n", varName);

    unsigned char buf[12];
    long total = 0;
    while (total < size) {
        int n = (size - total > 12) ? 12 : (int)(size - total);
        fread(buf, 1, n, f);
        fprintf(out, "    ");
        for (int i = 0; i < n; i++)
            fprintf(out, "0x%02X, ", buf[i]);
        fprintf(out, "\n");
        total += n;
    }

    fprintf(out, "};\n");
    fprintf(out, "const unsigned int %s_SIZE = sizeof(%s_DATA);\n\n", varName, varName);
    fclose(f);
}

static void nameForFile(const char *filename, char *out)
{
    const char *dot = strrchr(filename, '.');
    if (!dot) { strcpy(out, filename); return; }

    int len = (int)(dot - filename);
    for (int i = 0; i < len; i++)
        out[i] = (filename[i] >= 'a' && filename[i] <= 'z') ? filename[i] - 32 : filename[i];
    out[len] = '_';
    for (int i = 1; dot[i]; i++)
        out[len + i] = (dot[i] >= 'a' && dot[i] <= 'z') ? dot[i] - 32 : dot[i];
    out[len + strlen(dot)] = '\0';
}

int main(int argc, char **argv)
{
    const char *assetsDir = argc > 1 ? argv[1] : "../assets";
    const char *outputFile = argc > 2 ? argv[2] : "assets.c";

    DIR *d = opendir(assetsDir);
    if (!d) { fprintf(stderr, "Cannot open directory %s\n", assetsDir); return 1; }

    FILE *out = fopen(outputFile, "w");
    if (!out) { fprintf(stderr, "Cannot write %s\n", outputFile); return 1; }

    fprintf(out, "#include \"assets.h\"\n\n");

    struct dirent *entry;
    while ((entry = readdir(d)) != NULL) {
        char path[512];
        snprintf(path, sizeof(path), "%s/%s", assetsDir, entry->d_name);
        if (!isRegularFile(path)) continue;

        const char *ext = strrchr(entry->d_name, '.');
        if (ext && strcmp(ext, ".a") == 0) continue;

        char varName[256];
        nameForFile(entry->d_name, varName);

        writeData(out, path, varName);
    }

    fclose(out);
    closedir(d);
    printf("Generated %s\n", outputFile);
    return 0;
}
