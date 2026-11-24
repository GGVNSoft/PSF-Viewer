#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>



typedef enum {
    M_USAGE, M_FILE_TOO_SMALL, M_READ_FAILED, M_TRUNCATED,
    M_BAD_DIMS, M_BAD_SIG, M_LOADED,
    M_MENU_HEADER, M_MENU_INFO, M_MENU_LIST, M_MENU_GLYPH,
    M_MENU_FIND, M_MENU_LANG, M_MENU_EXIT, M_PROMPT,
    M_FONT_INFO, M_FILE_LABEL, M_VERSION_LABEL, M_GLYPHS_LABEL,
    M_SIZE_LABEL, M_UNI_LABEL, M_YES, M_NO,
    M_GLYPHS_HEADER, M_PRESS_ENTER,
    M_GLYPH_INDEX_PROMPT, M_GLYPH_HEADER, M_UNICODE_LABEL,
    M_NO_UNICODE, M_OUT_OF_RANGE, M_UNICODE_PROMPT, M_GLYPH_NOT_FOUND,
    M_UNKNOWN_OPTION, M_LANG_PROMPT, M_LANG_UNKNOWN, M_LANG_SET,
    M_COUNT
} MsgId;

enum { LANG_EN, LANG_RU, LANG_ES, LANG_COUNT };

static const char *lang_names[LANG_COUNT] = { "English", "Русский", "Español" };
static int g_lang = LANG_EN;

