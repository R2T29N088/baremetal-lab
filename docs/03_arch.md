# arch layer（CPU 依存コード）

arch layer は CPU アーキテクチャに依存する機能（リセットハンドラ、低レベルCPU操作など）を提供する層です。
SoC や評価ボード依存のコード（UART、GPIO、クロックなど）は platform layer に分離されます。

CPU アーキテクチャは SoC や評価ボードとは独立した概念であり、
例えば ARM Cortex-M0 と ARM Cortex-M3 は同じく ARM 社の CPU ですが、異なる例外モデルや命令セットを持ちます。
そのため baremetal-lab では CPU 依存コードを CPU アーキテクチャ単位で分離しています。

この思想を表現しているのが `arch` フォルダで `arch/armv6-m/` は ARMv6-M というアーキテクチャに基づくCPUに向けた実装が格納されていることを意味します。

例えば、ARM Cortex-M0 や ARM Cortex-M0+ は ARMv6-M アーキテクチャに基づく CPU なので、
これら CPU が搭載されている SoC や評価ボードを用いる場合、このフォルダに格納されている実装を使用します。

どのようにして、これを選択するのか？

`Makefile` に答えが記述されています。

```
TARGET ?= qemu-lm3s6965evb
PLATFORM_DIR := $(ROOT_DIR)platform/$(TARGET)/
include $(PLATFORM_DIR)platform.mk

ARCH_DIR := $(ROOT_DIR)arch/$(ARCH)
```

`TARGET` は platform を指定するための変数です。
ここでは `qemu-lm3s6965evb` を初期値として設定していることになります。
これにより `PLATFORM_DIR` には `$(ROOT_DIR)platform/qemu-lm3s6965evb/` が設定されることになります。

そして `include $(PLATFORM_DIR)platform.mk` は `$(ROOT_DIR)platform/qemu-lm3s6965evb/platform.mk` というファイルをインクルードしなさい、という意味を持つことになります。

`$(ROOT_DIR)platform/qemu-lm3s6965evb/platform.mk` は下記のように記述されています。

```
ARCH := armv6-m
CPU := cortex-m0plus
export ARCH
export CPU
```

この記述により `ARCH_DIR` には `$(ROOT_DIR)arch/armv6-m` という値が格納されます。
これにより `qemu-lm3s6965evb` という platform では armv6-m という CPU アーキテクチャと CPU コアとして ARM Cortex-M0+ を選択していることになります。

注意：`ARCH` は CPU アーキテクチャを表し、`CPU` は、そのアーキテクチャを実装した具体的な CPU コアを表しています。

## 1. リセットハンドラ

[こちらをご参照ください。](01_overview.md#4-起動シーケンス)


## 2. 例外と割り込み

baremetal-lab は例外と割り込みの制御に以下のAPIを提供しています。

`arch_int_disable` / `arch_int_restore`

`arch_irq_enable` / `arch_irq_disable`

これらの API は ARM CMSIS により提供される CPU アーキテクチャや CPU コア依存の関数を抽象化したものであり、
これら API を利用することで、例外と割り込みの制御を実現します。

```
---------------------------------
| CPU core                      |
|   register : PRIMASK          |
---------------------------------
               ^
               | IRQ signal (CPU 全体の受付)
-------------------------------------------------
| NVIC (Nested Vectored Interrupt Controller)   |
|   register : ICER, ISER                       |
-------------------------------------------------
      ^                ^                   ^
      |                |                   |
 uart IRQ signal   gpio IRQ signal   timer IRQ signal
      |                |                   |
   --------           --------         ---------
   | UART |           | GPIO |         | TIMER |
   --------           --------         ---------
```

ARM Cortex-M は CPU コアに含まれるレジスタ：PRIMASK により CPU 全体の例外を制御します。
これを 1 にすると CPU は Hard Fault を除くすべての例外を無視します。
0 にすると、例外が受け付けられます。
`arch_int_disable` / `arch_int_restore` は、この CPU 全体の例外マスク操作を抽象化したものです。

ARM Cortex-M の場合、NVIC に含まれるレジスタ：ICER, ISER は、
各ペリフェラルの割り込み信号（上図では、uart IRQ, gpio IRQ, timer IRQ）の有効・無効を決定します。
レジスタ：ICER, ISER は 32bit のレジスタで、各ビット番号に各ペリフェラルが割り当てられています。
この割り当ては利用する platform により決まります。
qemu-lm3s6965evb の場合、lm3s6965 のデータシート：https://www.ti.com/product/LM3S6965 に
 InterruptNumber(Bit in Interrupt Registers) として UART0 は 5 と記載されているので、
レジスタ：ICER, ISER の bit5 を Set することで、UART0 の割り込み信号の有効・無効を決定することができると言えます。
`arch_irq_enable` / `arch_irq_disable` は、この操作を行います。

注意：NVICに含まれるレジスタ：ICER, ISER は、各々 Interrupt Clear Enable Register, Interrupt Set Enable Register を省略した名称です。

注意：実際に lm3s6965 の UART0 の割り込みを使うには、UART0 のレジスタ：UARTIM の設定も必要になります。
一般的に、ペリフェラルの割り込みを使うには、CPU側の例外制御（ARM Cortex-M では PRIMASK）、
割り込みコントローラ（ARM Cortex-M では NVIC）、そして、ペリフェラル本体のレジスタを適切に設定する必要があります。

関数仕様について `include/arch/arch.h` をご参照ください。
