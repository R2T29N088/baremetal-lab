# 使い方

## 1. 環境

本プロジェクトのビルド・実行には以下が必要です。

- WSL + Ubuntu
- git
- GNU Make
- arm-none-eabi-gcc（GNU Arm Embedded Toolchain）
- QEMU（Cortex-M 対応）

## 2. WSL+Ubuntu環境構築

最初に WSL を有効化します。
Windows の Power Shell で下記を実行します。

```
$ wsl --install
$ wsl --set-default-version 2
```

次に Ubuntu をインストールします。

1. Windows の スタートメニューから Ubuntu と入力して検索
2. Ubuntu 24.04.1 LTS を選択

Ubuntuが起動すると Enter new UNIX username: と出力されるので、ユーザー名・パスワードを設定します。

注意：初回起動時、WslRegisterDistribution failed with error: 0x80370114 というようなエラーが出る場合、BIOSで仮想化支援機能（Intel VT-ｘ/AMD-V）が有効になっていない可能性があります。Windowsを再起動して、BIOS設定を確認してください。

Ubuntuにログイン後、開発環境の準備として、まずはパッケージを更新します。

```
$ sudo apt update
$ sudo apt upgrade -y
```

GNU Make をインストールします。

```
$ sudo apt install make
```

Windows から Ubuntu ディレクトリへアクセスするには、エクスプローラのアドレスバーに以下を入力します。Ubuntu-24.04 というドライブが見えるはずです。

```
\\wsl$
```

## 3. ビルド手順

以下のようにコマンドすることで baremetal-lab を clone します。

```
$ git clone git@github.com:R2T29N088/baremetal-lab.git
```
または
```
$ git clone https://github.com/R2T29N088/baremetal-lab.git
```

注意：SSHを使うには GitHub アカウントが必要です。「5. その他」をご参照ください。

以下のようにコマンドすることで、ARM CMSIS とツールチェーン（arm-none-eabi-gcc）をインストールします。

```
$ cd baremetal-lab
$ ./tools/scripts/get_cmsis.sh
$ ./tools/scripts/get_toolchain.sh
```

その後、以下のようにコマンドすることで、ビルドすることができます。

```
$ make
```

ビルドに成功すると `baremetal-lab/out` に下記ファイルが出力されます。

| ファイル | 説明 |
|---------|------|
| `qemu-lm3s6965evb.elf`    | 実行可能ファイル（ELF形式、デバッグ情報付き） |
| `qemu-lm3s6965evb.map`    | map ファイル |
| `qemu-lm3s6965evb.disasm` | ディスアセンブルファイル（ELFヘッダ・セクション情報・ディスアセンブルを含む、ELFファイルのダンプ） |
| `qemu-lm3s6965evb.bin`    | 実効可能ファイル（バイナリ形式、QEMU/実機向け） |

注意：`make clean` とコマンドすることで、これら成果物を削除することができます。

## 4. 動作確認@QEMU

以下のようにコマンドすることで、ARM用の QEMU をインストールすることができます。

```
$ sudo apt install qemu-system-arm
```

その後、以下のようにコマンドすることで、QEMU上で `./out/qemu-lm3s6965evb.elf` を実行することができます。

```
$ timeout 5s qemu-system-arm -M lm3s6965evb -kernel ./out/qemu-lm3s6965evb.elf -nographic
```
または
```
$ timeout 5s qemu-system-arm -M lm3s6965evb -kernel ./out/qemu-lm3s6965evb.bin -nographic
```

注意：`timeout 5s` は QEMU を 5 秒で強制終了させるためのコマンドです。

注意：`.elf` / `.bin` のどちらも QEMU で実行できます

実行すると、以下のログが出力されます。（timeout 5s を付けているため、5秒経過すると QEMU は自動的に終了します）

```
Timer with period zero, disabling
Hello World
qemu-system-arm: terminating on signal 15 from pid xxxx (timeout)
```

QEMUが終了しない場合、別プロンプトから以下のようにコマンドすることで、QEMU を終了することができます。

```
$ pkill qemu-system-arm
```

## 5. その他

GitHubからクローンする際、SSHを使う場合、GitHubアカウントが必要になります。https://github.com/ にアクセスして、アカウントを作成してください。

次にそのアカウントに対して SSH鍵 を設定します。
Ubuntu上で以下のようにコマンドすることで、SSH鍵 を生成することができます。

```
$ ssh-keygen -t ed25519 -C "あなたのGitHubメールアドレス"
```

実行すると、以下のように問われます。Enterを押してデフォルトの保存場所で大丈夫です。

```
Enter a file in which to save the key (/home/you/.ssh/id_ed25519):
```

以下のように問われます。パスフレーズを決めて入力してください。clone や push などするときに必要になるので覚えておきましょう。

```
Enter passphrase (empty for no passphrase):
```

以下のようにコマンドして、生成した鍵をコピーして GitHub 側に設定してください。（GitHub の Settings → SSH and GPG keys → New SSH key から登録できます。）

```
$ cat ~/.ssh/id_ed25519.pub
```

以下のようにコマンドして、GitHubと通信ができれば準備完了です。

```
$ ssh -T git@github.com
```