static const char *tr_table[LANG_COUNT][M_COUNT] = {
    [LANG_EN] = {
        [M_USAGE]              = "Usage: %s <file.psf> [-l en|ru|es]",
        [M_FILE_TOO_SMALL]     = "File too small.",
        [M_READ_FAILED]        = "Failed to read file.",
        [M_TRUNCATED]          = "PSF%d: file truncated (need %zu, have %ld).",
        [M_BAD_DIMS]           = "PSF2: invalid dimensions.",
        [M_BAD_SIG]            = "Unknown signature: not PSF1 nor PSF2.",
        [M_LOADED]             = "Loaded %s: PSF%d, glyphs %d, %dx%d, unicode table: %s",
        [M_MENU_HEADER]        = "===== PSF Viewer =====",
        [M_MENU_INFO]          = "1. Font info",
        [M_MENU_LIST]          = "2. List glyphs",
        [M_MENU_GLYPH]         = "3. Show glyph by index",
        [M_MENU_FIND]          = "4. Find glyph by Unicode (hex)",
        [M_MENU_LANG]          = "5. Change language",
        [M_MENU_EXIT]          = "0. Exit",
        [M_PROMPT]             = "Choice: ",
        [M_FONT_INFO]          = "Font info:",
        [M_FILE_LABEL]         = "  File: %s",
        [M_VERSION_LABEL]      = "  Version: PSF%d",
        [M_GLYPHS_LABEL]       = "  Glyphs: %d",
        [M_SIZE_LABEL]         = "  Glyph size: %dx%d (%d bytes/glyph)",
        [M_UNI_LABEL]          = "  Unicode table: %s",
        [M_YES]                = "yes",
        [M_NO]                 = "no",
        [M_GLYPHS_HEADER]      = "Glyphs (index: Unicode / char):",
        [M_PRESS_ENTER]        = "-- press Enter to continue (q - back to menu) --",
        [M_GLYPH_INDEX_PROMPT] = "Glyph index (0..%d): ",
        [M_GLYPH_HEADER]       = "Glyph %d  (%dx%d)",
        [M_UNICODE_LABEL]      = "Unicode: U+%04X  char: '%s'",
        [M_NO_UNICODE]         = "Unicode: no data (glyph has no mapping)",
        [M_OUT_OF_RANGE]       = "Index out of range (0..%d).",
        [M_UNICODE_PROMPT]     = "Unicode codepoint in hex (e.g. 0041): ",
        [M_GLYPH_NOT_FOUND]    = "Glyph with U+%04X not found.",
        [M_UNKNOWN_OPTION]     = "Unknown option.",
        [M_LANG_PROMPT]        = "Language (en/ru/es): ",
        [M_LANG_UNKNOWN]       = "Unknown language.",
        [M_LANG_SET]           = "Language set to: %s",
    },
    [LANG_RU] = {
        [M_USAGE]              = "Использование: %s <файл.psf> [-l en|ru|es]",
        [M_FILE_TOO_SMALL]     = "Файл слишком мал.",
        [M_READ_FAILED]        = "Не удалось прочитать файл.",
        [M_TRUNCATED]          = "PSF%d: файл обрезан (нужно %zu, есть %ld).",
        [M_BAD_DIMS]           = "PSF2: некорректные размеры.",
        [M_BAD_SIG]            = "Неизвестная сигнатура: не PSF1 и не PSF2.",
        [M_LOADED]             = "Загружен %s: PSF%d, глифов %d, %dx%d, unicode-таблица: %s",
        [M_MENU_HEADER]        = "===== PSF Viewer =====",
        [M_MENU_INFO]          = "1. Информация о шрифте",
        [M_MENU_LIST]          = "2. Список глифов",
        [M_MENU_GLYPH]         = "3. Показать глиф по индексу",
        [M_MENU_FIND]          = "4. Найти глиф по Unicode (hex)",
        [M_MENU_LANG]          = "5. Сменить язык",
        [M_MENU_EXIT]          = "0. Выход",
        [M_PROMPT]             = "Выбор: ",
        [M_FONT_INFO]          = "Информация о шрифте:",
        [M_FILE_LABEL]         = "  Файл: %s",
        [M_VERSION_LABEL]      = "  Версия: PSF%d",
        [M_GLYPHS_LABEL]       = "  Глифов: %d",
        [M_SIZE_LABEL]         = "  Размер глифа: %dx%d (%d байт/глиф)",
        [M_UNI_LABEL]          = "  Unicode-таблица: %s",
        [M_YES]                = "есть",
        [M_NO]                 = "нет",
        [M_GLYPHS_HEADER]      = "Глифы (индекс: Unicode / символ):",
        [M_PRESS_ENTER]        = "-- нажмите Enter для продолжения (q - выход в меню) --",
        [M_GLYPH_INDEX_PROMPT] = "Индекс глифа (0..%d): ",
        [M_GLYPH_HEADER]       = "Глиф %d  (%dx%d)",
        [M_UNICODE_LABEL]      = "Unicode: U+%04X  символ: '%s'",
        [M_NO_UNICODE]         = "Unicode: нет данных (глиф без сопоставления)",
        [M_OUT_OF_RANGE]       = "Индекс вне диапазона (0..%d).",
        [M_UNICODE_PROMPT]     = "Unicode codepoint в hex (например 0041): ",
        [M_GLYPH_NOT_FOUND]    = "Глиф с U+%04X не найден.",
        [M_UNKNOWN_OPTION]     = "Неизвестный пункт.",
        [M_LANG_PROMPT]        = "Язык (en/ru/es): ",
        [M_LANG_UNKNOWN]       = "Неизвестный язык.",
        [M_LANG_SET]           = "Язык изменён на: %s",
    },
    [LANG_ES] = {
        [M_USAGE]              = "Uso: %s <archivo.psf> [-l en|ru|es]",
        [M_FILE_TOO_SMALL]     = "Archivo demasiado pequeño.",
        [M_READ_FAILED]        = "No se pudo leer el archivo.",
        [M_TRUNCATED]          = "PSF%d: archivo truncado (necesita %zu, tiene %ld).",
        [M_BAD_DIMS]           = "PSF2: dimensiones no válidas.",
        [M_BAD_SIG]            = "Firma desconocida: no es PSF1 ni PSF2.",
        [M_LOADED]             = "Cargado %s: PSF%d, glifos %d, %dx%d, tabla unicode: %s",
        [M_MENU_HEADER]        = "===== Visor PSF =====",
        [M_MENU_INFO]          = "1. Información de la fuente",
        [M_MENU_LIST]          = "2. Listar glifos",
        [M_MENU_GLYPH]         = "3. Mostrar glifo por índice",
        [M_MENU_FIND]          = "4. Buscar glifo por Unicode (hex)",
        [M_MENU_LANG]          = "5. Cambiar idioma",
        [M_MENU_EXIT]          = "0. Salir",
        [M_PROMPT]             = "Opción: ",
        [M_FONT_INFO]          = "Información de la fuente:",
        [M_FILE_LABEL]         = "  Archivo: %s",
        [M_VERSION_LABEL]      = "  Versión: PSF%d",
        [M_GLYPHS_LABEL]       = "  Glifos: %d",
        [M_SIZE_LABEL]         = "  Tamaño del glifo: %dx%d (%d bytes/glifo)",
        [M_UNI_LABEL]          = "  Tabla unicode: %s",
        [M_YES]                = "sí",
        [M_NO]                 = "no",
        [M_GLYPHS_HEADER]      = "Glifos (índice: Unicode / carácter):",
        [M_PRESS_ENTER]        = "-- pulse Enter para continuar (q - volver al menú) --",
        [M_GLYPH_INDEX_PROMPT] = "Índice del glifo (0..%d): ",
        [M_GLYPH_HEADER]       = "Glifo %d  (%dx%d)",
        [M_UNICODE_LABEL]      = "Unicode: U+%04X  carácter: '%s'",
        [M_NO_UNICODE]         = "Unicode: sin datos (glifo sin asignación)",
        [M_OUT_OF_RANGE]       = "Índice fuera de rango (0..%d).",
        [M_UNICODE_PROMPT]     = "Codepoint Unicode en hex (p.ej. 0041): ",
        [M_GLYPH_NOT_FOUND]    = "Glifo con U+%04X no encontrado.",
        [M_UNKNOWN_OPTION]     = "Opción desconocida.",
        [M_LANG_PROMPT]        = "Idioma (en/ru/es): ",
        [M_LANG_UNKNOWN]       = "Idioma desconocido.",
        [M_LANG_SET]           = "Idioma cambiado a: %s",
    },
};

