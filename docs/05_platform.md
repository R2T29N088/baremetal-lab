# platform layer（SoC / ボード依存コード）

platform layer は SoC や ボード依存のコードを扱う層です。

SoC や ボードによって、以下の項目が決定されます。

- CPU
- ペリフェラル
- メモリマップ
- 電源・クロック

`Makefile` には以下のように記述されています。

```
TARGET ?= qemu-lm3s6965evb
PLATFORM_DIR := $(ROOT_DIR)platform/$(TARGET)/
include $(PLATFORM_DIR)platform.mk
```

`TARGET` に設定されている qemu-lm3s6965evb は QEMU というシミュレーターに含まれる
TI 社製 SoC lm3s6965 が搭載された評価ボードをターゲットにしていることを意味しています。

lm3s6965 のデータシート：https://www.ti.com/product/LM3S6965 を参照すると、
この SoC に搭載されている CPU 、ペリフェラル、メモリマップを知ることができます。

## 1. CPU

CPU は `include $(PLATFORM_DIR)platform.mk` により決定されます。

lm3s6965 に搭載されている CPU は ARM Cortex-M3 ですが、
qemu-lm3s6965evb では ARM Cortex-M0+ を選択しています。
baremetal-lab としては ARM Cortex-M 系で一番ベーシックな ARMv6-M アーキテクチャを
リファレンス実装としたかったので ARM Cortex-M0+ を選択しています。

注意：ARM Cortex-M3 は ARMv7-M アーキテクチャの CPU です。
この CPU は ARMv6-M アーキテクチャに互換性があり、
ARM Cortex-M0+ 用にビルドされたバイナリをそのまま実行することができます。
この互換性により qemu-lm3s6965evb において ARM Cortex-M0+ を選択することが可能となっています。

## 2. ペリフェラル

lm3s6965 には UART、GPIO、タイマ、ADC など複数のペリフェラルが搭載されています。
baremetal-lab では UART0 のみを利用しています。

UART0 のレジスタマップはデータシートに記載されており、
ベースアドレスは 0x4000C000 です。

`platform/qemu-lm3s6965evb/uart.c` では、このレジスタマップに基づき
以下の API を提供しています。

- platform_uart_init()
- platform_uart_putc()
- platform_uart_getc()（必要に応じて）

runtime layer の `console_putc()` / `console_puts()` は
これら platform API に委譲する形で実装されています。

## 3. メモリマップ

lm3s6965 のメモリマップは以下のように定義されています。

- FLASH : 0x0000_0000 〜
- SRAM  : 0x2000_0000 〜
- ペリフェラル : 0x4000_0000 〜

baremetal-lab のリンカスクリプト
`platform/qemu-lm3s6965evb/qemu-lm3s6965evb.ld` は
このメモリマップに基づいて `.text` / `.data` / `.bss` / `.ramfunc` を配置しています。

メモリレイアウトの詳細は [04_runtime layer](docs/04_runtime.md) で説明していますが、
platform layer は 「SoC が提供するメモリ空間をどう使うか」 を定義する役割を持ちます。

## 4. 電源・クロック

lm3s6965 のクロック構成はデータシートに従います。
baremetal-lab では system.c 内で以下の初期化を行っています。

- SysTick ディセーブル
- NVIC 全割り込みマスク
- UART0 の有効化
- CMSIS API : __enable_irq の呼び出しによる PRIMASK=0 

QEMU 上ではクロック設定の挙動は簡略化されているため、
実機の Stellaris LM3S6965 よりも初期化コードは少なくなっています。

## 5. 例外ベクタテーブル

`platform/qemu-lm3s6965evb/vector.c` では、
ARM Cortex-M の例外ベクタテーブルを定義しています。

- 初期 MSP
- Reset_Handler
- NMI_Handler
- HardFault_Handler
- UART0_Handler（必要に応じて）

