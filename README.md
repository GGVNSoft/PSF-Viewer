# psf_viewer

Interactive console viewer for **PC Screen Font** files (`.psf`/`.psfu`) used by the Linux console

## Build & Run

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
./build/psf_viewer /usr/share/consolefonts/Uni2-VGA16.psf
```

Features

    PSF1 and PSF2 (with or without Unicode table)

    ASCII-art glyph rendering

    Glyph ↔ Unicode lookup both ways

    Menu-driven UI, paginated glyph list

    i18n: en / ru / es (auto from LANG, switchable at runtime)
    

Usage
psf_viewer <file.psf> [-l en|ru|es]

Menu: 1 info · 2 list glyphs · 3 glyph by index · 4 find by Unicode · 5 language · 0 exit.

Example — glyph A:
Glyph 65  (8x16)
+--------+
|..####..|
|.##..##.|
|##....##|
|########|
|##....##|
|##....##|
+--------+
Unicode: U+0041  char: 'A'

Notes

    Supports PC Screen Font only — .psf from games (Portable Sound Format) is unsupported.

    Adding a language: LANG_XX in enum + lang_names + tr_table block + one line in lang_from_string(). Missing strings fall back to English.

    Find fonts: find /usr/share -name '*.psf*'.
    
License

MIT