static const char *tr(MsgId id) {
    const char *s = tr_table[g_lang][id];
    return s ? s : tr_table[LANG_EN][id];
}
#define _(id) tr(id)

static int lang_from_string(const char *s) {
    if (!s) return -1;
    if (strncmp(s, "ru", 2) == 0) return LANG_RU;
    if (strncmp(s, "es", 2) == 0) return LANG_ES;
    if (strncmp(s, "en", 2) == 0) return LANG_EN;
    return -1;
}

static void detect_language_from_env(void) {
    const char *vars[] = { "LC_ALL", "LC_MESSAGES", "LANG", NULL };
    for (int i = 0; vars[i]; i++) {
        const char *v = getenv(vars[i]);
        if (!v || !*v) continue;
        int l = lang_from_string(v);
        if (l >= 0) { g_lang = l; return; }
    }
}



static uint16_t rd16(const uint8_t *p) { return (uint16_t)p[0] | ((uint16_t)p[1] << 8); }
static uint32_t rd32(const uint8_t *p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}



typedef struct {
    int      version;
    int      numglyphs;
    int      width, height;
    int      bytesperglyph;
    uint8_t *glyphs;
    uint8_t *uni_raw;
    size_t   uni_size;
    int      has_unicode;
} Font;

static void font_free(Font *f) {
    free(f->glyphs);
    free(f->uni_raw);
    memset(f, 0, sizeof(*f));
}

static void flush_line(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}



