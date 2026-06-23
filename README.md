
# 📘 Álbum de Figurinhas - Copa do Mundo 2026

Projeto prático desenvolvido para a disciplina de **Fundamentos de Programação 2** na **UTFPR**.

---

## 📋 Descrição

O sistema consiste em um álbum digital interativo desenvolvido em **linguagem C**, utilizando a biblioteca gráfica **Raylib**.

O projeto simula a coleção de figurinhas da Copa do Mundo de 2026, permitindo ao usuário colecionar, abrir pacotes, gerenciar o álbum e interagir com mini-games e loja de itens.

---

## 🚀 Funcionalidades

- 📦 Sistema de pacotes com 7 figurinhas aleatórias  
- 🧾 CRUD completo de figurinhas (criar, listar, buscar, atualizar e remover)  
- 🖼️ Álbum visual interativo com Raylib  
- 🎮 Mini-game de pênaltis  
- 🏪 Loja de pacotes com moedas  
- 📊 Estatísticas do progresso do álbum  
- 💾 Persistência de dados (CSV e binário)  
- 📤 Exportação do álbum para CSV  
- 🧩 Código modularizado em múltiplos arquivos  

---

## 🛠️ Tecnologias e Conceitos Utilizados

- Linguagem C (C99)  
- Biblioteca gráfica Raylib 5.0  
- Estruturas (`struct`)  
- Alocação dinâmica de memória (`malloc`, `realloc`, `free`)  
- Manipulação de arquivos (CSV e binário)  
- Programação modular  
- Makefile para automação de build  

---

## 📂 Estrutura do Projeto

```

projetoprogramacao2/
├── assets/
├── include/
├── src/
├── Makefile
├── README.md
└── .gitignore

````

---

## ⚙️ Como Compilar e Executar

---

# 🐧 Linux (Ubuntu/Debian)

## 📌 Pré-requisitos

```bash
sudo apt update
sudo apt install -y build-essential git cmake \
libasound2-dev libx11-dev libxrandr-dev \
libxi-dev libgl1-mesa-dev libglu1-mesa-dev \
libxcursor-dev libxinerama-dev
````

---

## 📦 Instalar Raylib

```bash
git clone https://github.com/raysan5/raylib.git
cd raylib
mkdir build && cd build
cmake ..
make
sudo make install
sudo ldconfig
```

---

## ▶️ Compilar projeto

```bash
make
```

---

## 🚀 Executar

```bash
make run
```

---

## 🧹 Limpar build

```bash
make clean
```

---

# 🪟 Windows (MSYS2)

## 📌 Pré-requisitos

Instale o MSYS2:
[https://www.msys2.org/](https://www.msys2.org/)

Abra MSYS2 UCRT64:

```bash
pacman -Syu
```

Depois:

```bash
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain \
mingw-w64-ucrt-x86_64-cmake git
```

---

## 📦 Instalar Raylib

```bash
git clone https://github.com/raysan5/raylib.git
cd raylib
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
mingw32-make install
```

---

## ▶️ Compilar projeto

```bash
make
```

ou:

```bash
mingw32-make
```

---

## 🚀 Executar

```bash
make run
```

ou:

```bash
album_copa2026.exe
```

---

## ⚠️ Observações

* Não commitar o executável no Git (use `.gitignore`)
* No Linux é necessário X11 (já configurado no Makefile)
* No Windows use MSYS2 UCRT64
* Raylib deve estar instalada no sistema

---

## 📌 🔧 Controle de versão (Git / GitHub)
## 🚀 Fluxo normal para atualizar o projeto

Sempre que fizer mudanças no código, use:

*git init
*git status
*git add .
*git commit -m "descrição clara da mudança"
*git push origin main

## 📌 Autor

Projeto acadêmico desenvolvido para fins educacionais.