例外ベクタテーブルは `.vectors` セクションとして配置され、
リンカスクリプトにより FLASH の先頭に配置されます。

注意：`Reset_Handler` は `arch/armv6-m/startup.S` に実装されています。

## 6. platform.mk

platform.mk は、この platform layer の“設定ファイル”として機能します。

```
ARCH := armv6-m
CPU := cortex-m0plus
export ARCH
export CPU
```

この設定により、Makefile は以下を決定します。

- arch/armv6-m/ を使用する
- CPU コアとして Cortex-M0+ を選択する

platform layer は 「SoC / ボードがどの CPU アーキテクチャを使うか」 を
Makefile に伝える役割を持っています。

## 7. platform.h

platform.h は、プラットフォーム依存の機能を提供するヘッダファイルです。
本プロジェクトでは、以下の 2 種類の定義を扱います。

- CPU コア側の基本的な定義（ARMCM0plus.h）
- SoC 固有の NVIC 割り込み番号

`include/platform/qemu-lm3s6965evb/platform.h` は以下のように記述しています。

```
#ifndef PLATFORM_H
#define PLATFORM_H

/*
 * ARMCM0plus.h is taken from the Cortex‑M0+ device template
 * provided in ARM CMSIS 5.9.0.
 *
 * It defines core‑level registers common to Cortex‑M0+ CPUs
 * (NVIC, SysTick, SCB, etc.) and does not include any SoC‑specific
 * peripherals or interrupt numbers.
 *
 * Core configuration macros such as __MPU_PRESENT and __VTOR_PRESENT
 * describe how the Cortex‑M0+ core is implemented in a given SoC,
 * and therefore depend on the SoC. For this reason, ARMCM0plus.h is
 * included from platform.h so that core definitions and SoC‑specific
 * settings can be combined.
 */
#include <ARMCM0plus.h>

/*
 * Platform-specific IRQ numbers for LM3S6965.
 * These values correspond to the NVIC interrupt numbers
 * defined in the device datasheet.
 */
#define PLATFORM_IRQ_GPIOA     0
#define PLATFORM_IRQ_GPIOB     1
#define PLATFORM_IRQ_GPIOC     2
#define PLATFORM_IRQ_GPIOD     3
#define PLATFORM_IRQ_GPIOE     4

#define PLATFORM_IRQ_UART0     5
#define PLATFORM_IRQ_UART1     6

#define PLATFORM_IRQ_TIMER0A   19
#define PLATFORM_IRQ_TIMER0B   20
#define PLATFORM_IRQ_TIMER1A   21
#define PLATFORM_IRQ_TIMER1B   22
#define PLATFORM_IRQ_TIMER2A   23
#define PLATFORM_IRQ_TIMER2B   24

#endif  /* PLATFORM_H */
```

### CPU コア定義（ARMCM0plus.h）

`include/platform/qemu-lm3s6965evb/ARMCM0plus.h` は、ARM CMSIS 5.9.0 に含まれる
Cortex‑M0+ 向けデバイステンプレートから引用したヘッダファイルで、
NVIC / SysTick / SCB など Cortex‑M0+ コアに共通するレジスタ定義や API を提供します。

SoC 固有の周辺機能や割り込み番号は含まれていません。

CPU コア側のコンフィギュレーション（__MPU_PRESENT / __VTOR_PRESENT など）は、
「その SoC がどのように Cortex‑M0+ コアを実装しているか」を示す値であり、SoC 依存の情報です。
そのため、platform.h から ARMCM0plus.h を include して CPU コア定義と SoC 固有定義を組み合わせています。

### LM3S6965 の割り込み番号

`include/platform/qemu-lm3s6965evb/platform.h` では、TI LM3S6965 の
データシート：https://www.ti.com/product/LM3S6965 に記載されている
NVIC 割り込み番号に基づいて IRQ 番号を定義しています。

これらの値は NVIC の ISER/ICER レジスタのビット位置として、そのまま利用できます。