static int load_psf(const char *path, Font *f) {
    FILE *fp = fopen(path, "rb");
    if (!fp) { perror("fopen"); return -1; }

    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    if (sz < 4) {
        fprintf(stderr, "%s\n", _(M_FILE_TOO_SMALL));
        fclose(fp); return -1;
    }

    uint8_t *buf = malloc((size_t)sz);
    if (!buf) { perror("malloc"); fclose(fp); return -1; }
    if (fread(buf, 1, (size_t)sz, fp) != (size_t)sz) {
        fprintf(stderr, "%s\n", _(M_READ_FAILED));
        free(buf); fclose(fp); return -1;
    }
    fclose(fp);

    memset(f, 0, sizeof(*f));

    if (buf[0] == 0x36 && buf[1] == 0x04 && sz >= 4) {

        f->version       = 1;
        uint8_t mode     = buf[2];
        uint8_t charsz   = buf[3];
        int n            = (mode & 0x01) ? 512 : 256;

        f->numglyphs     = n;
        f->width         = 8;
        f->height        = charsz;
        f->bytesperglyph = charsz;
        f->has_unicode   = (mode & 0x02) != 0;

        size_t off  = 4;
        size_t need = off + (size_t)n * charsz;
        if (sz < (long)need) {
            fprintf(stderr, _(M_TRUNCATED), f->version, need, sz);
            fprintf(stderr, "\n");
            free(buf); return -1;
        }

        f->glyphs = malloc((size_t)n * charsz);
        if (!f->glyphs) { perror("malloc"); free(buf); return -1; }
        memcpy(f->glyphs, buf + off, (size_t)n * charsz);

        if (f->has_unicode) {
            f->uni_size = (size_t)sz - need;
            f->uni_raw  = malloc(f->uni_size);
            if (f->uni_raw) memcpy(f->uni_raw, buf + need, f->uni_size);
            else f->has_unicode = 0;
        }
    } else if (rd32(buf) == 0x864ab572u && sz >= 32) {

        f->version       = 2;
        uint32_t hdrsize = rd32(buf + 8);
        uint32_t flags   = rd32(buf + 12);
        uint32_t n       = rd32(buf + 16);
        uint32_t bpg     = rd32(buf + 20);
        uint32_t h       = rd32(buf + 24);
        uint32_t w       = rd32(buf + 28);

        if (n == 0 || bpg == 0 || w == 0 || h == 0) {
            fprintf(stderr, "%s\n", _(M_BAD_DIMS));
            free(buf); return -1;
        }

        f->numglyphs     = (int)n;
        f->width         = (int)w;
        f->height        = (int)h;
        f->bytesperglyph = (int)bpg;
        f->has_unicode   = (flags & 0x01) != 0;

        size_t need = hdrsize + (size_t)n * bpg;
        if (sz < (long)need) {
            fprintf(stderr, _(M_TRUNCATED), f->version, need, sz);
            fprintf(stderr, "\n");
            free(buf); return -1;
        }

        f->glyphs = malloc((size_t)n * bpg);
        if (!f->glyphs) { perror("malloc"); free(buf); return -1; }
        memcpy(f->glyphs, buf + hdrsize, (size_t)n * bpg);

        if (f->has_unicode) {
            f->uni_size = (size_t)sz - need;
            f->uni_raw  = malloc(f->uni_size);
            if (f->uni_raw) memcpy(f->uni_raw, buf + need, f->uni_size);
            else f->has_unicode = 0;
        }
    } else {
        fprintf(stderr, "%s\n", _(M_BAD_SIG));
        free(buf); return -1;
    }

    free(buf);
    return 0;
}



static int cp_to_utf8(uint32_t cp, char out[5]) {
    if (cp <= 0x7F) { out[0] = (char)cp; out[1] = 0; return 1; }
    if (cp <= 0x7FF) {
        out[0] = (char)(0xC0 | (cp >> 6));
        out[1] = (char)(0x80 | (cp & 0x3F));
        out[2] = 0; return 2;
    }
    if (cp <= 0xFFFF) {
        out[0] = (char)(0xE0 | (cp >> 12));
        out[1] = (char)(0x80 | ((cp >> 6) & 0x3F));
        out[2] = (char)(0x80 | (cp & 0x3F));
        out[3] = 0; return 3;
    }
    if (cp <= 0x10FFFF) {
        out[0] = (char)(0xF0 | (cp >> 18));
        out[1] = (char)(0x80 | ((cp >> 12) & 0x3F));
        out[2] = (char)(0x80 | ((cp >> 6) & 0x3F));
        out[3] = (char)(0x80 | (cp & 0x3F));
        out[4] = 0; return 4;
    }
    return 0;
}

