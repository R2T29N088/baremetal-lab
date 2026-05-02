# runtime layer（最小 C ランタイム）

## 1. はじめに

runtime layer は、OS や標準 libc に依存せずに C プログラムを実行するための
最小限の C ランタイム機能（セクション初期化・基本関数）を提供します。

C 言語ランタイム（CRT: C Runtime Library）は、
C 言語で作成されたプログラムを実行する際に必要となる基本的な初期化処理や
標準関数（`printf`、`malloc` など）を提供する仕組みです。
通常は OS が提供する標準 C ライブラリ（libc）がこれらの機能を持ちますが、
OS のない baremetal 環境では自前で準備する必要があります。

baremetal-lab では、最小の機能に絞ってランタイムを実装しています。
`main()` を実行する前に 初期値付き変数（`.data`） や 初期値なし変数（`.bss`） を初期化し、
`putc()` / `puts()` / `memcpy()` / `memset()` を提供しています。

本章では、これら最小ランタイムの構成要素について説明します。

## 2. `.data` / `.bss` セクション初期化

`runtime/init.c` に実装されている関数：CrtInit により `.data` / `.bss` セクション初期化が行われます。

Reset_Handler → SystemInit → CrtInit → main の順に実行されます。
（[起動シーケンスの詳細はこちらをご参照ください。](01_overview.md#4-起動シーケンス)）

CrtInit が完了した時点で、C 言語の実行環境として次が保証されます。

- `.ramfunc` の内容がフラッシュから RAM にコピーされている
- `.data` の初期値付き変数がフラッシュ上の初期値で初期化されている
- `.bss` の初期値なし変数がすべて 0 クリアされている

注意：C 言語の仕様上、初期値付き変数（`.data`）と初期値なし変数（`.bss`）は
「RAM 上に存在する」「プログラム開始前に初期化される」ことが前提となっているため、CrtInit による初期化が必要になります。

注意：`.ramfunc` セクションは、フラッシュではなく RAM 上から実行したい関数を配置するための領域です。
（例えば、フラッシュ書き換え中に実行するコードなど）

```
void CrtInit(void)
{
  extern uint32_t _siramfunc, _sramfunc, _eramfunc;
  extern uint32_t _sidata, _sdata, _edata;
  extern uint32_t _sbss, _ebss;

  uint32_t *src, *dst;

  /* .ramfunc */
  src = &_siramfunc;
  dst = &_sramfunc;
  while (dst < &_eramfunc)
    *dst++ = *src++;

  /* .data */
  src = &_sidata;
  dst = &_sdata;
  while (dst < &_edata)
    *dst++ = *src++;

  /* .bss */
  for (dst = &_sbss; dst < &_ebss; dst++)
    *dst = 0;
}
```

`.data` セクションの初期値が格納されているアドレスは `&_sidata` です。
`.data` セクションの先頭アドレスは `&_sdata` です。
`.data` セクションの終端アドレスは `&_edata` で示されます。
他のセクション（`.ramfunc` や `.bss`）も同様ですが、
これらのアドレスはリンカスクリプト `platform/qemu-lm3s6965evb/qemu-lm3s6965evb.ld` により決定されます。

```
MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 64K
  RAM   (rwx): ORIGIN = 0x20000000, LENGTH = 8K
}

SECTIONS
{
  .text :
  {
    _stext = .;
    KEEP(*(.vectors))
    KEEP(*(.reset_handler))
    *(.text*)
    *(.rodata*)
    KEEP(*(.init))
    KEEP(*(.fini))
    . = ALIGN(8);
    _etext = .;
  } > FLASH

  .ramfunc : AT(ADDR(.text) + SIZEOF(.text))
  {
    . = ALIGN(8);
    _sramfunc = .;
    *(.ramfunc*)
    _eramfunc = .;
  } > RAM
  _siramfunc = LOADADDR(.ramfunc);

  .data : AT(ADDR(.text) + SIZEOF(.text) + SIZEOF(.ramfunc))
  {
    . = ALIGN(8);
    _sdata = .;
    *(.data*)
    _edata = .;
  } > RAM
  _sidata = LOADADDR(.data);

  .bss :
  {
    . = ALIGN(8);
    _sbss = .;
    *(.bss*)
    *(COMMON)
    _ebss = .;
  } > RAM

  .msp (NOLOAD):
  {
    . = ALIGN(8);
    _msp_top = ORIGIN(RAM) + LENGTH(RAM);
    _msp_bottom = _msp_top - 1024;
  } > RAM
}
```

注意：`.text` は ROM の扱いでプログラムや固定値を配置します。
プログラムの後ろに変数領域を配置するのがセオリーで、このリンカスクリプトはセオリーに従って記述しています。
`.ramfunc` にはプログラムを格納しますが、`.data` や `.bss` の後ろにこれを配置すると、変数の追加・削除により、
プログラムの配置アドレスが変わることになります。
このように、RAM 上のコード（.ramfunc）と RAM 上の変数（.data / .bss）を明確に分離することで、
メモリ配置の安定性とデバッグのしやすさが向上します。

注意：スタックの初期化は startup.S で行われます。

## 3. RAMFUNC

`.ramfunc` セクションは、フラッシュではなく RAM 上から実行したい関数を配置するための領域です。
例えば以下のような場合に。
- フラッシュ書き換え中に実行するコード
- 割り込みレイテンシを極限まで下げたいコード
- フラッシュが低速な MCU で高速処理したいコード

`.ramfunc` セクションに関数を配置するには以下のように記述します。

```
#include <runtime/ramfunc.h>

RAMFUNC
void flash_write_sequence(void)
{
  ...
}
```

## 4. `putc()` / `puts()`

`include/runtime/console.h` をインクルードして使います。
関数仕様について `include/runtime/console.h` をご参照ください。

## 5. `memcpy()` / `memset()`

`include/runtime/mem.h` をインクルードして使います。
関数仕様について `include/runtime/mem.h` をご参照ください。