static uint32_t glyph_unicode(const Font *f, int idx) {
    if (!f->has_unicode || !f->uni_raw) return 0;

    if (f->version == 1) {
        const uint8_t *p = f->uni_raw;
        const uint8_t *end = p + f->uni_size;
        int cur = 0;
        while (p + 2 <= end) {
            uint16_t v = rd16(p); p += 2;
            if (v == 0xFFFF) { cur++; if (cur > idx) break; continue; }
            if (v == 0xFFFE) continue;
            if (cur == idx && v != 0) return v;
        }
        return 0;
    } else {
        const uint8_t *p = f->uni_raw;
        const uint8_t *end = p + f->uni_size;
        int cur = 0;
        while (p < end) {
            if (*p == 0xFF) { cur++; p++; if (cur > idx) break; continue; }
            if (*p == 0xFE) { p++; continue; }

            uint32_t cp = 0; int len = 0;
            if      ((*p & 0x80) == 0)    { cp = *p;        len = 1; }
            else if ((*p & 0xE0) == 0xC0) { cp = *p & 0x1F; len = 2; }
            else if ((*p & 0xF0) == 0xE0) { cp = *p & 0x0F; len = 3; }
            else if ((*p & 0xF8) == 0xF0) { cp = *p & 0x07; len = 4; }
            else { p++; continue; }

            if (p + len > end) break;
            for (int i = 1; i < len; i++) cp = (cp << 6) | (p[i] & 0x3F);
            p += len;

            if (cur == idx && cp != 0) return cp;
        }
        return 0;
    }
}



static void draw_glyph(const Font *f, int idx) {
    if (idx < 0 || idx >= f->numglyphs) {
        printf(_(M_OUT_OF_RANGE), f->numglyphs - 1);
        putchar('\n');
        return;
    }

    const uint8_t *g = f->glyphs + (size_t)idx * f->bytesperglyph;
    int stride = (f->width + 7) / 8;

    printf("\n");
    printf(_(M_GLYPH_HEADER), idx, f->width, f->height);
    printf("\n+");
    for (int x = 0; x < f->width; x++) putchar('-');
    printf("+\n");

    for (int y = 0; y < f->height; y++) {
        printf("|");
        for (int x = 0; x < f->width; x++) {
            int byte = y * stride + x / 8;
            int bit  = 7 - (x % 8);
            putchar((g[byte] >> bit) & 1 ? '#' : '.');
        }
        printf("|\n");
    }

    printf("+");
    for (int x = 0; x < f->width; x++) putchar('-');
    printf("+\n");

    uint32_t cp = glyph_unicode(f, idx);
    if (cp) {
        char u8[5] = {0};
        cp_to_utf8(cp, u8);
        printf(_(M_UNICODE_LABEL), (unsigned)cp, u8);
        putchar('\n');
    } else {
        printf("%s\n", _(M_NO_UNICODE));
    }
}

static void list_glyphs(const Font *f) {
    printf("\n%s\n", _(M_GLYPHS_HEADER));
    for (int i = 0; i < f->numglyphs; i++) {
        uint32_t cp = glyph_unicode(f, i);
        if (cp) {
            char u8[5] = {0};
            cp_to_utf8(cp, u8);
            printf("  %3d : U+%04X  '%s'\n", i, (unsigned)cp, u8);
        } else {
            printf("  %3d : ---\n", i);
        }
        if ((i + 1) % 16 == 0) {
            printf("%s", _(M_PRESS_ENTER));
            int c = getchar();
            if (c != '\n') flush_line();
            if (c == 'q' || c == 'Q') return;
        }
    }
}

static int find_by_unicode(const Font *f, uint32_t cp) {
    for (int i = 0; i < f->numglyphs; i++)
        if (glyph_unicode(f, i) == cp) return i;
    return -1;
}



static void print_menu(void) {
    printf("\n%s\n",  _(M_MENU_HEADER));
    printf("%s\n",    _(M_MENU_INFO));
    printf("%s\n",    _(M_MENU_LIST));
    printf("%s\n",    _(M_MENU_GLYPH));
    printf("%s\n",    _(M_MENU_FIND));
    printf("%s\n",    _(M_MENU_LANG));
    printf("%s\n",    _(M_MENU_EXIT));
    printf("%s",      _(M_PROMPT));
}



int main(int argc, char *argv[]) {
    detect_language_from_env();

    const char *filename = NULL;
    for (int i = 1; i < argc; i++) {
        const char *a = argv[i];
        if (!strcmp(a, "-l") || !strcmp(a, "--lang")) {
            if (i + 1 >= argc) { fprintf(stderr, _(M_USAGE), argv[0]); return EXIT_FAILURE; }
            int l = lang_from_string(argv[++i]);
            if (l < 0) { fprintf(stderr, "%s\n", _(M_LANG_UNKNOWN)); return EXIT_FAILURE; }
            g_lang = l;
        } else if (!strncmp(a, "--lang=", 7)) {
            int l = lang_from_string(a + 7);
            if (l < 0) { fprintf(stderr, "%s\n", _(M_LANG_UNKNOWN)); return EXIT_FAILURE; }
            g_lang = l;
        } else if (!filename && a[0] != '-') {
            filename = a;
        } else {
            fprintf(stderr, _(M_USAGE), argv[0]);
            fprintf(stderr, "\n");
            return EXIT_FAILURE;
        }
    }
    if (!filename) {
        fprintf(stderr, _(M_USAGE), argv[0]);
        fprintf(stderr, "\n");
        return EXIT_FAILURE;
    }

    Font f;
    if (load_psf(filename, &f) != 0) return EXIT_FAILURE;

    printf(_(M_LOADED), filename, f.version, f.numglyphs, f.width, f.height,
           f.has_unicode ? _(M_YES) : _(M_NO));
    putchar('\n');

    int running = 1;
    while (running) {
        print_menu();

        int choice;
        if (scanf("%d", &choice) != 1) { flush_line(); continue; }
        flush_line();

        switch (choice) {
        case 1:
            printf("\n%s\n", _(M_FONT_INFO));
            printf(_(M_FILE_LABEL), filename);                putchar('\n');
            printf(_(M_VERSION_LABEL), f.version);            putchar('\n');
            printf(_(M_GLYPHS_LABEL), f.numglyphs);           putchar('\n');
            printf(_(M_SIZE_LABEL), f.width, f.height, f.bytesperglyph); putchar('\n');
            printf(_(M_UNI_LABEL), f.has_unicode ? _(M_YES) : _(M_NO));  putchar('\n');
            break;

        case 2:
            list_glyphs(&f);
            break;

        case 3: {
            printf("%s", _(M_GLYPH_INDEX_PROMPT));

            printf("\r%s", _(M_GLYPH_INDEX_PROMPT));

            printf("\r");
            printf(_(M_GLYPH_INDEX_PROMPT), f.numglyphs - 1);
            int idx;
            if (scanf("%d", &idx) != 1) { flush_line(); break; }
            flush_line();
            draw_glyph(&f, idx);
            break;
        }

        case 4: {
            printf("%s", _(M_UNICODE_PROMPT));
            unsigned int cp;
            if (scanf("%x", &cp) != 1) { flush_line(); break; }
            flush_line();
            int idx = find_by_unicode(&f, cp);
            if (idx < 0) {
                printf(_(M_GLYPH_NOT_FOUND), cp);
                putchar('\n');
            } else {
                draw_glyph(&f, idx);
            }
            break;
        }

        case 5: {
            printf("%s", _(M_LANG_PROMPT));
            char buf[16];
            if (scanf("%15s", buf) != 1) { flush_line(); break; }
            flush_line();
            int l = lang_from_string(buf);
            if (l < 0) {
                printf("%s\n", _(M_LANG_UNKNOWN));
            } else {
                g_lang = l;
                printf(_(M_LANG_SET), lang_names[g_lang]);
                putchar('\n');
            }
            break;
        }

        case 0:
            running = 0;
            break;

        default:
            printf("%s\n", _(M_UNKNOWN_OPTION));
        }
    }

    font_free(&f);
    return EXIT_SUCCESS;
}